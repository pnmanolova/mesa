/*
 * Mesa 3-D graphics library
 *
 * Copyright (C) 1999-2008  Brian Paul   All Rights Reserved.
 * Copyright (c) 2009  VMware, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * Texture buffer/memory mapping and texel fetch/store functions
 */


#include "main/colormac.h"
#include "main/macros.h"
#include "main/texcompress.h"
#include "main/texcompress_fxt1.h"
#include "main/texcompress_s3tc.h"
#include "main/texcompress_rgtc.h"
#include "main/teximage.h"
#include "swrast/s_context.h"
#include "../../gallium/auxiliary/util/u_format_rgb9e5.h"
#include "../../gallium/auxiliary/util/u_format_r11g11b10f.h"


/**
 * Convert an 8-bit sRGB value from non-linear space to a
 * linear RGB value in [0, 1].
 * Implemented with a 256-entry lookup table.
 */
static INLINE GLfloat
nonlinear_to_linear(GLubyte cs8)
{
   static GLfloat table[256];
   static GLboolean tableReady = GL_FALSE;
   if (!tableReady) {
      /* compute lookup table now */
      GLuint i;
      for (i = 0; i < 256; i++) {
         const GLfloat cs = UBYTE_TO_FLOAT(i);
         if (cs <= 0.04045) {
            table[i] = cs / 12.92f;
         }
         else {
            table[i] = (GLfloat) pow((cs + 0.055) / 1.055, 2.4);
         }
      }
      tableReady = GL_TRUE;
   }
   return table[cs8];
}


/**
 * Null texel fetch function.
 *
 * Have to have this so the FetchTexel function pointer is never NULL.
 */
static void
fetch_null_texel(const struct swrast_texture_image *texImage,
                 GLint i, GLint j, GLint k, GLfloat * texel)
{
   texel[RCOMP] = 0.0;
   texel[GCOMP] = 0.0;
   texel[BCOMP] = 0.0;
   texel[ACOMP] = 0.0;
   _mesa_warning(NULL, "fetch_null_texelf() called!");
}

static void
store_null_texel(struct swrast_texture_image *texImage,
                 GLint i, GLint j, GLint k, const void *texel)
{
   /* no-op */
}


#define TEXEL_ADDR(type, texImage, i, j, k, size) \
   ((type *)(texImage->SliceMaps[k] + \
             texImage->RowStride * (j) + \
             texImage->TexelSize * (i)))


static void
fetch_texel_z32(const struct swrast_texture_image *texImage,
                GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLuint *src = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   texel[0] = src[0] * (1.0F / 0xffffffff);
}

static void
store_texel_z32(struct swrast_texture_image *texImage,
                GLint i, GLint j, GLint k, const void *texel)
{
   const GLuint *depth = (const GLuint *) texel;
   GLuint *dst = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   dst[0] = *depth;
}



static void
fetch_texel_z16(const struct swrast_texture_image *texImage,
                GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLushort *src = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   texel[0] = src[0] * (1.0F / 65535.0F);
}

static void
store_texel_z16(struct swrast_texture_image *texImage,
                GLint i, GLint j, GLint k, const void *texel)
{
   const GLushort *depth = (const GLushort *) texel;
   GLushort *dst = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   dst[0] = *depth;
}



static void
fetch_texel_rgba_f32(const struct swrast_texture_image *texImage,
                     GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLfloat *src = TEXEL_ADDR(GLfloat, texImage, i, j, k, 4);
   texel[RCOMP] = src[0];
   texel[GCOMP] = src[1];
   texel[BCOMP] = src[2];
   texel[ACOMP] = src[3];
}

static void
store_texel_rgba_f32(struct swrast_texture_image *texImage,
                     GLint i, GLint j, GLint k, const void *texel)
{
   const GLfloat *depth = (const GLfloat *) texel;
   GLfloat *dst = TEXEL_ADDR(GLfloat, texImage, i, j, k, 4);
   dst[0] = depth[RCOMP];
   dst[1] = depth[GCOMP];
   dst[2] = depth[BCOMP];
   dst[3] = depth[ACOMP];
}



static void
fetch_texel_rgba_f16(const struct swrast_texture_image *texImage,
                     GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLhalfARB *src = TEXEL_ADDR(GLhalfARB, texImage, i, j, k, 4);
   texel[RCOMP] = _mesa_half_to_float(src[0]);
   texel[GCOMP] = _mesa_half_to_float(src[1]);
   texel[BCOMP] = _mesa_half_to_float(src[2]);
   texel[ACOMP] = _mesa_half_to_float(src[3]);
}

static void
store_texel_rgba_f16(struct swrast_texture_image *texImage,
                     GLint i, GLint j, GLint k, const void *texel)
{
   const GLfloat *src = (const GLfloat *) texel;
   GLhalfARB *dst = TEXEL_ADDR(GLhalfARB, texImage, i, j, k, 4);
   dst[0] = _mesa_float_to_half(src[RCOMP]);
   dst[1] = _mesa_float_to_half(src[GCOMP]);
   dst[2] = _mesa_float_to_half(src[BCOMP]);
   dst[3] = _mesa_float_to_half(src[ACOMP]);
}



static void
fetch_texel_rgb_f32(const struct swrast_texture_image *texImage,
                    GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLfloat *src = TEXEL_ADDR(GLfloat, texImage, i, j, k, 3);
   texel[RCOMP] = src[0];
   texel[GCOMP] = src[1];
   texel[BCOMP] = src[2];
   texel[ACOMP] = 1.0F;
}

static void
store_texel_rgb_f32(struct swrast_texture_image *texImage,
                    GLint i, GLint j, GLint k, const void *texel)
{
   const GLfloat *src = (const GLfloat *) texel;
   GLfloat *dst = TEXEL_ADDR(GLfloat, texImage, i, j, k, 3);
   dst[0] = src[RCOMP];
   dst[1] = src[GCOMP];
   dst[2] = src[BCOMP];
}



static void
fetch_texel_rgb_f16(const struct swrast_texture_image *texImage,
                    GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLhalfARB *src = TEXEL_ADDR(GLhalfARB, texImage, i, j, k, 3);
   texel[RCOMP] = _mesa_half_to_float(src[0]);
   texel[GCOMP] = _mesa_half_to_float(src[1]);
   texel[BCOMP] = _mesa_half_to_float(src[2]);
   texel[ACOMP] = 1.0F;
}


static void
store_texel_rgb_f16(struct swrast_texture_image *texImage,
                    GLint i, GLint j, GLint k, const void *texel)
{
   const GLfloat *src = (const GLfloat *) texel;
   GLhalfARB *dst = TEXEL_ADDR(GLhalfARB, texImage, i, j, k, 3);
   dst[0] = _mesa_float_to_half(src[RCOMP]);
   dst[1] = _mesa_float_to_half(src[GCOMP]);
   dst[2] = _mesa_float_to_half(src[BCOMP]);
}



static void
fetch_texel_alpha_f32(const struct swrast_texture_image *texImage,
                      GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLfloat *src = TEXEL_ADDR(GLfloat, texImage, i, j, k, 1);
   texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = 0.0F;
   texel[ACOMP] = src[0];
}


static void
store_texel_alpha_f32(struct swrast_texture_image *texImage,
                      GLint i, GLint j, GLint k, const void *texel)
{
   const GLfloat *rgba = (const GLfloat *) texel;
   GLfloat *dst = TEXEL_ADDR(GLfloat, texImage, i, j, k, 1);
   dst[0] = rgba[ACOMP];
}



static void
fetch_texel_alpha_f16(const struct swrast_texture_image *texImage,
                      GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLhalfARB *src = TEXEL_ADDR(GLhalfARB, texImage, i, j, k, 1);
   texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = 0.0F;
   texel[ACOMP] = _mesa_half_to_float(src[0]);
}


static void
store_texel_alpha_f16(struct swrast_texture_image *texImage,
                      GLint i, GLint j, GLint k, const void *texel)
{
   const GLfloat *rgba = (const GLfloat *) texel;
   GLhalfARB *dst = TEXEL_ADDR(GLhalfARB, texImage, i, j, k, 1);
   dst[0] = _mesa_float_to_half(rgba[ACOMP]);
}



static void
fetch_texel_luminance_f32(const struct swrast_texture_image *texImage,
                          GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLfloat *src = TEXEL_ADDR(GLfloat, texImage, i, j, k, 1);
   texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = src[0];
   texel[ACOMP] = 1.0F;
}


static void
store_texel_luminance_f32(struct swrast_texture_image *texImage,
                          GLint i, GLint j, GLint k, const void *texel)
{
   const GLfloat *rgba = (const GLfloat *) texel;
   GLfloat *dst = TEXEL_ADDR(GLfloat, texImage, i, j, k, 1);
   dst[0] = rgba[RCOMP];
}



static void
fetch_texel_luminance_f16(const struct swrast_texture_image *texImage,
                          GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLhalfARB *src = TEXEL_ADDR(GLhalfARB, texImage, i, j, k, 1);
   texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = _mesa_half_to_float(src[0]);
   texel[ACOMP] = 1.0F;
}

static void
store_texel_luminance_f16(struct swrast_texture_image *texImage,
                          GLint i, GLint j, GLint k, const void *texel)
{
   const GLfloat *rgba = (const GLfloat *) texel;
   GLhalfARB *dst = TEXEL_ADDR(GLhalfARB, texImage, i, j, k, 1);
   dst[0] = _mesa_float_to_half(rgba[RCOMP]);
}



static void
fetch_texel_luminance_alpha_f32(const struct swrast_texture_image *texImage,
                                GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLfloat *src = TEXEL_ADDR(GLfloat, texImage, i, j, k, 2);
   texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = src[0];
   texel[ACOMP] = src[1];
}

static void
store_texel_luminance_alpha_f32(struct swrast_texture_image *texImage,
                                GLint i, GLint j, GLint k, const void *texel)
{
   const GLfloat *rgba = (const GLfloat *) texel;
   GLfloat *dst = TEXEL_ADDR(GLfloat, texImage, i, j, k, 2);
   dst[0] = rgba[RCOMP];
   dst[1] = rgba[ACOMP];
}



static void
fetch_texel_luminance_alpha_f16(const struct swrast_texture_image *texImage,
                                GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLhalfARB *src = TEXEL_ADDR(GLhalfARB, texImage, i, j, k, 2);
   texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = _mesa_half_to_float(src[0]);
   texel[ACOMP] = _mesa_half_to_float(src[1]);
}

