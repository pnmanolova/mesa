/*
 * Copyright © 2008 Jérôme Glisse
 * Copyright © 2010 Marek Olšák <maraeo@gmail.com>
 * Copyright © 2015 Advanced Micro Devices, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS, AUTHORS
 * AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 */
/*
 * Authors:
 *      Marek Olšák <maraeo@gmail.com>
 */

#include "amdgpu_cs.h"
#include "os/os_time.h"
#include <stdio.h>
#include <amdgpu_drm.h>


/* FENCES */

static struct pipe_fence_handle *
amdgpu_fence_create(struct amdgpu_ctx *ctx, unsigned ip_type,
                    unsigned ip_instance, unsigned ring)
{
   struct amdgpu_fence *fence = CALLOC_STRUCT(amdgpu_fence);

   fence->reference.count = 1;
   fence->ctx = ctx;
   fence->ip_type = ip_type;
   fence->ip_instance = ip_instance;
   fence->ring = ring;
   fence->submission_in_progress = true;
   p_atomic_inc(&ctx->refcount);
   return (struct pipe_fence_handle *)fence;
}

static void amdgpu_fence_submitted(struct pipe_fence_handle *fence,
                                   uint64_t fence_id)
{
   struct amdgpu_fence *rfence = (struct amdgpu_fence*)fence;

   rfence->fence = fence_id;
   rfence->submission_in_progress = false;
}

static void amdgpu_fence_signalled(struct pipe_fence_handle *fence)
{
   struct amdgpu_fence *rfence = (struct amdgpu_fence*)fence;

   rfence->signalled = true;
}

bool amdgpu_fence_wait(struct pipe_fence_handle *fence, uint64_t timeout,
                       bool absolute)
{
   struct amdgpu_fence *rfence = (struct amdgpu_fence*)fence;
   struct amdgpu_cs_query_fence query = {0};
   uint32_t expired;
   int64_t abs_timeout;
   int r;

   if (rfence->signalled)
      return true;

   if (absolute)
      abs_timeout = timeout;
   else
      abs_timeout = os_time_get_absolute_timeout(timeout);

   /* The fence might not have a number assigned if its IB is being
    * submitted in the other thread right now. Wait until the submission
    * is done. */
   if (!os_wait_until_zero_abs_timeout(&rfence->submission_in_progress,
                                       abs_timeout))
      return false;

   /* Now use the libdrm query. */
   query.fence = rfence->fence;
   query.context = rfence->ctx->ctx;
   query.ip_type = rfence->ip_type;
   query.ip_instance = rfence->ip_instance;
   query.ring = rfence->ring;

   r = amdgpu_cs_query_fence_status(&query,
				    abs_timeout,
				    AMDGPU_QUERY_FENCE_TIMEOUT_IS_ABSOLUTE,
				    &expired);
   if (r) {
      fprintf(stderr, "amdgpu: amdgpu_cs_query_fence_status failed.\n");
      return FALSE;
   }

   if (expired) {
      /* This variable can only transition from false to true, so it doesn't
       * matter if threads race for it. */
      rfence->signalled = true;
      return true;
   }
   return false;
}

static bool amdgpu_fence_wait_rel_timeout(struct radeon_winsys *rws,
                                          struct pipe_fence_handle *fence,
                                          uint64_t timeout)
{
   return amdgpu_fence_wait(fence, timeout, false);
}

/* CONTEXTS */

static struct radeon_winsys_ctx *amdgpu_ctx_create(struct radeon_winsys *ws)
{
   struct amdgpu_ctx *ctx = CALLOC_STRUCT(amdgpu_ctx);
   int r;

   ctx->ws = amdgpu_winsys(ws);
   ctx->refcount = 1;

   r = amdgpu_cs_ctx_create(ctx->ws->dev, &ctx->ctx);
   if (r) {
      fprintf(stderr, "amdgpu: amdgpu_cs_ctx_create failed. (%i)\n", r);
      FREE(ctx);
      return NULL;
   }

   return (struct radeon_winsys_ctx*)ctx;
}

static void amdgpu_ctx_destroy(struct radeon_winsys_ctx *rwctx)
{
   amdgpu_ctx_unref((struct amdgpu_ctx*)rwctx);
}

