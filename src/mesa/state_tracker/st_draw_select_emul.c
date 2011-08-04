/**************************************************************************
 * 
 * Copyright 2011 Micael Dias <kam1kaz3 (at) gmail (dot) com>,
 *                Pierre-Eric Pelloux-Prayer
 * All Rights Reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL TUNGSTEN GRAPHICS AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 **************************************************************************/

/**
 * This file contains code that attempts to accelerate GL_SELECT rendering.
 * It was created with the intent of solving the following bug:
 *     https://bugs.freedesktop.org/show_bug.cgi?id=34495
 *
 * This code is a rewritten version of the patch "GL_SELECT hw support v5"
 * posted by user Pierre-Eric Pelloux-Prayer in the previously mentioned bug.
 *
 * In order to test this code, the env. variable MESA_HW_SELECT should be
 * exported.
 *
 * How this algorithm works:
 * - When in GL_SELECT mode, draw calls are redirected to st_select_draw_func()
 * which configures states in a way that makes the rendered primitives draw
 * to a custom FBO that stores depth values (both min and max values) and
 * emits these values through _mesa_update_hitflag().
 */

#include "main/imports.h"
#include "main/image.h"
#include "main/macros.h"
#include "main/mfeatures.h"
#include "main/hash.h"

#include "main/context.h"
#include "main/enable.h"
#include "main/fbobject.h"
#include "main/depth.h"
#include "main/state.h"
#include "main/scissor.h"
#include "main/viewport.h"
#include "main/framebuffer.h"
#include "main/feedback.h"

#include "vbo/vbo.h"

#include "st_context.h"
#include "st_atom.h"
#include "st_cb_bufferobjects.h"
#include "st_draw.h"
#include "st_program.h"
#include "st_texture.h"
#include "st_cb_readpixels.h"

#include "pipe/p_context.h"
#include "pipe/p_defines.h"
#include "pipe/p_state.h"
#include "pipe/p_format.h"
#include "pipe/p_shader_tokens.h"
#include "util/u_inlines.h"
#include "util/u_simple_shaders.h"
#include "cso_cache/cso_context.h"
#include "tgsi/tgsi_ureg.h"

#include "draw/draw_private.h"
#include "draw/draw_context.h"

/**
 * Custom fragment shader that does nothing but ouput a constant color
 */
static void*
st_select_emul_create_fs(struct gl_context *ctx)
{
   struct st_context *st = st_context(ctx);
   struct pipe_context *pipe = st->pipe;
   struct ureg_program *ureg;
   struct ureg_dst out;
   struct ureg_src imm;

   ureg = ureg_create( TGSI_PROCESSOR_FRAGMENT );
   if (ureg == NULL)
      return NULL;

   out = ureg_DECL_output( ureg,
                           TGSI_SEMANTIC_COLOR,
                           0 );

   imm = ureg_imm4f( ureg, 0.0f, 0.0f, 0.0f, 1.0f );

   ureg_MOV( ureg, out, imm );
   ureg_END( ureg );

   return ureg_create_shader_and_destroy( ureg, pipe );
}

/**
 * Clears the depth values of our FBO by setting the
 * 1st pixel to 1.0f and the 2nd to 0.0f
 */