static void
store_texel_luminance_alpha_f16(struct swrast_texture_image *texImage,
                                GLint i, GLint j, GLint k, const void *texel)
{
   const GLfloat *rgba = (const GLfloat *) texel;
   GLhalfARB *dst = TEXEL_ADDR(GLhalfARB, texImage, i, j, k, 2);
   dst[0] = _mesa_float_to_half(rgba[RCOMP]);
   dst[1] = _mesa_float_to_half(rgba[ACOMP]);
}



static void
fetch_texel_intensity_f32(const struct swrast_texture_image *texImage,
                          GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLfloat *src = TEXEL_ADDR(GLfloat, texImage, i, j, k, 1);
   texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = texel[ACOMP] = src[0];
}

static void
store_texel_intensity_f32(struct swrast_texture_image *texImage,
                          GLint i, GLint j, GLint k, const void *texel)
{
   const GLfloat *rgba = (const GLfloat *) texel;
   GLfloat *dst = TEXEL_ADDR(GLfloat, texImage, i, j, k, 1);
   dst[0] = rgba[RCOMP];
}



static void
fetch_texel_intensity_f16(const struct swrast_texture_image *texImage,
                          GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLhalfARB *src = TEXEL_ADDR(GLhalfARB, texImage, i, j, k, 1);
   texel[RCOMP] =
      texel[GCOMP] =
      texel[BCOMP] = texel[ACOMP] = _mesa_half_to_float(src[0]);
}

static void
store_texel_intensity_f16(struct swrast_texture_image *texImage,
                          GLint i, GLint j, GLint k, const void *texel)
{
   const GLfloat *rgba = (const GLfloat *) texel;
   GLhalfARB *dst = TEXEL_ADDR(GLhalfARB, texImage, i, j, k, 1);
   dst[0] = _mesa_float_to_half(rgba[RCOMP]);
}



static void
fetch_texel_r_f32(const struct swrast_texture_image *texImage,
                  GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLfloat *src = TEXEL_ADDR(GLfloat, texImage, i, j, k, 1);
   texel[RCOMP] = src[0];
   texel[GCOMP] = 0.0F;
   texel[BCOMP] = 0.0F;
   texel[ACOMP] = 1.0F;
}

static void
store_texel_r_f32(struct swrast_texture_image *texImage,
                  GLint i, GLint j, GLint k, const void *texel)
{
   const GLfloat *rgba = (const GLfloat *) texel;
   GLfloat *dst = TEXEL_ADDR(GLfloat, texImage, i, j, k, 1);
   dst[0] = rgba[RCOMP];
}



static void
fetch_texel_r_f16(const struct swrast_texture_image *texImage,
                  GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLhalfARB *src = TEXEL_ADDR(GLhalfARB, texImage, i, j, k, 1);
   texel[RCOMP] = _mesa_half_to_float(src[0]);
   texel[GCOMP] = 0.0F;
   texel[BCOMP] = 0.0F;
   texel[ACOMP] = 1.0F;
}

static void
store_texel_r_f16(struct swrast_texture_image *texImage,
                  GLint i, GLint j, GLint k, const void *texel)
{
   const GLfloat *rgba = (const GLfloat *) texel;
   GLhalfARB *dst = TEXEL_ADDR(GLhalfARB, texImage, i, j, k, 1);
   dst[0] = _mesa_float_to_half(rgba[RCOMP]);
}



static void
fetch_texel_rg_f32(const struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLfloat *src = TEXEL_ADDR(GLfloat, texImage, i, j, k, 2);
   texel[RCOMP] = src[0];
   texel[GCOMP] = src[1];
   texel[BCOMP] = 0.0F;
   texel[ACOMP] = 1.0F;
}

static void
store_texel_rg_f32(struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, const void *texel)
{
   const GLfloat *rgba = (const GLfloat *) texel;
   GLfloat *dst = TEXEL_ADDR(GLfloat, texImage, i, j, k, 2);
   dst[0] = rgba[RCOMP];
   dst[1] = rgba[GCOMP];
}



static void
fetch_texel_rg_f16(const struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLhalfARB *src = TEXEL_ADDR(GLhalfARB, texImage, i, j, k, 2);
   texel[RCOMP] = _mesa_half_to_float(src[0]);
   texel[GCOMP] = _mesa_half_to_float(src[1]);
   texel[BCOMP] = 0.0F;
   texel[ACOMP] = 1.0F;
}

static void
store_texel_rg_f16(struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, const void *texel)
{
   const GLfloat *rgba = (const GLfloat *) texel;
   GLhalfARB *dst = TEXEL_ADDR(GLhalfARB, texImage, i, j, k, 2);
   dst[0] = _mesa_float_to_half(rgba[RCOMP]);
   dst[1] = _mesa_float_to_half(rgba[GCOMP]);
}



static void
fetch_texel_rgba8888(const struct swrast_texture_image *texImage,
                     GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLuint s = *TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   texel[RCOMP] = UBYTE_TO_FLOAT((s >> 24));
   texel[GCOMP] = UBYTE_TO_FLOAT((s >> 16) & 0xff);
   texel[BCOMP] = UBYTE_TO_FLOAT((s >> 8) & 0xff);
   texel[ACOMP] = UBYTE_TO_FLOAT((s) & 0xff);
}

static void
store_texel_rgba8888(struct swrast_texture_image *texImage,
                     GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLuint *dst = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   *dst = PACK_COLOR_8888(rgba[RCOMP], rgba[GCOMP], rgba[BCOMP], rgba[ACOMP]);
}



static void
fetch_texel_rgba8888_rev(const struct swrast_texture_image *texImage,
                         GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLuint s = *TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   texel[RCOMP] = UBYTE_TO_FLOAT((s) & 0xff);
   texel[GCOMP] = UBYTE_TO_FLOAT((s >> 8) & 0xff);
   texel[BCOMP] = UBYTE_TO_FLOAT((s >> 16) & 0xff);
   texel[ACOMP] = UBYTE_TO_FLOAT((s >> 24));
}

static void
store_texel_rgba8888_rev(struct swrast_texture_image *texImage,
                         GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLuint *dst = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   *dst =
      PACK_COLOR_8888_REV(rgba[RCOMP], rgba[GCOMP], rgba[BCOMP], rgba[ACOMP]);
}



static void
fetch_texel_argb8888(const struct swrast_texture_image *texImage,
                     GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLuint s = *TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   texel[RCOMP] = UBYTE_TO_FLOAT((s >> 16) & 0xff);
   texel[GCOMP] = UBYTE_TO_FLOAT((s >> 8) & 0xff);
   texel[BCOMP] = UBYTE_TO_FLOAT((s) & 0xff);
   texel[ACOMP] = UBYTE_TO_FLOAT((s >> 24));
}

static void
store_texel_argb8888(struct swrast_texture_image *texImage,
                     GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLuint *dst = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   *dst = PACK_COLOR_8888(rgba[ACOMP], rgba[RCOMP], rgba[GCOMP], rgba[BCOMP]);
}



static void
fetch_texel_argb8888_rev(const struct swrast_texture_image *texImage,
                         GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLuint s = *TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   texel[RCOMP] = UBYTE_TO_FLOAT((s >> 8) & 0xff);
   texel[GCOMP] = UBYTE_TO_FLOAT((s >> 16) & 0xff);
   texel[BCOMP] = UBYTE_TO_FLOAT((s >> 24));
   texel[ACOMP] = UBYTE_TO_FLOAT((s) & 0xff);
}

static void
store_texel_argb8888_rev(struct swrast_texture_image *texImage,
                         GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLuint *dst = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   *dst = PACK_COLOR_8888(rgba[BCOMP], rgba[GCOMP], rgba[RCOMP], rgba[ACOMP]);
}



static void
fetch_texel_xrgb8888(const struct swrast_texture_image *texImage,
                     GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLuint s = *TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   texel[RCOMP] = UBYTE_TO_FLOAT((s >> 16) & 0xff);
   texel[GCOMP] = UBYTE_TO_FLOAT((s >> 8) & 0xff);
   texel[BCOMP] = UBYTE_TO_FLOAT((s) & 0xff);
   texel[ACOMP] = 1.0f;
}

static void
store_texel_xrgb8888(struct swrast_texture_image *texImage,
                     GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLuint *dst = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   *dst = PACK_COLOR_8888(0xff, rgba[RCOMP], rgba[GCOMP], rgba[BCOMP]);
}



static void
fetch_texel_xrgb8888_rev(const struct swrast_texture_image *texImage,
                         GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLuint s = *TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   texel[RCOMP] = UBYTE_TO_FLOAT((s >> 8) & 0xff);
   texel[GCOMP] = UBYTE_TO_FLOAT((s >> 16) & 0xff);
   texel[BCOMP] = UBYTE_TO_FLOAT((s >> 24));
   texel[ACOMP] = 1.0f;
}

static void
store_texel_xrgb8888_rev(struct swrast_texture_image *texImage,
                         GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLuint *dst = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   *dst = PACK_COLOR_8888(rgba[BCOMP], rgba[GCOMP], rgba[RCOMP], 0xff);
}



static void
fetch_texel_rgb888(const struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLubyte *src = TEXEL_ADDR(GLubyte, texImage, i, j, k, 3);
   texel[RCOMP] = UBYTE_TO_FLOAT(src[2]);
   texel[GCOMP] = UBYTE_TO_FLOAT(src[1]);
   texel[BCOMP] = UBYTE_TO_FLOAT(src[0]);
   texel[ACOMP] = 1.0F;
}

static void
store_texel_rgb888(struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLubyte *dst = TEXEL_ADDR(GLubyte, texImage, i, j, k, 3);
   dst[0] = rgba[BCOMP];
   dst[1] = rgba[GCOMP];
   dst[2] = rgba[RCOMP];
}



static void
fetch_texel_bgr888(const struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLubyte *src = TEXEL_ADDR(GLubyte, texImage, i, j, k, 3);
   texel[RCOMP] = UBYTE_TO_FLOAT(src[0]);
   texel[GCOMP] = UBYTE_TO_FLOAT(src[1]);
   texel[BCOMP] = UBYTE_TO_FLOAT(src[2]);
   texel[ACOMP] = 1.0F;
}

static void
store_texel_bgr888(struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLubyte *dst = TEXEL_ADDR(GLubyte, texImage, i, j, k, 3);
   dst[0] = rgba[RCOMP];
   dst[1] = rgba[GCOMP];
   dst[2] = rgba[BCOMP];
}



/* use color expansion like (g << 2) | (g >> 4) (does somewhat random rounding)
 * instead of slow (g << 2) * 255 / 252 (always rounds down)
 */