static enum pipe_reset_status
amdgpu_ctx_query_reset_status(struct radeon_winsys_ctx *rwctx)
{
   struct amdgpu_ctx *ctx = (struct amdgpu_ctx*)rwctx;
   uint32_t result, hangs;
   int r;

   r = amdgpu_cs_query_reset_state(ctx->ctx, &result, &hangs);
   if (r) {
      fprintf(stderr, "amdgpu: amdgpu_cs_query_reset_state failed. (%i)\n", r);
      return PIPE_NO_RESET;
   }

   switch (result) {
   case AMDGPU_CTX_GUILTY_RESET:
      return PIPE_GUILTY_CONTEXT_RESET;
   case AMDGPU_CTX_INNOCENT_RESET:
      return PIPE_INNOCENT_CONTEXT_RESET;
   case AMDGPU_CTX_UNKNOWN_RESET:
      return PIPE_UNKNOWN_CONTEXT_RESET;
   case AMDGPU_CTX_NO_RESET:
   default:
      return PIPE_NO_RESET;
   }
}

/* COMMAND SUBMISSION */

static bool amdgpu_get_new_ib(struct amdgpu_cs *cs)
{
   struct amdgpu_cs_context *cur_cs = cs->csc;
   /* The maximum size is 4MB - 1B, which is unaligned.
    * Use aligned size 4MB - 16B. */
   const unsigned max_ib_size = (1024 * 1024 - 16) * 4;
   const unsigned min_ib_size = 24 * 1024 * 4;

   cs->base.cdw = 0;
   cs->base.buf = NULL;

   /* Allocate a new buffer for IBs if the current buffer is all used. */
   if (!cs->big_ib_buffer ||
       cs->used_ib_space + min_ib_size > cs->big_ib_buffer->size) {
      struct radeon_winsys *ws = &cs->ctx->ws->base;
      struct radeon_winsys_cs_handle *winsys_bo;

      pb_reference(&cs->big_ib_buffer, NULL);
      cs->big_ib_winsys_buffer = NULL;
      cs->ib_mapped = NULL;
      cs->used_ib_space = 0;

      cs->big_ib_buffer = ws->buffer_create(ws, max_ib_size,
                                            4096, true,
                                            RADEON_DOMAIN_GTT,
                                            RADEON_FLAG_CPU_ACCESS);
      if (!cs->big_ib_buffer)
         return false;

      winsys_bo = ws->buffer_get_cs_handle(cs->big_ib_buffer);

      cs->ib_mapped = ws->buffer_map(winsys_bo, NULL, PIPE_TRANSFER_WRITE);
      if (!cs->ib_mapped) {
         pb_reference(&cs->big_ib_buffer, NULL);
         return false;
      }

      cs->big_ib_winsys_buffer = (struct amdgpu_winsys_bo*)winsys_bo;
   }

   pb_reference(&cur_cs->ib_buffer, cs->big_ib_buffer);
   cur_cs->ib_winsys_buffer = cs->big_ib_winsys_buffer;
   cur_cs->ib.ib_mc_address = cs->big_ib_winsys_buffer->va + cs->used_ib_space;
   cs->base.buf = (uint32_t*)(cs->ib_mapped + cs->used_ib_space);
   cs->base.max_dw = (cs->big_ib_buffer->size - cs->used_ib_space) / 4;
   return true;
}

static boolean amdgpu_init_cs_context(struct amdgpu_cs_context *csc,
                                      enum ring_type ring_type)
{
   int i;

   switch (ring_type) {
   case RING_DMA:
      csc->request.ip_type = AMDGPU_HW_IP_DMA;
      break;

   case RING_UVD:
      csc->request.ip_type = AMDGPU_HW_IP_UVD;
      break;

   case RING_VCE:
      csc->request.ip_type = AMDGPU_HW_IP_VCE;
      break;

   case RING_COMPUTE:
      csc->request.ip_type = AMDGPU_HW_IP_COMPUTE;
      break;

   default:
   case RING_GFX:
      csc->request.ip_type = AMDGPU_HW_IP_GFX;
      break;
   }

   csc->request.number_of_ibs = 1;
   csc->request.ibs = &csc->ib;

   csc->max_num_buffers = 512;
   csc->buffers = (struct amdgpu_cs_buffer*)
                  CALLOC(1, csc->max_num_buffers * sizeof(struct amdgpu_cs_buffer));
   if (!csc->buffers) {
      return FALSE;
   }

   csc->handles = CALLOC(1, csc->max_num_buffers * sizeof(amdgpu_bo_handle));
   if (!csc->handles) {
      FREE(csc->buffers);
      return FALSE;
   }

