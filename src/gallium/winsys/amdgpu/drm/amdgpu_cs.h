/*
 * Copyright © 2011 Marek Olšák <maraeo@gmail.com>
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

#ifndef AMDGPU_CS_H
#define AMDGPU_CS_H

#include "amdgpu_bo.h"
#include "util/u_memory.h"

struct amdgpu_cs_buffer {
   struct amdgpu_winsys_bo *bo;
   enum radeon_bo_usage usage;
   enum radeon_bo_domain domains;
};

struct amdgpu_cs_context {
   struct amdgpu_cs_request    request;
   struct amdgpu_cs_ib_info    ib;

   /* Relocs. */
   unsigned                    max_num_buffers;
   unsigned                    num_buffers;
   amdgpu_bo_handle            *handles;
   uint8_t                     *flags;
   struct amdgpu_cs_buffer     *buffers;

   int                         buffer_indices_hashlist[512];

   unsigned                    used_vram;
   unsigned                    used_gart;

   struct pipe_fence_handle    *fence;
};

struct amdgpu_cs {
   struct radeon_winsys_cs base;

   /* We flip between these two CS. While one is being consumed
    * by the kernel in another thread, the other one is being filled
    * by the pipe driver. */
   struct amdgpu_cs_context csc1;
   struct amdgpu_cs_context csc2;
   /* The currently-used CS. */
   struct amdgpu_cs_context *csc;
   /* The CS being currently-owned by the other thread. */
   struct amdgpu_cs_context *cst;

   /* The winsys. */
   struct amdgpu_winsys *ws;

   /* Flush CS. */
   void (*flush_cs)(void *ctx, unsigned flags, struct pipe_fence_handle **fence);
   void *flush_data;

   pipe_semaphore flush_completed;
};

struct amdgpu_fence {
   struct pipe_reference reference;

   uint64_t fence;          /* fence ID */
   unsigned ip_type;        /* which hw ip block the fence belongs to */
   uint32_t ring;           /* ring index of the hw ip block */

   /* If the fence is unknown due to an IB still being submitted
     * in the other thread. */
   bool submission_in_progress;
   bool signalled;
};

static INLINE void amdgpu_fence_reference(struct pipe_fence_handle **dst,
                                          struct pipe_fence_handle *src)
{
   struct amdgpu_fence **rdst = (struct amdgpu_fence **)dst;
   struct amdgpu_fence *rsrc = (struct amdgpu_fence *)src;

   if (pipe_reference(&(*rdst)->reference, &rsrc->reference))
      FREE(*rdst);
   *rdst = rsrc;
}

int amdgpu_get_reloc(struct amdgpu_cs_context *csc, struct amdgpu_winsys_bo *bo);

static INLINE struct amdgpu_cs *
amdgpu_cs(struct radeon_winsys_cs *base)
{
   return (struct amdgpu_cs*)base;
}

static INLINE boolean
amdgpu_bo_is_referenced_by_cs(struct amdgpu_cs *cs,
                              struct amdgpu_winsys_bo *bo)
{
   int num_refs = bo->num_cs_references;
   return num_refs == bo->rws->num_cs ||
         (num_refs && amdgpu_get_reloc(cs->csc, bo) != -1);
}

static INLINE boolean
amdgpu_bo_is_referenced_by_cs_with_usage(struct amdgpu_cs *cs,
                                         struct amdgpu_winsys_bo *bo,
                                         enum radeon_bo_usage usage)
{
   int index;

   if (!bo->num_cs_references)
      return FALSE;

   index = amdgpu_get_reloc(cs->csc, bo);
   if (index == -1)
      return FALSE;

   return (cs->csc->buffers[index].usage & usage) != 0;
}

static INLINE boolean
amdgpu_bo_is_referenced_by_any_cs(struct amdgpu_winsys_bo *bo)
{
   return bo->num_cs_references != 0;
}

void amdgpu_cs_sync_flush(struct radeon_winsys_cs *rcs);
void amdgpu_cs_init_functions(struct amdgpu_winsys *ws);
void amdgpu_cs_emit_ioctl_oneshot(struct amdgpu_cs *cs, struct amdgpu_cs_context *csc);

#endif