static void
fetch_texel_rgb565(const struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLushort *src = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   const GLushort s = *src;
   texel[RCOMP] = ((s >> 11) & 0x1f) * (1.0F / 31.0F);
   texel[GCOMP] = ((s >> 5) & 0x3f) * (1.0F / 63.0F);
   texel[BCOMP] = ((s) & 0x1f) * (1.0F / 31.0F);
   texel[ACOMP] = 1.0F;
}

static void
store_texel_rgb565(struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLushort *dst = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   *dst = PACK_COLOR_565(rgba[RCOMP], rgba[GCOMP], rgba[BCOMP]);
}



static void
fetch_texel_rgb565_rev(const struct swrast_texture_image *texImage,
                       GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLushort *src = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   const GLushort s = (*src >> 8) | (*src << 8);        /* byte swap */
   texel[RCOMP] = UBYTE_TO_FLOAT(((s >> 8) & 0xf8) | ((s >> 13) & 0x7));
   texel[GCOMP] = UBYTE_TO_FLOAT(((s >> 3) & 0xfc) | ((s >> 9) & 0x3));
   texel[BCOMP] = UBYTE_TO_FLOAT(((s << 3) & 0xf8) | ((s >> 2) & 0x7));
   texel[ACOMP] = 1.0F;
}

static void
store_texel_rgb565_rev(struct swrast_texture_image *texImage,
                       GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLushort *dst = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   *dst = PACK_COLOR_565(rgba[BCOMP], rgba[GCOMP], rgba[RCOMP]);
}



static void
fetch_texel_argb4444(const struct swrast_texture_image *texImage,
                     GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLushort *src = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   const GLushort s = *src;
   texel[RCOMP] = ((s >> 8) & 0xf) * (1.0F / 15.0F);
   texel[GCOMP] = ((s >> 4) & 0xf) * (1.0F / 15.0F);
   texel[BCOMP] = ((s) & 0xf) * (1.0F / 15.0F);
   texel[ACOMP] = ((s >> 12) & 0xf) * (1.0F / 15.0F);
}

static void
store_texel_argb4444(struct swrast_texture_image *texImage,
                     GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLushort *dst = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   *dst = PACK_COLOR_4444(rgba[ACOMP], rgba[RCOMP], rgba[GCOMP], rgba[BCOMP]);
}



static void
fetch_texel_argb4444_rev(const struct swrast_texture_image *texImage,
                         GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLushort s = *TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   texel[RCOMP] = ((s) & 0xf) * (1.0F / 15.0F);
   texel[GCOMP] = ((s >> 12) & 0xf) * (1.0F / 15.0F);
   texel[BCOMP] = ((s >> 8) & 0xf) * (1.0F / 15.0F);
   texel[ACOMP] = ((s >> 4) & 0xf) * (1.0F / 15.0F);
}

static void
store_texel_argb4444_rev(struct swrast_texture_image *texImage,
                         GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLushort *dst = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   *dst = PACK_COLOR_4444(rgba[ACOMP], rgba[BCOMP], rgba[GCOMP], rgba[RCOMP]);
}


static void
fetch_texel_rgba5551(const struct swrast_texture_image *texImage,
                     GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLushort *src = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   const GLushort s = *src;
   texel[RCOMP] = ((s >> 11) & 0x1f) * (1.0F / 31.0F);
   texel[GCOMP] = ((s >> 6) & 0x1f) * (1.0F / 31.0F);
   texel[BCOMP] = ((s >> 1) & 0x1f) * (1.0F / 31.0F);
   texel[ACOMP] = ((s) & 0x01) * 1.0F;
}

static void
store_texel_rgba5551(struct swrast_texture_image *texImage,
                     GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLushort *dst = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   *dst = PACK_COLOR_5551(rgba[RCOMP], rgba[GCOMP], rgba[BCOMP], rgba[ACOMP]);
}



static void
fetch_texel_argb1555(const struct swrast_texture_image *texImage,
                     GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLushort *src = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   const GLushort s = *src;
   texel[RCOMP] = ((s >> 10) & 0x1f) * (1.0F / 31.0F);
   texel[GCOMP] = ((s >> 5) & 0x1f) * (1.0F / 31.0F);
   texel[BCOMP] = ((s >> 0) & 0x1f) * (1.0F / 31.0F);
   texel[ACOMP] = ((s >> 15) & 0x01) * 1.0F;
}

static void
store_texel_argb1555(struct swrast_texture_image *texImage,
                     GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLushort *dst = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   *dst = PACK_COLOR_1555(rgba[ACOMP], rgba[RCOMP], rgba[GCOMP], rgba[BCOMP]);
}



static void
fetch_texel_argb1555_rev(const struct swrast_texture_image *texImage,
                         GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLushort *src = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   const GLushort s = (*src << 8) | (*src >> 8);        /* byteswap */
   texel[RCOMP] = UBYTE_TO_FLOAT(((s >> 7) & 0xf8) | ((s >> 12) & 0x7));
   texel[GCOMP] = UBYTE_TO_FLOAT(((s >> 2) & 0xf8) | ((s >> 7) & 0x7));
   texel[BCOMP] = UBYTE_TO_FLOAT(((s << 3) & 0xf8) | ((s >> 2) & 0x7));
   texel[ACOMP] = UBYTE_TO_FLOAT(((s >> 15) & 0x01) * 255);
}

static void
store_texel_argb1555_rev(struct swrast_texture_image *texImage,
                         GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLushort *dst = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   *dst =
      PACK_COLOR_1555_REV(rgba[ACOMP], rgba[RCOMP], rgba[GCOMP], rgba[BCOMP]);
}



static void
fetch_texel_argb2101010(const struct swrast_texture_image *texImage,
                        GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLuint *src = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   const GLuint s = *src;
   texel[RCOMP] = ((s >> 20) & 0x3ff) * (1.0F / 1023.0F);
   texel[GCOMP] = ((s >> 10) & 0x3ff) * (1.0F / 1023.0F);
   texel[BCOMP] = ((s >> 0) & 0x3ff) * (1.0F / 1023.0F);
   texel[ACOMP] = ((s >> 30) & 0x03) * (1.0F / 3.0F);
}

static void
store_texel_argb2101010(struct swrast_texture_image *texImage,
                        GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLuint *dst = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   *dst =
      PACK_COLOR_2101010_UB(rgba[ACOMP], rgba[RCOMP], rgba[GCOMP],
                            rgba[BCOMP]);
}



static void
fetch_texel_rg88(const struct swrast_texture_image *texImage,
                 GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLushort s = *TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   texel[RCOMP] = UBYTE_TO_FLOAT(s & 0xff);
   texel[GCOMP] = UBYTE_TO_FLOAT(s >> 8);
   texel[BCOMP] = 0.0;
   texel[ACOMP] = 1.0;
}

static void
store_texel_rg88(struct swrast_texture_image *texImage,
                 GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLushort *dst = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   *dst = PACK_COLOR_88(rgba[RCOMP], rgba[GCOMP]);
}



static void
fetch_texel_rg88_rev(const struct swrast_texture_image *texImage,
                     GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLushort s = *TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   texel[RCOMP] = UBYTE_TO_FLOAT(s & 0xff);
   texel[GCOMP] = UBYTE_TO_FLOAT(s >> 8);
   texel[BCOMP] = 0.0;
   texel[ACOMP] = 1.0;
}

static void
store_texel_rg88_rev(struct swrast_texture_image *texImage,
                     GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLushort *dst = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   *dst = PACK_COLOR_88(rgba[GCOMP], rgba[RCOMP]);
}



static void
fetch_texel_al44(const struct swrast_texture_image *texImage,
                 GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLubyte s = *TEXEL_ADDR(GLubyte, texImage, i, j, k, 1);
   texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = (s & 0xf) * (1.0F / 15.0F);
   texel[ACOMP] = ((s >> 4) & 0xf) * (1.0F / 15.0F);
}

static void
store_texel_al44(struct swrast_texture_image *texImage,
                 GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLubyte *dst = TEXEL_ADDR(GLubyte, texImage, i, j, k, 1);
   *dst = PACK_COLOR_44(rgba[ACOMP], rgba[RCOMP]);
}



static void
fetch_texel_al88(const struct swrast_texture_image *texImage,
                 GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLushort s = *TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = UBYTE_TO_FLOAT(s & 0xff);
   texel[ACOMP] = UBYTE_TO_FLOAT(s >> 8);
}

static void
store_texel_al88(struct swrast_texture_image *texImage,
                 GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLushort *dst = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   *dst = PACK_COLOR_88(rgba[ACOMP], rgba[RCOMP]);
}



static void
fetch_texel_r8(const struct swrast_texture_image *texImage,
               GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLubyte s = *TEXEL_ADDR(GLubyte, texImage, i, j, k, 1);
   texel[RCOMP] = UBYTE_TO_FLOAT(s);
   texel[GCOMP] = 0.0;
   texel[BCOMP] = 0.0;
   texel[ACOMP] = 1.0;
}

static void
store_texel_r8(struct swrast_texture_image *texImage,
               GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLubyte *dst = TEXEL_ADDR(GLubyte, texImage, i, j, k, 1);
   *dst = rgba[RCOMP];
}



static void
fetch_texel_r16(const struct swrast_texture_image *texImage,
                GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLushort s = *TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   texel[RCOMP] = USHORT_TO_FLOAT(s);
   texel[GCOMP] = 0.0;
   texel[BCOMP] = 0.0;
   texel[ACOMP] = 1.0;
}

static void
store_texel_r16(struct swrast_texture_image *texImage,
                GLint i, GLint j, GLint k, const void *texel)
{
   const GLushort *rgba = (const GLushort *) texel;
   GLushort *dst = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   *dst = rgba[RCOMP];
}



static void
fetch_texel_al88_rev(const struct swrast_texture_image *texImage,
                     GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLushort s = *TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = UBYTE_TO_FLOAT(s >> 8);
   texel[ACOMP] = UBYTE_TO_FLOAT(s & 0xff);
}

static void
store_texel_al88_rev(struct swrast_texture_image *texImage,
                     GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLushort *dst = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   *dst = PACK_COLOR_88(rgba[RCOMP], rgba[ACOMP]);
}



static void
fetch_texel_rg1616(const struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLuint s = *TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   texel[RCOMP] = USHORT_TO_FLOAT(s & 0xffff);
   texel[GCOMP] = USHORT_TO_FLOAT(s >> 16);
   texel[BCOMP] = 0.0;
   texel[ACOMP] = 1.0;
}

static void
store_texel_rg1616(struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLushort *dst = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   *dst = PACK_COLOR_1616(rgba[RCOMP], rgba[GCOMP]);
}