   csc->flags = CALLOC(1, csc->max_num_buffers);
   if (!csc->flags) {
      FREE(csc->handles);
      FREE(csc->buffers);
      return FALSE;
   }

   for (i = 0; i < Elements(csc->buffer_indices_hashlist); i++) {
      csc->buffer_indices_hashlist[i] = -1;
   }
   return TRUE;
}

static void amdgpu_cs_context_cleanup(struct amdgpu_cs_context *csc)
{
   unsigned i;

   for (i = 0; i < csc->num_buffers; i++) {
      p_atomic_dec(&csc->buffers[i].bo->num_cs_references);
      amdgpu_winsys_bo_reference(&csc->buffers[i].bo, NULL);
      csc->handles[i] = NULL;
      csc->flags[i] = 0;
   }

   csc->num_buffers = 0;
   csc->used_gart = 0;
   csc->used_vram = 0;
   amdgpu_fence_reference(&csc->fence, NULL);

   for (i = 0; i < Elements(csc->buffer_indices_hashlist); i++) {
      csc->buffer_indices_hashlist[i] = -1;
   }
}

static void amdgpu_destroy_cs_context(struct amdgpu_cs_context *csc)
{
   amdgpu_cs_context_cleanup(csc);
   pb_reference(&csc->ib_buffer, NULL);
   FREE(csc->flags);
   FREE(csc->buffers);
   FREE(csc->handles);
   FREE(csc->request.dependencies);
}


static struct radeon_winsys_cs *
amdgpu_cs_create(struct radeon_winsys_ctx *rwctx,
                 enum ring_type ring_type,
                 void (*flush)(void *ctx, unsigned flags,
                               struct pipe_fence_handle **fence),
                 void *flush_ctx,
                 struct radeon_winsys_cs_handle *trace_buf)
{
   struct amdgpu_ctx *ctx = (struct amdgpu_ctx*)rwctx;
   struct amdgpu_cs *cs;

   cs = CALLOC_STRUCT(amdgpu_cs);
   if (!cs) {
      return NULL;
   }

   pipe_semaphore_init(&cs->flush_completed, 1);

   cs->ctx = ctx;
   cs->flush_cs = flush;
   cs->flush_data = flush_ctx;

   if (!amdgpu_init_cs_context(&cs->csc1, ring_type)) {
      FREE(cs);
      return NULL;
   }
   if (!amdgpu_init_cs_context(&cs->csc2, ring_type)) {
      amdgpu_destroy_cs_context(&cs->csc1);
      FREE(cs);
      return NULL;
   }

   /* Set the first command buffer as current. */
   cs->csc = &cs->csc1;
   cs->cst = &cs->csc2;
   cs->base.ring_type = ring_type;

   if (!amdgpu_get_new_ib(cs)) {
      amdgpu_destroy_cs_context(&cs->csc2);
      amdgpu_destroy_cs_context(&cs->csc1);
      FREE(cs);
      return NULL;
   }

   p_atomic_inc(&ctx->ws->num_cs);
   return &cs->base;
}

#define OUT_CS(cs, value) (cs)->buf[(cs)->cdw++] = (value)

int amdgpu_get_reloc(struct amdgpu_cs_context *csc, struct amdgpu_winsys_bo *bo)
{
   unsigned hash = bo->unique_id & (Elements(csc->buffer_indices_hashlist)-1);
   int i = csc->buffer_indices_hashlist[hash];

   /* not found or found */
   if (i == -1 || csc->buffers[i].bo == bo)
      return i;

   /* Hash collision, look for the BO in the list of relocs linearly. */
   for (i = csc->num_buffers - 1; i >= 0; i--) {
      if (csc->buffers[i].bo == bo) {
         /* Put this reloc in the hash list.
          * This will prevent additional hash collisions if there are
          * several consecutive get_reloc calls for the same buffer.
          *
          * Example: Assuming buffers A,B,C collide in the hash list,
          * the following sequence of relocs:
          *         AAAAAAAAAAABBBBBBBBBBBBBBCCCCCCCC
          * will collide here: ^ and here:   ^,
          * meaning that we should get very few collisions in the end. */
         csc->buffer_indices_hashlist[hash] = i;
         return i;
      }
   }
   return -1;
}