static void
st_select_emul_clear_fbo(struct gl_context *ctx)
{
   struct st_context *st = st_context(ctx);
   struct gl_scissor_attrib saved_scissor;
   struct gl_framebuffer *saved_fbo = NULL;
   GLfloat clearDepth;

   /* save states */
   saved_scissor = ctx->Scissor;
   if (ctx->DrawBuffer) {
      _mesa_reference_framebuffer(&saved_fbo, ctx->DrawBuffer);
   }
   clearDepth = ctx->Depth.Clear;

   /* hack needed because Clear does nothing if render mode != GL_RENDER */
   ctx->RenderMode = GL_RENDER;
   
   /* disable old draw fbo */
   if (saved_fbo) {
      /* XXX: needs flushing? */
      _mesa_check_end_texture_render(ctx, saved_fbo);
   }

   /* use our fbo */
   _mesa_check_begin_texture_render(ctx, st->select_emul.fbo);
   _mesa_reference_framebuffer(&ctx->DrawBuffer, st->select_emul.fbo);

   /* update fbo */
   _mesa_update_framebuffer(ctx);

   /* make sure context is up to date */
   _mesa_update_state(ctx);

   /* enable scissor */
   ctx->Scissor.Enabled = GL_TRUE;

   /* clear min-Z */
   ctx->Scissor.X = 0;
   ctx->Scissor.Y = 0;
   ctx->Scissor.Width = 1;
   ctx->Scissor.Height = 1;
   ctx->Depth.Clear = 1.0f;
   if (ctx->Driver.ClearDepth)
      (*ctx->Driver.ClearDepth)(ctx, ctx->Depth.Clear);
   ctx->NewState |= _NEW_SCISSOR | _NEW_DEPTH | _NEW_BUFFERS;
   _mesa_update_state(ctx);
   ctx->Driver.Clear(ctx, BUFFER_BIT_DEPTH);

   /* clear max-Z */
   ctx->Scissor.X = 1;
   ctx->Scissor.Y = 0;
   ctx->Scissor.Width = 1;
   ctx->Scissor.Height = 1;
   ctx->Depth.Clear = 0.0f;
   if (ctx->Driver.ClearDepth)
      (*ctx->Driver.ClearDepth)(ctx, ctx->Depth.Clear);
   ctx->NewState |= _NEW_SCISSOR | _NEW_DEPTH;
   _mesa_update_state(ctx);
   ctx->Driver.Clear(ctx, BUFFER_BIT_DEPTH);

   /* restore states */
   _mesa_check_end_texture_render(ctx, st->select_emul.fbo);
   if (saved_fbo) {
      _mesa_check_begin_texture_render(ctx, saved_fbo);
      _mesa_reference_framebuffer(&ctx->DrawBuffer, saved_fbo);
      _mesa_reference_framebuffer(&saved_fbo, NULL);
   }
   else
      _mesa_reference_framebuffer(&ctx->DrawBuffer, NULL);

   ctx->RenderMode = GL_SELECT;
   ctx->Depth.Clear = clearDepth;
   if (ctx->Driver.ClearDepth)
      (*ctx->Driver.ClearDepth)(ctx, ctx->Depth.Clear);
   ctx->Scissor = saved_scissor;
   ctx->NewState |= _NEW_SCISSOR | _NEW_DEPTH | _NEW_BUFFERS;
   _mesa_update_state(ctx);
}

/**
 * Clean up data
 */
static void
st_select_emul_destroy_fbo(struct gl_context *ctx)
{
   struct st_context *st = st_context(ctx);
   if (st->select_emul.rb_color_name) {
      _mesa_DeleteRenderbuffersEXT(1, &st->select_emul.rb_color_name);
      st->select_emul.rb_color_name = 0;
   }
   if (st->select_emul.rb_depth_name) {
      _mesa_DeleteRenderbuffersEXT(1, &st->select_emul.rb_depth_name);
      st->select_emul.rb_depth_name = 0;
   }
   if (st->select_emul.fbo) {
      _mesa_DeleteFramebuffersEXT(1, &st->select_emul.fbo->Name);
      _mesa_reference_framebuffer(&st->select_emul.fbo, NULL);
   }
   if (st->select_emul.fs) {
      cso_delete_fragment_shader(st->cso_context, st->select_emul.fs);
      st->select_emul.fs = NULL;
   }
}

/**
 * Create and setup FBO needed for our operations
 */
static bool
st_select_emul_create_fbo(struct gl_context *ctx)
{
   GLuint fboName;
   struct st_context *st = st_context(ctx);

   /* make sure we don't leak memory */
   st_select_emul_destroy_fbo(ctx);

   /* create buffer names */
   _mesa_GenFramebuffersEXT(1, &fboName);
   _mesa_GenRenderbuffersEXT(1, &st->select_emul.rb_depth_name);
   _mesa_GenRenderbuffersEXT(1, &st->select_emul.rb_color_name);

   /* allocate buffers' memory */
   _mesa_BindFramebufferEXT(GL_FRAMEBUFFER, fboName);
   _mesa_BindRenderbufferEXT(GL_RENDERBUFFER, st->select_emul.rb_depth_name);
   _mesa_RenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 2, 1);
   _mesa_BindRenderbufferEXT(GL_RENDERBUFFER, st->select_emul.rb_color_name);
   _mesa_RenderbufferStorageEXT(GL_RENDERBUFFER, GL_RGBA, 2, 1);
   _mesa_BindRenderbufferEXT(GL_RENDERBUFFER, 0);

   /* setup fbo */
   _mesa_FramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                    GL_RENDERBUFFER,
                                    st->select_emul.rb_depth_name);
   _mesa_FramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                    GL_RENDERBUFFER,
                                    st->select_emul.rb_color_name);

   if (_mesa_CheckFramebufferStatusEXT(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      st_select_emul_destroy_fbo(ctx);
      return false;
   }

   _mesa_BindFramebufferEXT(GL_FRAMEBUFFER, 0);

   /* get fbo pointer */
   st->select_emul.fbo = _mesa_lookup_framebuffer(ctx, fboName);

   /* create fragment shader */
   st->select_emul.fs = st_select_emul_create_fs(ctx);

   /* check fbo pointer validity */
   if (!st->select_emul.fbo) {
      st_select_emul_destroy_fbo(ctx);
      return false;
   }

   return true;
}