static void
fetch_texel_rg1616_rev(const struct swrast_texture_image *texImage,
                       GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLuint s = *TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   texel[RCOMP] = USHORT_TO_FLOAT(s >> 16);
   texel[GCOMP] = USHORT_TO_FLOAT(s & 0xffff);
   texel[BCOMP] = 0.0;
   texel[ACOMP] = 1.0;
}

static void
store_texel_rg1616_rev(struct swrast_texture_image *texImage,
                       GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLushort *dst = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   *dst = PACK_COLOR_1616(rgba[GCOMP], rgba[RCOMP]);
}



static void
fetch_texel_al1616(const struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLuint s = *TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = USHORT_TO_FLOAT(s & 0xffff);
   texel[ACOMP] = USHORT_TO_FLOAT(s >> 16);
}

static void
store_texel_al1616(struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, const void *texel)
{
   const GLushort *rgba = (const GLushort *) texel;
   GLuint *dst = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   *dst = PACK_COLOR_1616(rgba[ACOMP], rgba[RCOMP]);
}



static void
fetch_texel_al1616_rev(const struct swrast_texture_image *texImage,
                       GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLuint s = *TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = USHORT_TO_FLOAT(s >> 16);
   texel[ACOMP] = USHORT_TO_FLOAT(s & 0xffff);
}

static void
store_texel_al1616_rev(struct swrast_texture_image *texImage,
                       GLint i, GLint j, GLint k, const void *texel)
{
   const GLushort *rgba = (const GLushort *) texel;
   GLuint *dst = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   *dst = PACK_COLOR_1616(rgba[RCOMP], rgba[ACOMP]);
}



static void
fetch_texel_rgb332(const struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLubyte *src = TEXEL_ADDR(GLubyte, texImage, i, j, k, 1);
   const GLubyte s = *src;
   texel[RCOMP] = ((s >> 5) & 0x7) * (1.0F / 7.0F);
   texel[GCOMP] = ((s >> 2) & 0x7) * (1.0F / 7.0F);
   texel[BCOMP] = ((s) & 0x3) * (1.0F / 3.0F);
   texel[ACOMP] = 1.0F;
}

static void
store_texel_rgb332(struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLubyte *dst = TEXEL_ADDR(GLubyte, texImage, i, j, k, 1);
   *dst = PACK_COLOR_332(rgba[RCOMP], rgba[GCOMP], rgba[BCOMP]);
}



static void
fetch_texel_a8(const struct swrast_texture_image *texImage,
               GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLubyte *src = TEXEL_ADDR(GLubyte, texImage, i, j, k, 1);
   texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = 0.0F;
   texel[ACOMP] = UBYTE_TO_FLOAT(src[0]);
}

static void
store_texel_a8(struct swrast_texture_image *texImage,
               GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLubyte *dst = TEXEL_ADDR(GLubyte, texImage, i, j, k, 1);
   *dst = rgba[ACOMP];
}



static void
fetch_texel_a16(const struct swrast_texture_image *texImage,
                GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLushort *src = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = 0.0F;
   texel[ACOMP] = USHORT_TO_FLOAT(src[0]);
}

static void
store_texel_a16(struct swrast_texture_image *texImage,
                GLint i, GLint j, GLint k, const void *texel)
{
   const GLushort *rgba = (const GLushort *) texel;
   GLushort *dst = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   *dst = rgba[ACOMP];
}



static void
fetch_texel_l8(const struct swrast_texture_image *texImage,
               GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLubyte *src = TEXEL_ADDR(GLubyte, texImage, i, j, k, 1);
   texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = UBYTE_TO_FLOAT(src[0]);
   texel[ACOMP] = 1.0F;
}

static void
store_texel_l8(struct swrast_texture_image *texImage,
               GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLubyte *dst = TEXEL_ADDR(GLubyte, texImage, i, j, k, 1);
   *dst = rgba[RCOMP];
}



static void
fetch_texel_l16(const struct swrast_texture_image *texImage,
                GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLushort *src = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = USHORT_TO_FLOAT(src[0]);
   texel[ACOMP] = 1.0F;
}

static void
store_texel_l16(struct swrast_texture_image *texImage,
                GLint i, GLint j, GLint k, const void *texel)
{
   const GLushort *rgba = (const GLushort *) texel;
   GLushort *dst = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   *dst = rgba[RCOMP];
}



static void
fetch_texel_i8(const struct swrast_texture_image *texImage,
               GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLubyte *src = TEXEL_ADDR(GLubyte, texImage, i, j, k, 1);
   texel[RCOMP] =
      texel[GCOMP] = texel[BCOMP] = texel[ACOMP] = UBYTE_TO_FLOAT(src[0]);
}

static void
store_texel_i8(struct swrast_texture_image *texImage,
               GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLubyte *dst = TEXEL_ADDR(GLubyte, texImage, i, j, k, 1);
   *dst = rgba[RCOMP];
}



static void
fetch_texel_i16(const struct swrast_texture_image *texImage,
                GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLushort *src = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   texel[RCOMP] =
      texel[GCOMP] = texel[BCOMP] = texel[ACOMP] = USHORT_TO_FLOAT(src[0]);
}

static void
store_texel_i16(struct swrast_texture_image *texImage,
                GLint i, GLint j, GLint k, const void *texel)
{
   const GLushort *rgba = (const GLushort *) texel;
   GLushort *dst = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   *dst = rgba[RCOMP];
}



static void
fetch_texel_ci8(const struct swrast_texture_image *texImage,
                GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLubyte *src = TEXEL_ADDR(GLubyte, texImage, i, j, k, 1);
   const struct gl_color_table *palette;
   GLuint index;
   GET_CURRENT_CONTEXT(ctx);

   if (ctx->Texture.SharedPalette) {
      palette = &ctx->Texture.Palette;
   }
   else {
      palette = &texImage->Base.TexObject->Palette;
   }
   if (palette->Size == 0)
      return;                   /* undefined results */

   /* Mask the index against size of palette to avoid going out of bounds */
   index = (*src) & (palette->Size - 1);

   {
      const GLfloat *table = palette->TableF;
      switch (palette->_BaseFormat) {
      case GL_ALPHA:
         texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = 0.0F;
         texel[ACOMP] = table[index];
         break;
      case GL_LUMINANCE:
         texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = table[index];
         texel[ACOMP] = 1.0F;
         break;
      case GL_INTENSITY:
         texel[RCOMP] =
            texel[GCOMP] = texel[BCOMP] = texel[ACOMP] = table[index];
         break;
      case GL_LUMINANCE_ALPHA:
         texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = table[index * 2 + 0];
         texel[ACOMP] = table[index * 2 + 1];
         break;
      case GL_RGB:
         texel[RCOMP] = table[index * 3 + 0];
         texel[GCOMP] = table[index * 3 + 1];
         texel[BCOMP] = table[index * 3 + 2];
         texel[ACOMP] = 1.0F;
         break;
      case GL_RGBA:
         texel[RCOMP] = table[index * 4 + 0];
         texel[GCOMP] = table[index * 4 + 1];
         texel[BCOMP] = table[index * 4 + 2];
         texel[ACOMP] = table[index * 4 + 3];
         break;
      default:
         _mesa_problem(ctx, "Bad palette format in fetch_texel_ci8");
         return;
      }
   }
}


static void
store_texel_ci8(struct swrast_texture_image *texImage,
                GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *index = (const GLubyte *) texel;
   GLubyte *dst = TEXEL_ADDR(GLubyte, texImage, i, j, k, 1);
   *dst = *index;
}



/* Note: component order is same as for MESA_FORMAT_RGB888 */
static void
fetch_texel_srgb8(const struct swrast_texture_image *texImage,
                  GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLubyte *src = TEXEL_ADDR(GLubyte, texImage, i, j, k, 3);
   texel[RCOMP] = nonlinear_to_linear(src[2]);
   texel[GCOMP] = nonlinear_to_linear(src[1]);
   texel[BCOMP] = nonlinear_to_linear(src[0]);
   texel[ACOMP] = 1.0F;
}

static void
store_texel_srgb8(struct swrast_texture_image *texImage,
                  GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLubyte *dst = TEXEL_ADDR(GLubyte, texImage, i, j, k, 3);
   dst[0] = rgba[BCOMP];        /* no conversion */
   dst[1] = rgba[GCOMP];
   dst[2] = rgba[RCOMP];
}


static void
fetch_texel_srgba8(const struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLuint s = *TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   texel[RCOMP] = nonlinear_to_linear((s >> 24));
   texel[GCOMP] = nonlinear_to_linear((s >> 16) & 0xff);
   texel[BCOMP] = nonlinear_to_linear((s >> 8) & 0xff);
   texel[ACOMP] = UBYTE_TO_FLOAT((s) & 0xff);   /* linear! */
}

static void
store_texel_srgba8(struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLuint *dst = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   *dst = PACK_COLOR_8888(rgba[RCOMP], rgba[GCOMP], rgba[BCOMP], rgba[ACOMP]);
}


static void
fetch_texel_sargb8(const struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLuint s = *TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   texel[RCOMP] = nonlinear_to_linear((s >> 16) & 0xff);
   texel[GCOMP] = nonlinear_to_linear((s >> 8) & 0xff);
   texel[BCOMP] = nonlinear_to_linear((s) & 0xff);
   texel[ACOMP] = UBYTE_TO_FLOAT((s >> 24));    /* linear! */
}

static void
store_texel_sargb8(struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLuint *dst = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   *dst = PACK_COLOR_8888(rgba[ACOMP], rgba[RCOMP], rgba[GCOMP], rgba[BCOMP]);
}


static void
fetch_texel_sl8(const struct swrast_texture_image *texImage,
                GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLubyte *src = TEXEL_ADDR(GLubyte, texImage, i, j, k, 1);
   texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = nonlinear_to_linear(src[0]);
   texel[ACOMP] = 1.0F;
}

static void
store_texel_sl8(struct swrast_texture_image *texImage,
                GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLubyte *dst = TEXEL_ADDR(GLubyte, texImage, i, j, k, 1);
   dst[0] = rgba[RCOMP];
}


static void
fetch_texel_sla8(const struct swrast_texture_image *texImage,
                 GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLubyte *src = TEXEL_ADDR(GLubyte, texImage, i, j, k, 2);
   texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = nonlinear_to_linear(src[0]);
   texel[ACOMP] = UBYTE_TO_FLOAT(src[1]);       /* linear */
}

static void
store_texel_sla8(struct swrast_texture_image *texImage,
                 GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLubyte *dst = TEXEL_ADDR(GLubyte, texImage, i, j, k, 2);
   dst[0] = rgba[RCOMP];
   dst[1] = rgba[ACOMP];
}