static unsigned amdgpu_add_reloc(struct amdgpu_cs *cs,
                                 struct amdgpu_winsys_bo *bo,
                                 enum radeon_bo_usage usage,
                                 enum radeon_bo_domain domains,
                                 unsigned priority,
                                 enum radeon_bo_domain *added_domains)
{
   struct amdgpu_cs_context *csc = cs->csc;
   struct amdgpu_cs_buffer *reloc;
   unsigned hash = bo->unique_id & (Elements(csc->buffer_indices_hashlist)-1);
   int i = -1;

   priority = MIN2(priority, 15);
   *added_domains = 0;

   i = amdgpu_get_reloc(csc, bo);

   if (i >= 0) {
      reloc = &csc->buffers[i];
      reloc->usage |= usage;
      *added_domains = domains & ~reloc->domains;
      reloc->domains |= domains;
      csc->flags[i] = MAX2(csc->flags[i], priority);
      return i;
   }

   /* New relocation, check if the backing array is large enough. */
   if (csc->num_buffers >= csc->max_num_buffers) {
      uint32_t size;
      csc->max_num_buffers += 10;

      size = csc->max_num_buffers * sizeof(struct amdgpu_cs_buffer);
      csc->buffers = realloc(csc->buffers, size);

      size = csc->max_num_buffers * sizeof(amdgpu_bo_handle);
      csc->handles = realloc(csc->handles, size);

      csc->flags = realloc(csc->flags, csc->max_num_buffers);
   }

   /* Initialize the new relocation. */
   csc->buffers[csc->num_buffers].bo = NULL;
   amdgpu_winsys_bo_reference(&csc->buffers[csc->num_buffers].bo, bo);
   csc->handles[csc->num_buffers] = bo->bo;
   csc->flags[csc->num_buffers] = priority;
   p_atomic_inc(&bo->num_cs_references);
   reloc = &csc->buffers[csc->num_buffers];
   reloc->bo = bo;
   reloc->usage = usage;
   reloc->domains = domains;

   csc->buffer_indices_hashlist[hash] = csc->num_buffers;

   *added_domains = domains;
   return csc->num_buffers++;
}

static unsigned amdgpu_cs_add_reloc(struct radeon_winsys_cs *rcs,
                                    struct radeon_winsys_cs_handle *buf,
                                    enum radeon_bo_usage usage,
                                    enum radeon_bo_domain domains,
                                    enum radeon_bo_priority priority)
{
   /* Don't use the "domains" parameter. Amdgpu doesn't support changing
    * the buffer placement during command submission.
    */
   struct amdgpu_cs *cs = amdgpu_cs(rcs);
   struct amdgpu_winsys_bo *bo = (struct amdgpu_winsys_bo*)buf;
   enum radeon_bo_domain added_domains;
   unsigned index = amdgpu_add_reloc(cs, bo, usage, bo->initial_domain,
                                     priority, &added_domains);

   if (added_domains & RADEON_DOMAIN_GTT)
      cs->csc->used_gart += bo->base.size;
   if (added_domains & RADEON_DOMAIN_VRAM)
      cs->csc->used_vram += bo->base.size;

   return index;
}

static int amdgpu_cs_get_reloc(struct radeon_winsys_cs *rcs,
                               struct radeon_winsys_cs_handle *buf)
{
   struct amdgpu_cs *cs = amdgpu_cs(rcs);

   return amdgpu_get_reloc(cs->csc, (struct amdgpu_winsys_bo*)buf);
}

static boolean amdgpu_cs_validate(struct radeon_winsys_cs *rcs)
{
   return TRUE;
}

static boolean amdgpu_cs_memory_below_limit(struct radeon_winsys_cs *rcs, uint64_t vram, uint64_t gtt)
{
   struct amdgpu_cs *cs = amdgpu_cs(rcs);
   boolean status =
         (cs->csc->used_gart + gtt) < cs->ctx->ws->info.gart_size * 0.7 &&
         (cs->csc->used_vram + vram) < cs->ctx->ws->info.vram_size * 0.7;

   return status;
}