/**
 * Called when entering GL_SELECT mode.
 * This function initializes our private data and clears
 * depth so that the next draw call will have it ready.
 */
bool
st_select_emul_begin(struct gl_context *ctx)
{
   struct st_context *st = st_context(ctx);

   /* perform an hw caps check once */
   static bool hw_checked = false;
   if (!hw_checked) {
      hw_checked = true;
      /* XXX: is this a proper way to check for hw support? */
      if (!ctx->Driver.ReadPixels ||
          !ctx->Driver.DrawPixels ||
          !ctx->Driver.NewFramebuffer ||
          !ctx->Driver.NewRenderbuffer) {
         st->select_emul.hw_unsupported = true;
         return false;
      }
   }

   /* initialize fbo if not yet initialized */
   if (!st->select_emul.fbo) {
      if (!st_select_emul_create_fbo(ctx)) {
         return false;
      }
   }
   
   /* clear FBO */
   st_select_emul_clear_fbo(ctx);

   return true;
}

void
st_select_emul_end(struct gl_context *ctx)
{
   st_select_emul_destroy_fbo(ctx);
}

/**
 * Reads into our FBO and emit z-buffer results
 * called by st_select_draw_func()
 */
static void
st_select_emul_update_hits(struct gl_context *ctx)
{
   struct st_context *st = st_context(ctx);
   struct gl_framebuffer *saved_fbo = NULL;
   float zData[2];

   /* set state */
   if (ctx->ReadBuffer) {
      _mesa_reference_framebuffer(&saved_fbo, ctx->ReadBuffer);
      _mesa_check_end_texture_render(ctx, saved_fbo);
   }
   _mesa_check_begin_texture_render(ctx, st->select_emul.fbo);
   _mesa_reference_framebuffer(&ctx->ReadBuffer, st->select_emul.fbo);

   /* update fbo */
   _mesa_update_framebuffer( ctx );
  
   /* update state */
   ctx->NewState |= _NEW_BUFFERS;
   _mesa_update_state( ctx );

   /* read pixels */
   ctx->Driver.ReadPixels( ctx, 0, 0, 2, 1,
                           GL_DEPTH_COMPONENT, GL_FLOAT,
                           &ctx->Pack, (void*)zData );

   /* emit z-buffer results */
   if (zData[0] != 1.0f)
      _mesa_update_hitflag( ctx, zData[0] );
   if (zData[1] != 0.0f)
      _mesa_update_hitflag( ctx, zData[1] );

   /* clear FBO */
   st_select_emul_clear_fbo(ctx);

   /* restore state */
   _mesa_check_end_texture_render(ctx, st->select_emul.fbo);
   if (saved_fbo) {
      _mesa_check_begin_texture_render(ctx, saved_fbo);
      _mesa_reference_framebuffer(&ctx->ReadBuffer, saved_fbo);
      _mesa_reference_framebuffer(&saved_fbo, NULL);
   }
   else
      _mesa_reference_framebuffer(&ctx->ReadBuffer, NULL);

   /* update state */
   ctx->NewState |= _NEW_BUFFERS;
   _mesa_update_state( ctx );
}

/**
 * Plug in draw function to draw in GL_SELECT mode.
 * This function does the following steps:
 *   1) render into the 1st pixel of our FBO with depth state
 *      configured so that we get the lowest Z values
 *   2) render into the 2nd pixel of our FBO with depth state
 *      configured so that we get the highest Z values
 *   3) calls st_select_emul_update_hits() to emit results
 */