static void
fetch_texel_rgba_int8(const struct swrast_texture_image *texImage,
                      GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLbyte *src = TEXEL_ADDR(GLbyte, texImage, i, j, k, 4);
   texel[RCOMP] = (GLfloat) src[0];
   texel[GCOMP] = (GLfloat) src[1];
   texel[BCOMP] = (GLfloat) src[2];
   texel[ACOMP] = (GLfloat) src[3];
}

static void
store_texel_rgba_int8(struct swrast_texture_image *texImage,
                      GLint i, GLint j, GLint k, const void *texel)
{
   const GLbyte *rgba = (const GLbyte *) texel;
   GLbyte *dst = TEXEL_ADDR(GLbyte, texImage, i, j, k, 4);
   dst[0] = rgba[RCOMP];
   dst[1] = rgba[GCOMP];
   dst[2] = rgba[BCOMP];
   dst[3] = rgba[ACOMP];
}



static void
fetch_texel_rgba_int16(const struct swrast_texture_image *texImage,
                       GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLshort *src = TEXEL_ADDR(GLshort, texImage, i, j, k, 4);
   texel[RCOMP] = (GLfloat) src[0];
   texel[GCOMP] = (GLfloat) src[1];
   texel[BCOMP] = (GLfloat) src[2];
   texel[ACOMP] = (GLfloat) src[3];
}

static void
store_texel_rgba_int16(struct swrast_texture_image *texImage,
                       GLint i, GLint j, GLint k, const void *texel)
{
   const GLshort *rgba = (const GLshort *) texel;
   GLshort *dst = TEXEL_ADDR(GLshort, texImage, i, j, k, 4);
   dst[0] = rgba[RCOMP];
   dst[1] = rgba[GCOMP];
   dst[2] = rgba[BCOMP];
   dst[3] = rgba[ACOMP];
}



static void
fetch_texel_rgba_int32(const struct swrast_texture_image *texImage,
                       GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLint *src = TEXEL_ADDR(GLint, texImage, i, j, k, 4);
   texel[RCOMP] = (GLfloat) src[0];
   texel[GCOMP] = (GLfloat) src[1];
   texel[BCOMP] = (GLfloat) src[2];
   texel[ACOMP] = (GLfloat) src[3];
}

static void
store_texel_rgba_int32(struct swrast_texture_image *texImage,
                       GLint i, GLint j, GLint k, const void *texel)
{
   const GLint *rgba = (const GLint *) texel;
   GLint *dst = TEXEL_ADDR(GLint, texImage, i, j, k, 4);
   dst[0] = rgba[RCOMP];
   dst[1] = rgba[GCOMP];
   dst[2] = rgba[BCOMP];
   dst[3] = rgba[ACOMP];
}



static void
fetch_texel_rgba_uint8(const struct swrast_texture_image *texImage,
                       GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLubyte *src = TEXEL_ADDR(GLubyte, texImage, i, j, k, 4);
   texel[RCOMP] = (GLfloat) src[0];
   texel[GCOMP] = (GLfloat) src[1];
   texel[BCOMP] = (GLfloat) src[2];
   texel[ACOMP] = (GLfloat) src[3];
}

static void
store_texel_rgba_uint8(struct swrast_texture_image *texImage,
                       GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLubyte *dst = TEXEL_ADDR(GLubyte, texImage, i, j, k, 4);
   dst[0] = rgba[RCOMP];
   dst[1] = rgba[GCOMP];
   dst[2] = rgba[BCOMP];
   dst[3] = rgba[ACOMP];
}



static void
fetch_texel_rgba_uint16(const struct swrast_texture_image *texImage,
                        GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLushort *src = TEXEL_ADDR(GLushort, texImage, i, j, k, 4);
   texel[RCOMP] = (GLfloat) src[0];
   texel[GCOMP] = (GLfloat) src[1];
   texel[BCOMP] = (GLfloat) src[2];
   texel[ACOMP] = (GLfloat) src[3];
}

static void
store_texel_rgba_uint16(struct swrast_texture_image *texImage,
                        GLint i, GLint j, GLint k, const void *texel)
{
   const GLushort *rgba = (const GLushort *) texel;
   GLushort *dst = TEXEL_ADDR(GLushort, texImage, i, j, k, 4);
   dst[0] = rgba[RCOMP];
   dst[1] = rgba[GCOMP];
   dst[2] = rgba[BCOMP];
   dst[3] = rgba[ACOMP];
}



static void
fetch_texel_rgba_uint32(const struct swrast_texture_image *texImage,
                        GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLuint *src = TEXEL_ADDR(GLuint, texImage, i, j, k, 4);
   texel[RCOMP] = (GLfloat) src[0];
   texel[GCOMP] = (GLfloat) src[1];
   texel[BCOMP] = (GLfloat) src[2];
   texel[ACOMP] = (GLfloat) src[3];
}

static void
store_texel_rgba_uint32(struct swrast_texture_image *texImage,
                        GLint i, GLint j, GLint k, const void *texel)
{
   const GLuint *rgba = (const GLuint *) texel;
   GLuint *dst = TEXEL_ADDR(GLuint, texImage, i, j, k, 4);
   dst[0] = rgba[RCOMP];
   dst[1] = rgba[GCOMP];
   dst[2] = rgba[BCOMP];
   dst[3] = rgba[ACOMP];
}



/* this format by definition produces 0,0,0,1 as rgba values,
   however we'll return the dudv values as rg and fix up elsewhere */
static void
fetch_texel_dudv8(const struct swrast_texture_image *texImage,
                  GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLbyte *src = TEXEL_ADDR(GLbyte, texImage, i, j, k, 2);
   texel[RCOMP] = BYTE_TO_FLOAT(src[0]);
   texel[GCOMP] = BYTE_TO_FLOAT(src[1]);
   texel[BCOMP] = 0;
   texel[ACOMP] = 0;
}


static void
fetch_texel_signed_r8(const struct swrast_texture_image *texImage,
                      GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLbyte s = *TEXEL_ADDR(GLbyte, texImage, i, j, k, 1);
   texel[RCOMP] = BYTE_TO_FLOAT_TEX(s);
   texel[GCOMP] = 0.0F;
   texel[BCOMP] = 0.0F;
   texel[ACOMP] = 1.0F;
}

static void
store_texel_signed_r8(struct swrast_texture_image *texImage,
                      GLint i, GLint j, GLint k, const void *texel)
{
   const GLbyte *rgba = (const GLbyte *) texel;
   GLbyte *dst = TEXEL_ADDR(GLbyte, texImage, i, j, k, 1);
   *dst = rgba[RCOMP];
}



static void
fetch_texel_signed_a8(const struct swrast_texture_image *texImage,
                      GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLbyte s = *TEXEL_ADDR(GLbyte, texImage, i, j, k, 1);
   texel[RCOMP] = 0.0F;
   texel[GCOMP] = 0.0F;
   texel[BCOMP] = 0.0F;
   texel[ACOMP] = BYTE_TO_FLOAT_TEX(s);
}

static void
store_texel_signed_a8(struct swrast_texture_image *texImage,
                      GLint i, GLint j, GLint k, const void *texel)
{
   const GLbyte *rgba = (const GLbyte *) texel;
   GLbyte *dst = TEXEL_ADDR(GLbyte, texImage, i, j, k, 1);
   *dst = rgba[ACOMP];
}



static void
fetch_texel_signed_l8(const struct swrast_texture_image *texImage,
                      GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLbyte s = *TEXEL_ADDR(GLbyte, texImage, i, j, k, 1);
   texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = BYTE_TO_FLOAT_TEX(s);
   texel[ACOMP] = 1.0F;
}

static void
store_texel_signed_l8(struct swrast_texture_image *texImage,
                      GLint i, GLint j, GLint k, const void *texel)
{
   const GLbyte *rgba = (const GLbyte *) texel;
   GLbyte *dst = TEXEL_ADDR(GLbyte, texImage, i, j, k, 1);
   *dst = rgba[RCOMP];
}



static void
fetch_texel_signed_i8(const struct swrast_texture_image *texImage,
                      GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLbyte s = *TEXEL_ADDR(GLbyte, texImage, i, j, k, 1);
   texel[RCOMP] =
      texel[GCOMP] = texel[BCOMP] = texel[ACOMP] = BYTE_TO_FLOAT_TEX(s);
}

static void
store_texel_signed_i8(struct swrast_texture_image *texImage,
                      GLint i, GLint j, GLint k, const void *texel)
{
   const GLbyte *rgba = (const GLbyte *) texel;
   GLbyte *dst = TEXEL_ADDR(GLbyte, texImage, i, j, k, 1);
   *dst = rgba[RCOMP];
}



static void
fetch_texel_signed_rg88_rev(const struct swrast_texture_image *texImage,
                            GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLushort s = *TEXEL_ADDR(GLshort, texImage, i, j, k, 1);
   texel[RCOMP] = BYTE_TO_FLOAT_TEX((GLbyte) (s & 0xff));
   texel[GCOMP] = BYTE_TO_FLOAT_TEX((GLbyte) (s >> 8));
   texel[BCOMP] = 0.0F;
   texel[ACOMP] = 1.0F;
}

static void
store_texel_signed_rg88_rev(struct swrast_texture_image *texImage,
                            GLint i, GLint j, GLint k, const void *texel)
{
   const GLbyte *rg = (const GLbyte *) texel;
   GLushort *dst = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   dst[0] = PACK_COLOR_88(rg[GCOMP], rg[RCOMP]);
}



static void
fetch_texel_signed_al88(const struct swrast_texture_image *texImage,
                        GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLushort s = *TEXEL_ADDR(GLshort, texImage, i, j, k, 1);
   texel[RCOMP] =
      texel[GCOMP] = texel[BCOMP] = BYTE_TO_FLOAT_TEX((GLbyte) (s & 0xff));
   texel[ACOMP] = BYTE_TO_FLOAT_TEX((GLbyte) (s >> 8));
}

static void
store_texel_signed_al88(struct swrast_texture_image *texImage,
                        GLint i, GLint j, GLint k, const void *texel)
{
   const GLbyte *rg = (const GLbyte *) texel;
   GLushort *dst = TEXEL_ADDR(GLushort, texImage, i, j, k, 1);
   dst[0] = PACK_COLOR_88(rg[ACOMP], rg[RCOMP]);
}



static void
fetch_texel_signed_rgbx8888(const struct swrast_texture_image *texImage,
                            GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLuint s = *TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   texel[RCOMP] = BYTE_TO_FLOAT_TEX((GLbyte) (s >> 24));
   texel[GCOMP] = BYTE_TO_FLOAT_TEX((GLbyte) (s >> 16));
   texel[BCOMP] = BYTE_TO_FLOAT_TEX((GLbyte) (s >> 8));
   texel[ACOMP] = 1.0f;
}