void amdgpu_cs_emit_ioctl_oneshot(struct amdgpu_cs *cs, struct amdgpu_cs_context *csc)
{
   struct amdgpu_winsys *ws = cs->ctx->ws;
   int i, j, r;
   uint64_t fence;

   csc->request.number_of_dependencies = 0;

   /* Since the kernel driver doesn't synchronize execution between different
    * rings automatically, we have to add fence dependencies manually. */
   pipe_mutex_lock(ws->bo_fence_lock);
   for (i = 0; i < csc->num_buffers; i++) {
      for (j = 0; j < RING_LAST; j++) {
         struct amdgpu_cs_dep_info *dep;
         unsigned idx;

         struct amdgpu_fence *bo_fence = (void *)csc->buffers[i].bo->fence[j];
         if (!bo_fence)
            continue;

         if (bo_fence->ctx == cs->ctx &&
             bo_fence->ip_type == csc->request.ip_type &&
             bo_fence->ip_instance == csc->request.ip_instance &&
             bo_fence->ring == csc->request.ring)
            continue;

         if (amdgpu_fence_wait((void *)bo_fence, 0, false))
            continue;

         idx = csc->request.number_of_dependencies++;
         if (idx >= csc->max_dependencies) {
            unsigned size;

            csc->max_dependencies = idx + 8;
            size = csc->max_dependencies * sizeof(struct amdgpu_cs_dep_info);
            csc->request.dependencies = realloc(csc->request.dependencies, size);
         }

         dep = &csc->request.dependencies[idx];
         dep->context = bo_fence->ctx->ctx;
         dep->ip_type = bo_fence->ip_type;
         dep->ip_instance = 0;
         dep->ring = bo_fence->ring;
         dep->fence = bo_fence->fence;
      }
   }
   pipe_mutex_unlock(ws->bo_fence_lock);

   r = amdgpu_cs_submit(cs->ctx->ctx, 0, &csc->request, 1, &fence);
   if (r) {
      fprintf(stderr, "amdgpu: The CS has been rejected, "
              "see dmesg for more information.\n");

      amdgpu_fence_signalled(csc->fence);
   } else {
      /* Success. */
      amdgpu_fence_submitted(csc->fence, fence);

      pipe_mutex_lock(ws->bo_fence_lock);
      for (i = 0; i < csc->num_buffers; i++)
         amdgpu_fence_reference(&csc->buffers[i].bo->fence[cs->base.ring_type],
                                csc->fence);
      pipe_mutex_unlock(ws->bo_fence_lock);
   }

   /* Cleanup. */
   if (csc->request.resources)
      amdgpu_bo_list_destroy(csc->request.resources);

   for (i = 0; i < csc->num_buffers; i++) {
      p_atomic_dec(&csc->buffers[i].bo->num_active_ioctls);
   }
   amdgpu_cs_context_cleanup(csc);
}

/*
 * Make sure previous submission of this cs are completed
 */
void amdgpu_cs_sync_flush(struct radeon_winsys_cs *rcs)
{
   struct amdgpu_cs *cs = amdgpu_cs(rcs);

   /* Wait for any pending ioctl to complete. */
   if (cs->ctx->ws->thread) {
      pipe_semaphore_wait(&cs->flush_completed);
      pipe_semaphore_signal(&cs->flush_completed);
   }
}

DEBUG_GET_ONCE_BOOL_OPTION(noop, "RADEON_NOOP", FALSE)