void
st_select_draw_func(struct gl_context *ctx,
                    const struct gl_client_array **arrays,
                    const struct _mesa_prim *prims,
                    GLuint nr_prims,
                    const struct _mesa_index_buffer *ib,
                    GLboolean index_bounds_valid,
                    GLuint min_index,
                    GLuint max_index)
{
   GLboolean saved_scissorEnabled;
   struct gl_framebuffer *saved_fbo = NULL;
   struct st_context *st = st_context(ctx);
   struct pipe_depth_stencil_alpha_state state_depth;
   struct pipe_viewport_state state_viewport;

   /* save states */
   if (ctx->DrawBuffer)
      _mesa_reference_framebuffer(&saved_fbo, ctx->DrawBuffer);

   saved_scissorEnabled = ctx->Scissor.Enabled;
   cso_save_fragment_shader(st->cso_context);
   cso_save_depth_stencil_alpha(st->cso_context);
   cso_save_viewport(st->cso_context);
   
   /* prepare stencil state */
   memset(&state_depth, 0, sizeof(state_depth));
   state_depth.depth.enabled = 1;
   state_depth.depth.writemask = 1;

   /* prepare viewport state */
   memset(&state_viewport, 0, sizeof(state_viewport));
   state_viewport.scale[0] = 0.5f * 2.0f * 0.5;
   state_viewport.scale[1] = 0.5f * 1.0f;
   state_viewport.scale[2] = 1.0f;
   state_viewport.scale[3] = 1.0f;
   state_viewport.translate[1] = 0.5f * 1.0f;
   state_viewport.translate[2] = 0.0f;
   state_viewport.translate[3] = 0.0f;

   /* use our custom fragment shader */
   cso_set_fragment_shader_handle(st->cso_context, st->select_emul.fs);

   /* disable scissor */
   ctx->Scissor.Enabled = GL_FALSE;

   /* disable old draw fbo */
   if (saved_fbo)
      _mesa_check_end_texture_render(ctx, saved_fbo);

   /* use our fbo */
   _mesa_check_begin_texture_render(ctx, st->select_emul.fbo);
   _mesa_reference_framebuffer(&ctx->DrawBuffer, st->select_emul.fbo);

   /* update fbo */
   _mesa_update_framebuffer( ctx );
   
   /* update context */
   _mesa_update_state(ctx);

   /* render min-z */
   {
      /* specific states */
      state_depth.depth.func = PIPE_FUNC_LESS;
      state_viewport.translate[0] = 0.5f * 1.0f + 0.0f;

      cso_set_depth_stencil_alpha(st->cso_context, &state_depth);
      cso_set_viewport(st->cso_context, &state_viewport);

      /* draw */
      st_draw_vbo(ctx, arrays, prims, nr_prims, ib, index_bounds_valid, min_index, max_index);
   }

   /* render max-z */
   {
      state_depth.depth.func = PIPE_FUNC_GREATER;
      state_viewport.translate[0] = 0.5f * 1.0f + 1.0f;

      cso_set_depth_stencil_alpha(st->cso_context, &state_depth);
      cso_set_viewport(st->cso_context, &state_viewport);

      /* draw */
      st_draw_vbo(ctx, arrays, prims, nr_prims, ib, index_bounds_valid, min_index, max_index);
   }

   /* XXX: needs flushing? */

   /* restore states */
   cso_restore_fragment_shader(st->cso_context);
   cso_restore_depth_stencil_alpha(st->cso_context);
   cso_restore_viewport(st->cso_context);
   ctx->Scissor.Enabled = saved_scissorEnabled;
   _mesa_check_end_texture_render(ctx, st->select_emul.fbo);
   if (saved_fbo) {
      _mesa_check_begin_texture_render(ctx, saved_fbo);
      _mesa_reference_framebuffer(&ctx->DrawBuffer, saved_fbo);
      _mesa_reference_framebuffer(&saved_fbo, NULL);
   }
   else
      _mesa_reference_framebuffer(&ctx->DrawBuffer, NULL);
   _mesa_update_state(ctx);

   /* update hits */
   st_select_emul_update_hits(ctx);
}