static void
store_texel_signed_rgbx8888(struct swrast_texture_image *texImage,
                            GLint i, GLint j, GLint k, const void *texel)
{
   const GLbyte *rgba = (const GLbyte *) texel;
   GLuint *dst = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   *dst = PACK_COLOR_8888(rgba[RCOMP], rgba[GCOMP], rgba[BCOMP], 255);
}



static void
fetch_texel_signed_rgba8888(const struct swrast_texture_image *texImage,
                            GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLuint s = *TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   texel[RCOMP] = BYTE_TO_FLOAT_TEX((GLbyte) (s >> 24));
   texel[GCOMP] = BYTE_TO_FLOAT_TEX((GLbyte) (s >> 16));
   texel[BCOMP] = BYTE_TO_FLOAT_TEX((GLbyte) (s >> 8));
   texel[ACOMP] = BYTE_TO_FLOAT_TEX((GLbyte) (s));
}

static void
store_texel_signed_rgba8888(struct swrast_texture_image *texImage,
                            GLint i, GLint j, GLint k, const void *texel)
{
   const GLbyte *rgba = (const GLbyte *) texel;
   GLuint *dst = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   *dst = PACK_COLOR_8888(rgba[RCOMP], rgba[GCOMP], rgba[BCOMP], rgba[ACOMP]);
}


static void
fetch_texel_signed_rgba8888_rev(const struct swrast_texture_image *texImage,
                                GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLuint s = *TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   texel[RCOMP] = BYTE_TO_FLOAT_TEX((GLbyte) (s));
   texel[GCOMP] = BYTE_TO_FLOAT_TEX((GLbyte) (s >> 8));
   texel[BCOMP] = BYTE_TO_FLOAT_TEX((GLbyte) (s >> 16));
   texel[ACOMP] = BYTE_TO_FLOAT_TEX((GLbyte) (s >> 24));
}

static void
store_texel_signed_rgba8888_rev(struct swrast_texture_image *texImage,
                                GLint i, GLint j, GLint k, const void *texel)
{
   const GLubyte *rgba = (const GLubyte *) texel;
   GLuint *dst = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   *dst =
      PACK_COLOR_8888_REV(rgba[RCOMP], rgba[GCOMP], rgba[BCOMP], rgba[ACOMP]);
}




static void
fetch_texel_signed_r16(const struct swrast_texture_image *texImage,
                       GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLshort s = *TEXEL_ADDR(GLshort, texImage, i, j, k, 1);
   texel[RCOMP] = SHORT_TO_FLOAT_TEX(s);
   texel[GCOMP] = 0.0F;
   texel[BCOMP] = 0.0F;
   texel[ACOMP] = 1.0F;
}

static void
store_texel_signed_r16(struct swrast_texture_image *texImage,
                       GLint i, GLint j, GLint k, const void *texel)
{
   const GLshort *rgba = (const GLshort *) texel;
   GLshort *dst = TEXEL_ADDR(GLshort, texImage, i, j, k, 1);
   *dst = rgba[0];
}



static void
fetch_texel_signed_a16(const struct swrast_texture_image *texImage,
                       GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLshort s = *TEXEL_ADDR(GLshort, texImage, i, j, k, 1);
   texel[RCOMP] = 0.0F;
   texel[GCOMP] = 0.0F;
   texel[BCOMP] = 0.0F;
   texel[ACOMP] = SHORT_TO_FLOAT_TEX(s);
}

static void
store_texel_signed_a16(struct swrast_texture_image *texImage,
                       GLint i, GLint j, GLint k, const void *texel)
{
   const GLshort *rgba = (const GLshort *) texel;
   GLshort *dst = TEXEL_ADDR(GLshort, texImage, i, j, k, 1);
   *dst = rgba[ACOMP];
}



static void
fetch_texel_signed_l16(const struct swrast_texture_image *texImage,
                       GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLshort s = *TEXEL_ADDR(GLshort, texImage, i, j, k, 1);
   texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = SHORT_TO_FLOAT_TEX(s);
   texel[ACOMP] = 1.0F;
}


static void
store_texel_signed_l16(struct swrast_texture_image *texImage,
                       GLint i, GLint j, GLint k, const void *texel)
{
   const GLshort *rgba = (const GLshort *) texel;
   GLshort *dst = TEXEL_ADDR(GLshort, texImage, i, j, k, 1);
   *dst = rgba[RCOMP];
}



static void
fetch_texel_signed_i16(const struct swrast_texture_image *texImage,
                       GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLshort s = *TEXEL_ADDR(GLshort, texImage, i, j, k, 1);
   texel[RCOMP] =
      texel[GCOMP] = texel[BCOMP] = texel[ACOMP] = SHORT_TO_FLOAT_TEX(s);
}

static void
store_texel_signed_i16(struct swrast_texture_image *texImage,
                       GLint i, GLint j, GLint k, const void *texel)
{
   const GLshort *rgba = (const GLshort *) texel;
   GLshort *dst = TEXEL_ADDR(GLshort, texImage, i, j, k, 1);
   *dst = rgba[RCOMP];
}



static void
fetch_texel_signed_rg1616(const struct swrast_texture_image *texImage,
                          GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLshort *s = TEXEL_ADDR(GLshort, texImage, i, j, k, 2);
   texel[RCOMP] = SHORT_TO_FLOAT_TEX(s[0]);
   texel[GCOMP] = SHORT_TO_FLOAT_TEX(s[1]);
   texel[BCOMP] = 0.0F;
   texel[ACOMP] = 1.0F;
}

static void
store_texel_signed_rg1616(struct swrast_texture_image *texImage,
                          GLint i, GLint j, GLint k, const void *texel)
{
   const GLshort *rgba = (const GLshort *) texel;
   GLshort *dst = TEXEL_ADDR(GLshort, texImage, i, j, k, 2);
   dst[0] = rgba[RCOMP];
   dst[1] = rgba[GCOMP];
}



static void
fetch_texel_signed_al1616(const struct swrast_texture_image *texImage,
                          GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLshort *s = TEXEL_ADDR(GLshort, texImage, i, j, k, 2);
   texel[RCOMP] = texel[GCOMP] = texel[BCOMP] = SHORT_TO_FLOAT_TEX(s[0]);
   texel[ACOMP] = SHORT_TO_FLOAT_TEX(s[1]);
}

static void
store_texel_signed_al1616(struct swrast_texture_image *texImage,
                          GLint i, GLint j, GLint k, const void *texel)
{
   const GLshort *rgba = (const GLshort *) texel;
   GLshort *dst = TEXEL_ADDR(GLshort, texImage, i, j, k, 2);
   dst[0] = rgba[RCOMP];
   dst[1] = rgba[ACOMP];
}



static void
fetch_texel_signed_rgb_16(const struct swrast_texture_image *texImage,
                          GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLshort *s = TEXEL_ADDR(GLshort, texImage, i, j, k, 3);
   texel[RCOMP] = SHORT_TO_FLOAT_TEX(s[0]);
   texel[GCOMP] = SHORT_TO_FLOAT_TEX(s[1]);
   texel[BCOMP] = SHORT_TO_FLOAT_TEX(s[2]);
   texel[ACOMP] = 1.0F;
}

static void
store_texel_signed_rgb_16(struct swrast_texture_image *texImage,
                          GLint i, GLint j, GLint k, const void *texel)
{
   const GLshort *rgba = (const GLshort *) texel;
   GLshort *dst = TEXEL_ADDR(GLshort, texImage, i, j, k, 3);
   dst[0] = rgba[RCOMP];
   dst[1] = rgba[GCOMP];
   dst[2] = rgba[BCOMP];
}



static void
fetch_texel_signed_rgba_16(const struct swrast_texture_image *texImage,
                           GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLshort *s = TEXEL_ADDR(GLshort, texImage, i, j, k, 4);
   texel[RCOMP] = SHORT_TO_FLOAT_TEX(s[0]);
   texel[GCOMP] = SHORT_TO_FLOAT_TEX(s[1]);
   texel[BCOMP] = SHORT_TO_FLOAT_TEX(s[2]);
   texel[ACOMP] = SHORT_TO_FLOAT_TEX(s[3]);
}

static void
store_texel_signed_rgba_16(struct swrast_texture_image *texImage,
                           GLint i, GLint j, GLint k, const void *texel)
{
   const GLshort *rgba = (const GLshort *) texel;
   GLshort *dst = TEXEL_ADDR(GLshort, texImage, i, j, k, 4);
   dst[0] = rgba[RCOMP];
   dst[1] = rgba[GCOMP];
   dst[2] = rgba[BCOMP];
   dst[3] = rgba[ACOMP];
}



static void
fetch_texel_rgba_16(const struct swrast_texture_image *texImage,
                    GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLushort *s = TEXEL_ADDR(GLushort, texImage, i, j, k, 4);
   texel[RCOMP] = USHORT_TO_FLOAT(s[0]);
   texel[GCOMP] = USHORT_TO_FLOAT(s[1]);
   texel[BCOMP] = USHORT_TO_FLOAT(s[2]);
   texel[ACOMP] = USHORT_TO_FLOAT(s[3]);
}

static void
store_texel_rgba_16(struct swrast_texture_image *texImage,
                    GLint i, GLint j, GLint k, const void *texel)
{
   const GLushort *rgba = (const GLushort *) texel;
   GLushort *dst = TEXEL_ADDR(GLushort, texImage, i, j, k, 4);
   dst[0] = rgba[RCOMP];
   dst[1] = rgba[GCOMP];
   dst[2] = rgba[BCOMP];
   dst[3] = rgba[ACOMP];
}



static void
fetch_texel_ycbcr(const struct swrast_texture_image *texImage,
                  GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLushort *src0 = TEXEL_ADDR(GLushort, texImage, (i & ~1), j, k, 1);    /* even */
   const GLushort *src1 = src0 + 1;     /* odd */
   const GLubyte y0 = (*src0 >> 8) & 0xff;      /* luminance */
   const GLubyte cb = *src0 & 0xff;     /* chroma U */
   const GLubyte y1 = (*src1 >> 8) & 0xff;      /* luminance */
   const GLubyte cr = *src1 & 0xff;     /* chroma V */
   const GLubyte y = (i & 1) ? y1 : y0; /* choose even/odd luminance */
   GLfloat r = 1.164F * (y - 16) + 1.596F * (cr - 128);
   GLfloat g = 1.164F * (y - 16) - 0.813F * (cr - 128) - 0.391F * (cb - 128);
   GLfloat b = 1.164F * (y - 16) + 2.018F * (cb - 128);
   r *= (1.0F / 255.0F);
   g *= (1.0F / 255.0F);
   b *= (1.0F / 255.0F);
   texel[RCOMP] = CLAMP(r, 0.0F, 1.0F);
   texel[GCOMP] = CLAMP(g, 0.0F, 1.0F);
   texel[BCOMP] = CLAMP(b, 0.0F, 1.0F);
   texel[ACOMP] = 1.0F;
}