static void amdgpu_cs_flush(struct radeon_winsys_cs *rcs,
                            unsigned flags,
                            struct pipe_fence_handle **fence,
                            uint32_t cs_trace_id)
{
   struct amdgpu_cs *cs = amdgpu_cs(rcs);
   struct amdgpu_winsys *ws = cs->ctx->ws;
   struct amdgpu_cs_context *tmp;

   switch (cs->base.ring_type) {
   case RING_DMA:
      /* pad DMA ring to 8 DWs */
      if (ws->info.chip_class <= SI) {
         while (rcs->cdw & 7)
            OUT_CS(&cs->base, 0xf0000000); /* NOP packet */
      } else {
         while (rcs->cdw & 7)
            OUT_CS(&cs->base, 0x00000000); /* NOP packet */
      }
      break;
   case RING_GFX:
      /* pad DMA ring to 8 DWs to meet CP fetch alignment requirements
             * r6xx, requires at least 4 dw alignment to avoid a hw bug.
             */
      if (ws->info.chip_class <= SI) {
         while (rcs->cdw & 7)
            OUT_CS(&cs->base, 0x80000000); /* type2 nop packet */
      } else {
         while (rcs->cdw & 7)
            OUT_CS(&cs->base, 0xffff1000); /* type3 nop packet */
      }
      break;
   case RING_UVD:
      while (rcs->cdw & 15)
         OUT_CS(&cs->base, 0x80000000); /* type2 nop packet */
      break;
   default:
      break;
   }

   if (rcs->cdw > rcs->max_dw) {
      fprintf(stderr, "amdgpu: command stream overflowed\n");
   }

   amdgpu_cs_add_reloc(rcs, (void*)cs->csc->ib_winsys_buffer,
		       RADEON_USAGE_READ, 0, RADEON_PRIO_MIN);

   amdgpu_cs_sync_flush(rcs);

   /* Swap command streams. */
   tmp = cs->csc;
   cs->csc = cs->cst;
   cs->cst = tmp;

   /* If the CS is not empty or overflowed, emit it in a separate thread. */
   if (cs->base.cdw && cs->base.cdw <= cs->base.max_dw && !debug_get_option_noop()) {
      unsigned i, num_buffers = cs->cst->num_buffers;
      int r;

      r = amdgpu_bo_list_create(ws->dev, cs->cst->num_buffers,
                                cs->cst->handles, cs->cst->flags,
                                &cs->cst->request.resources);

      if (r) {
         fprintf(stderr, "amdgpu: resource list creation failed (%d)\n", r);
         cs->cst->request.resources = NULL;
      }

      cs->cst->ib.size = cs->base.cdw;
      cs->used_ib_space += cs->base.cdw * 4;

      for (i = 0; i < num_buffers; i++) {
         /* Update the number of active asynchronous CS ioctls for the buffer. */
         p_atomic_inc(&cs->cst->buffers[i].bo->num_active_ioctls);
      }

      amdgpu_fence_reference(&cs->cst->fence, NULL);
      cs->cst->fence = amdgpu_fence_create(cs->ctx,
                                           cs->cst->request.ip_type,
                                           cs->cst->request.ip_instance,
                                           cs->cst->request.ring);

      if (fence)
         amdgpu_fence_reference(fence, cs->cst->fence);

      if (ws->thread) {
         pipe_semaphore_wait(&cs->flush_completed);
         amdgpu_ws_queue_cs(ws, cs);
         if (!(flags & RADEON_FLUSH_ASYNC))
            amdgpu_cs_sync_flush(rcs);
      } else {
         amdgpu_cs_emit_ioctl_oneshot(cs, cs->cst);
      }
   } else {
      amdgpu_cs_context_cleanup(cs->cst);
   }

   amdgpu_get_new_ib(cs);

   ws->num_cs_flushes++;
}

static void amdgpu_cs_destroy(struct radeon_winsys_cs *rcs)
{
   struct amdgpu_cs *cs = amdgpu_cs(rcs);

   amdgpu_cs_sync_flush(rcs);
   pipe_semaphore_destroy(&cs->flush_completed);
   amdgpu_cs_context_cleanup(&cs->csc1);
   amdgpu_cs_context_cleanup(&cs->csc2);
   p_atomic_dec(&cs->ctx->ws->num_cs);
   amdgpu_destroy_cs_context(&cs->csc1);
   amdgpu_destroy_cs_context(&cs->csc2);
   pb_reference(&cs->big_ib_buffer, NULL);
   FREE(cs);
}

static boolean amdgpu_bo_is_referenced(struct radeon_winsys_cs *rcs,
                                       struct radeon_winsys_cs_handle *_buf,
                                       enum radeon_bo_usage usage)
{
   struct amdgpu_cs *cs = amdgpu_cs(rcs);
   struct amdgpu_winsys_bo *bo = (struct amdgpu_winsys_bo*)_buf;

   return amdgpu_bo_is_referenced_by_cs_with_usage(cs, bo, usage);
}

void amdgpu_cs_init_functions(struct amdgpu_winsys *ws)
{
   ws->base.ctx_create = amdgpu_ctx_create;
   ws->base.ctx_destroy = amdgpu_ctx_destroy;
   ws->base.ctx_query_reset_status = amdgpu_ctx_query_reset_status;
   ws->base.cs_create = amdgpu_cs_create;
   ws->base.cs_destroy = amdgpu_cs_destroy;
   ws->base.cs_add_reloc = amdgpu_cs_add_reloc;
   ws->base.cs_get_reloc = amdgpu_cs_get_reloc;
   ws->base.cs_validate = amdgpu_cs_validate;
   ws->base.cs_memory_below_limit = amdgpu_cs_memory_below_limit;
   ws->base.cs_flush = amdgpu_cs_flush;
   ws->base.cs_is_buffer_referenced = amdgpu_bo_is_referenced;
   ws->base.cs_sync_flush = amdgpu_cs_sync_flush;
   ws->base.fence_wait = amdgpu_fence_wait_rel_timeout;
   ws->base.fence_reference = amdgpu_fence_reference;
}