static void
store_texel_ycbcr(struct swrast_texture_image *texImage,
                  GLint i, GLint j, GLint k, const void *texel)
{
   (void) i;
   (void) j;
   (void) texel;
   /* XXX to do */
}



static void
fetch_texel_ycbcr_rev(const struct swrast_texture_image *texImage,
                      GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLushort *src0 = TEXEL_ADDR(GLushort, texImage, (i & ~1), j, k, 1);    /* even */
   const GLushort *src1 = src0 + 1;     /* odd */
   const GLubyte y0 = *src0 & 0xff;     /* luminance */
   const GLubyte cr = (*src0 >> 8) & 0xff;      /* chroma V */
   const GLubyte y1 = *src1 & 0xff;     /* luminance */
   const GLubyte cb = (*src1 >> 8) & 0xff;      /* chroma U */
   const GLubyte y = (i & 1) ? y1 : y0; /* choose even/odd luminance */
   GLfloat r = 1.164F * (y - 16) + 1.596F * (cr - 128);
   GLfloat g = 1.164F * (y - 16) - 0.813F * (cr - 128) - 0.391F * (cb - 128);
   GLfloat b = 1.164F * (y - 16) + 2.018F * (cb - 128);
   r *= (1.0F / 255.0F);
   g *= (1.0F / 255.0F);
   b *= (1.0F / 255.0F);
   texel[RCOMP] = CLAMP(r, 0.0F, 1.0F);
   texel[GCOMP] = CLAMP(g, 0.0F, 1.0F);
   texel[BCOMP] = CLAMP(b, 0.0F, 1.0F);
   texel[ACOMP] = 1.0F;
}

static void
store_texel_ycbcr_rev(struct swrast_texture_image *texImage,
                      GLint i, GLint j, GLint k, const void *texel)
{
   (void) i;
   (void) j;
   (void) texel;
   /* XXX to do */
}


static void
fetch_texel_z24_s8(const struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, GLfloat * texel)
{
   /* only return Z, not stencil data */
   const GLuint *src = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   const GLfloat scale = 1.0F / (GLfloat) 0xffffff;
   texel[0] = ((*src) >> 8) * scale;
   ASSERT(texel[0] >= 0.0F);
   ASSERT(texel[0] <= 1.0F);
}

static void
store_texel_z24_s8(struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, const void *texel)
{
   /* only store Z, not stencil */
   GLuint *dst = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   GLfloat depth = *((GLfloat *) texel);
   GLuint zi = ((GLuint) (depth * 0xffffff)) << 8;
   *dst = zi | (*dst & 0xff);
}



static void
fetch_texel_s8_z24(const struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, GLfloat * texel)
{
   /* only return Z, not stencil data */
   const GLuint *src = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   const GLfloat scale = 1.0F / (GLfloat) 0xffffff;
   texel[0] = ((*src) & 0x00ffffff) * scale;
   ASSERT(texel[0] >= 0.0F);
   ASSERT(texel[0] <= 1.0F);
}

static void
store_texel_s8_z24(struct swrast_texture_image *texImage,
                   GLint i, GLint j, GLint k, const void *texel)
{
   /* only store Z, not stencil */
   GLuint *dst = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   GLfloat depth = *((GLfloat *) texel);
   GLuint zi = (GLuint) (depth * 0xffffff);
   *dst = zi | (*dst & 0xff000000);
}



static void
fetch_texel_rgb9_e5(const struct swrast_texture_image *texImage,
                    GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLuint *src = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   rgb9e5_to_float3(*src, texel);
   texel[ACOMP] = 1.0F;
}

static void
store_texel_rgb9_e5(struct swrast_texture_image *texImage,
                    GLint i, GLint j, GLint k, const void *texel)
{
   const GLfloat *src = (const GLfloat *) texel;
   GLuint *dst = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   *dst = float3_to_rgb9e5(src);
}



static void
fetch_texel_r11_g11_b10f(const struct swrast_texture_image *texImage,
                         GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLuint *src = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   r11g11b10f_to_float3(*src, texel);
   texel[ACOMP] = 1.0F;
}

static void
store_texel_r11_g11_b10f(struct swrast_texture_image *texImage,
                         GLint i, GLint j, GLint k, const void *texel)
{
   const GLfloat *src = (const GLfloat *) texel;
   GLuint *dst = TEXEL_ADDR(GLuint, texImage, i, j, k, 1);
   *dst = float3_to_r11g11b10f(src);
}



/** Just a (temporary?) wrapper around the old texture fetch function. */
static void
_swrast_fetch_texel_srgb_dxt1(const struct swrast_texture_image *texImage,
                              GLint i, GLint j, GLint k, GLfloat * texel)
{
   _mesa_fetch_texel_2d_f_srgb_dxt1(texImage, i, j, k, texel);
}

/** Just a (temporary?) wrapper around the old texture fetch function. */
static void
_swrast_fetch_texel_srgba_dxt1(const struct swrast_texture_image *texImage,
                               GLint i, GLint j, GLint k, GLfloat * texel)
{
   _mesa_fetch_texel_2d_f_srgba_dxt1(texImage, i, j, k, texel);
}

/** Just a (temporary?) wrapper around the old texture fetch function. */
static void
_swrast_fetch_texel_srgba_dxt3(const struct swrast_texture_image *texImage,
                               GLint i, GLint j, GLint k, GLfloat * texel)
{
   _mesa_fetch_texel_2d_f_srgba_dxt3(texImage, i, j, k, texel);
}

/** Just a (temporary?) wrapper around the old texture fetch function. */
static void
_swrast_fetch_texel_srgba_dxt5(const struct swrast_texture_image *texImage,
                               GLint i, GLint j, GLint k, GLfloat * texel)
{
   _mesa_fetch_texel_2d_f_srgba_dxt5(texImage, i, j, 0, texel);
}



static void
fetch_texel_z32x24s8(const struct swrast_texture_image *texImage,
                     GLint i, GLint j, GLint k, GLfloat * texel)
{
   const GLfloat *src = TEXEL_ADDR(GLfloat, texImage, i, j, k, 2);
   texel[RCOMP] = src[0];
   texel[GCOMP] = 0.0F;
   texel[BCOMP] = 0.0F;
   texel[ACOMP] = 1.0F;
}

static void
store_texel_z32x24s8(struct swrast_texture_image *texImage,
                     GLint i, GLint j, GLint k, const void *texel)
{
   const GLfloat *src = (const GLfloat *) texel;
   GLfloat *dst = TEXEL_ADDR(GLfloat, texImage, i, j, k, 2);
   dst[0] = src[0];
}




/**
 * Table to map MESA_FORMAT_ to texel fetch/store funcs.
 * XXX this is somewhat temporary.
 */
static struct
{
   gl_format Name;
   FetchTexelFunc Fetch;
   StoreTexelFunc Store;
}
texfetch_funcs[MESA_FORMAT_COUNT] = {
   { MESA_FORMAT_NONE, fetch_null_texel, store_null_texel },
   { MESA_FORMAT_RGBA8888, fetch_texel_rgba8888, store_texel_rgba8888 },
   { MESA_FORMAT_RGBA8888_REV, fetch_texel_rgba8888_rev, store_texel_rgba8888_rev },
   { MESA_FORMAT_ARGB8888, fetch_texel_argb8888, store_texel_argb8888 },
   { MESA_FORMAT_ARGB8888_REV, fetch_texel_argb8888_rev, store_texel_argb8888_rev },
   { MESA_FORMAT_XRGB8888, fetch_texel_xrgb8888, store_texel_xrgb8888 },
   { MESA_FORMAT_XRGB8888_REV, fetch_texel_xrgb8888_rev, store_texel_xrgb8888_rev },
   { MESA_FORMAT_RGB888, fetch_texel_rgb888, store_texel_rgb888 },
   { MESA_FORMAT_BGR888, fetch_texel_bgr888, store_texel_bgr888 },
   { MESA_FORMAT_RGB565, fetch_texel_rgb565, store_texel_rgb565 },
   { MESA_FORMAT_RGB565_REV, fetch_texel_rgb565_rev, store_texel_rgb565_rev },
   { MESA_FORMAT_ARGB4444, fetch_texel_argb4444, store_texel_argb4444 },
   { MESA_FORMAT_ARGB4444_REV, fetch_texel_argb4444_rev, store_texel_argb4444_rev },
   { MESA_FORMAT_RGBA5551, fetch_texel_rgba5551, store_texel_rgba5551 },
   { MESA_FORMAT_ARGB1555, fetch_texel_argb1555, store_texel_argb1555 },
   { MESA_FORMAT_ARGB1555_REV, fetch_texel_argb1555_rev, store_texel_argb1555_rev },
   { MESA_FORMAT_AL44, fetch_texel_al44, store_texel_al44 },
   { MESA_FORMAT_AL88, fetch_texel_al88, store_texel_al88 },
   { MESA_FORMAT_AL88_REV, fetch_texel_al88_rev, store_texel_al88_rev },
   { MESA_FORMAT_AL1616, fetch_texel_al1616, store_texel_al1616 },
   { MESA_FORMAT_AL1616_REV, fetch_texel_al1616_rev, store_texel_al1616_rev },
   { MESA_FORMAT_RGB332, fetch_texel_rgb332, store_texel_rgb332 },
   { MESA_FORMAT_A8, fetch_texel_a8, store_texel_a8 },
   { MESA_FORMAT_A16, fetch_texel_a16, store_texel_a16 },
   { MESA_FORMAT_L8, fetch_texel_l8, store_texel_l8 },
   { MESA_FORMAT_L16, fetch_texel_l16, store_texel_l16 },
   { MESA_FORMAT_I8, fetch_texel_i8, store_texel_i8 },
   { MESA_FORMAT_I16, fetch_texel_i16, store_texel_i16 },
   { MESA_FORMAT_CI8, fetch_texel_ci8, store_texel_ci8 },
   { MESA_FORMAT_YCBCR, fetch_texel_ycbcr, store_texel_ycbcr },
   { MESA_FORMAT_YCBCR_REV, fetch_texel_ycbcr_rev, store_texel_ycbcr_rev },
   { MESA_FORMAT_R8, fetch_texel_r8, store_texel_r8 },
   { MESA_FORMAT_RG88, fetch_texel_rg88, store_texel_rg88 },
   { MESA_FORMAT_RG88_REV, fetch_texel_rg88_rev, store_texel_rg88_rev },
   { MESA_FORMAT_R16, fetch_texel_r16, store_texel_r16 },
   { MESA_FORMAT_RG1616, fetch_texel_rg1616, store_texel_rg1616 },
   { MESA_FORMAT_RG1616_REV, fetch_texel_rg1616_rev, store_texel_rg1616_rev },
   { MESA_FORMAT_ARGB2101010, fetch_texel_argb2101010, store_texel_argb2101010 },
   { MESA_FORMAT_Z24_S8, fetch_texel_z24_s8, store_texel_z24_s8 },
   { MESA_FORMAT_S8_Z24, fetch_texel_s8_z24, store_texel_s8_z24 },
   { MESA_FORMAT_Z16, fetch_texel_z16, store_texel_z16 },
   { MESA_FORMAT_X8_Z24, fetch_texel_s8_z24, store_texel_s8_z24 },
   { MESA_FORMAT_Z24_X8, fetch_texel_z24_s8, store_texel_z24_s8 },
   { MESA_FORMAT_Z32, fetch_texel_z32, store_texel_z32 },
   { MESA_FORMAT_S8, NULL, NULL },
   { MESA_FORMAT_SRGB8, fetch_texel_srgb8, store_texel_srgb8 },
   { MESA_FORMAT_SRGBA8, fetch_texel_srgba8, store_texel_srgba8 },
   { MESA_FORMAT_SARGB8, fetch_texel_sargb8, store_texel_sargb8 },
   { MESA_FORMAT_SL8, fetch_texel_sl8, store_texel_sl8 },
   { MESA_FORMAT_SLA8, fetch_texel_sla8, store_texel_sla8 },
   { MESA_FORMAT_SRGB_DXT1, _swrast_fetch_texel_srgb_dxt1, NULL },
   { MESA_FORMAT_SRGBA_DXT1, _swrast_fetch_texel_srgba_dxt1, NULL },
   { MESA_FORMAT_SRGBA_DXT3, _swrast_fetch_texel_srgba_dxt3, NULL },
   { MESA_FORMAT_SRGBA_DXT5, _swrast_fetch_texel_srgba_dxt5, NULL },
   { MESA_FORMAT_RGB_FXT1, _mesa_fetch_texel_2d_f_rgb_fxt1, NULL },
   { MESA_FORMAT_RGBA_FXT1, _mesa_fetch_texel_2d_f_rgba_fxt1, NULL },
   { MESA_FORMAT_RGB_DXT1, _mesa_fetch_texel_2d_f_rgb_dxt1, NULL },
   { MESA_FORMAT_RGBA_DXT1, _mesa_fetch_texel_2d_f_rgba_dxt1, NULL },
   { MESA_FORMAT_RGBA_DXT3, _mesa_fetch_texel_2d_f_rgba_dxt3, NULL },
   { MESA_FORMAT_RGBA_DXT5, _mesa_fetch_texel_2d_f_rgba_dxt5, NULL },
   { MESA_FORMAT_RGBA_FLOAT32, fetch_texel_rgba_f32, store_texel_rgba_f32 },
   { MESA_FORMAT_RGBA_FLOAT16, fetch_texel_rgba_f16, store_texel_rgba_f16 },
   { MESA_FORMAT_RGB_FLOAT32, fetch_texel_rgb_f32, store_texel_rgb_f32 },
   { MESA_FORMAT_RGB_FLOAT16, fetch_texel_rgb_f16, store_texel_rgb_f16 },
   { MESA_FORMAT_ALPHA_FLOAT32, fetch_texel_alpha_f32, store_texel_alpha_f32 },
   { MESA_FORMAT_ALPHA_FLOAT16, fetch_texel_alpha_f16, store_texel_alpha_f16 },
   { MESA_FORMAT_LUMINANCE_FLOAT32, fetch_texel_luminance_f32, store_texel_luminance_f32 },
   { MESA_FORMAT_LUMINANCE_FLOAT16, fetch_texel_luminance_f16, store_texel_luminance_f16 },
   { MESA_FORMAT_LUMINANCE_ALPHA_FLOAT32, fetch_texel_luminance_alpha_f32, store_texel_luminance_alpha_f32 },
   { MESA_FORMAT_LUMINANCE_ALPHA_FLOAT16, fetch_texel_luminance_alpha_f16, store_texel_luminance_alpha_f16 },
   { MESA_FORMAT_INTENSITY_FLOAT32, fetch_texel_intensity_f32, store_texel_intensity_f32 },
   { MESA_FORMAT_INTENSITY_FLOAT16, fetch_texel_intensity_f16, store_texel_intensity_f16 },
   { MESA_FORMAT_R_FLOAT32, fetch_texel_r_f32, store_texel_r_f32 },
   { MESA_FORMAT_R_FLOAT16, fetch_texel_r_f16, store_texel_r_f16 },
   { MESA_FORMAT_RG_FLOAT32, fetch_texel_rg_f32, store_texel_rg_f32 },
   { MESA_FORMAT_RG_FLOAT16, fetch_texel_rg_f16, store_texel_rg_f16 },
   /* non-normalized, signed int */
   { MESA_FORMAT_RGBA_INT8, fetch_texel_rgba_int8, store_texel_rgba_int8 },
   { MESA_FORMAT_RGBA_INT16, fetch_texel_rgba_int16, store_texel_rgba_int16 },
   { MESA_FORMAT_RGBA_INT32, fetch_texel_rgba_int32, store_texel_rgba_int32 },
   /* non-normalized, unsigned int */
   { MESA_FORMAT_RGBA_UINT8, fetch_texel_rgba_uint8, store_texel_rgba_uint8 },
   { MESA_FORMAT_RGBA_UINT16, fetch_texel_rgba_uint16, store_texel_rgba_uint16 },
   { MESA_FORMAT_RGBA_UINT32, fetch_texel_rgba_uint32, store_texel_rgba_uint32 },
   /* dudv */
   { MESA_FORMAT_DUDV8, fetch_texel_dudv8, NULL },
      /* signed, normalized */
   { MESA_FORMAT_SIGNED_R8, fetch_texel_signed_r8, store_texel_signed_r8 },
   { MESA_FORMAT_SIGNED_RG88_REV, fetch_texel_signed_rg88_rev, store_texel_signed_rg88_rev },
   { MESA_FORMAT_SIGNED_RGBX8888, fetch_texel_signed_rgbx8888, store_texel_signed_rgbx8888 },
   { MESA_FORMAT_SIGNED_RGBA8888, fetch_texel_signed_rgba8888, store_texel_signed_rgba8888 },
   { MESA_FORMAT_SIGNED_RGBA8888_REV, fetch_texel_signed_rgba8888_rev, store_texel_signed_rgba8888_rev },
   { MESA_FORMAT_SIGNED_R16, fetch_texel_signed_r16, store_texel_signed_r16 },
   { MESA_FORMAT_SIGNED_GR1616, fetch_texel_signed_rg1616, store_texel_signed_rg1616 },
   { MESA_FORMAT_SIGNED_RGB_16, fetch_texel_signed_rgb_16, store_texel_signed_rgb_16 },
   { MESA_FORMAT_SIGNED_RGBA_16, fetch_texel_signed_rgba_16, store_texel_signed_rgba_16 },
   { MESA_FORMAT_RGBA_16, fetch_texel_rgba_16, store_texel_rgba_16 },
   { MESA_FORMAT_RED_RGTC1, _mesa_fetch_texel_2d_f_red_rgtc1, NULL },
   { MESA_FORMAT_SIGNED_RED_RGTC1, _mesa_fetch_texel_2d_f_signed_red_rgtc1, NULL },
   { MESA_FORMAT_RG_RGTC2, _mesa_fetch_texel_2d_f_rg_rgtc2, NULL },
   { MESA_FORMAT_SIGNED_RG_RGTC2, _mesa_fetch_texel_2d_f_signed_rg_rgtc2, NULL },
   { MESA_FORMAT_L_LATC1, _mesa_fetch_texel_2d_f_l_latc1, NULL },
   { MESA_FORMAT_SIGNED_L_LATC1, _mesa_fetch_texel_2d_f_signed_l_latc1, NULL },
   { MESA_FORMAT_LA_LATC2, _mesa_fetch_texel_2d_f_la_latc2, NULL },
   { MESA_FORMAT_SIGNED_LA_LATC2, _mesa_fetch_texel_2d_f_signed_la_latc2, NULL },
   { MESA_FORMAT_SIGNED_A8, fetch_texel_signed_a8, store_texel_signed_a8 },
   { MESA_FORMAT_SIGNED_L8, fetch_texel_signed_l8, store_texel_signed_l8 },
   { MESA_FORMAT_SIGNED_AL88, fetch_texel_signed_al88, store_texel_signed_al88 },
   { MESA_FORMAT_SIGNED_I8, fetch_texel_signed_i8, store_texel_signed_i8 },
   { MESA_FORMAT_SIGNED_A16, fetch_texel_signed_a16, store_texel_signed_a16 },
   { MESA_FORMAT_SIGNED_L16, fetch_texel_signed_l16, store_texel_signed_l16 },
   { MESA_FORMAT_SIGNED_AL1616, fetch_texel_signed_al1616, store_texel_signed_al1616 },
   { MESA_FORMAT_SIGNED_I16, fetch_texel_signed_i16, store_texel_signed_i16 },
   { MESA_FORMAT_RGB9_E5_FLOAT, fetch_texel_rgb9_e5, store_texel_rgb9_e5 },
   { MESA_FORMAT_R11_G11_B10_FLOAT, fetch_texel_r11_g11_b10f, store_texel_r11_g11_b10f },
   { MESA_FORMAT_Z32_FLOAT, fetch_texel_r_f32, store_texel_r_f32 },
   { MESA_FORMAT_Z32_FLOAT_X24S8, fetch_texel_z32x24s8, store_texel_z32x24s8 }
};


FetchTexelFunc
_swrast_get_texel_fetch_func(gl_format format)
{
#ifdef DEBUG
   /* check that the table entries are sorted by format name */
   gl_format fmt;
   for (fmt = 0; fmt < MESA_FORMAT_COUNT; fmt++) {
      assert(texfetch_funcs[fmt].Name == fmt);
   }
#endif

   assert(Elements(texfetch_funcs) == MESA_FORMAT_COUNT);
   assert(format < MESA_FORMAT_COUNT);

   return texfetch_funcs[format].Fetch;
}


StoreTexelFunc
_swrast_get_texel_store_func(gl_format format)
{
   assert(format < MESA_FORMAT_COUNT);
   return texfetch_funcs[format].Store;
}
