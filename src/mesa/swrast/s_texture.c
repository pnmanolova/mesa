/*
 * Mesa 3-D graphics library
 *
 * Copyright (C) 2011 VMware, Inc.
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
 * THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * Functions for mapping/unmapping texture images and renderbuffers.
 */


#include "main/context.h"
#include "main/fbobject.h"
#include "swrast/swrast.h"
#include "swrast/s_context.h"


#if 0
struct swrast_renderbuffer
{
   struct gl_renderbuffer Base;
   GLboolean Mapped;
   void *Buffer;
};



static struct swrast_renderbuffer *
swrast_renderbuffer(struct gl_renderbuffer *rb)
{
   return (struct swrast_renderbuffer *) rb;
}



/**
 * Map a renderbuffer's memory into user space.
 * (x,y,w,h) defines a region of interest (ROI).  Reading/writing pixels
 * outside of the ROI is undefined.
 *
 * \param rb  the renderbuffer
 * \param x, y, w, h  region of interest
 * \param mode  bitmask of GL_MAP_READ_BIT, GL_MAP_WRITE_BIT,
 *              GL_MAP_INVALIDATE_RANGE_BIT
 * \param mapOut  returns start of mapping of ROI
 * \param rowStrideOut  returns row stride (in bytes)
 */
void
_swrast_map_renderbuffer(struct gl_context *ctx,
                         struct gl_renderbuffer *rb,
                         GLuint x, GLuint y, GLuint w, GLuint h,
                         GLbitfield mode,
                         GLubyte **mapOut,
                         GLint *rowStrideOut)
{
   struct swrast_renderbuffer *srb = swrast_renderbuffer(rb);

   /* For swrast, ignore mapping mode */

   assert(!srb->Mapped);
   srb->Mapped = GL_TRUE;

   *mapOut = srb->Buffer;
   *rowStrideOut = _mesa_format_row_stride(rb->Format, rb->Width);
}


void
_swrast_unmap_renderbuffer(struct gl_context *ctx,
                           struct gl_renderbuffer *rb)
{
   struct swrast_renderbuffer *srb = swrast_renderbuffer(rb);

   assert(srb->Mapped);
   srb->Mapped = GL_FALSE;
}
#endif


/**
 * Are the dimensions of the texture image all powers of two?
 */
static GLboolean
is_power_of_two_teximage(const struct gl_texture_image *texImage)
{
   return ((texImage->Width == 1 || _mesa_is_pow_two(texImage->Width2)) &&
           (texImage->Height == 1 || _mesa_is_pow_two(texImage->Height2)) &&
           (texImage->Depth == 1 || _mesa_is_pow_two(texImage->Depth2)));
}


/**
 * Error checking for debugging only.
 */
static void
_mesa_check_map_teximage(struct gl_texture_image *texImage,
                         GLuint slice, GLuint x, GLuint y, GLuint w, GLuint h)
{

   if (texImage->TexObject->Target == GL_TEXTURE_1D)
      assert(y == 0 && h == 1);

   assert(x < texImage->Width || texImage->Width == 0);
   assert(y < texImage->Height || texImage->Height == 0);
   assert(x + w <= texImage->Width);
   assert(y + h <= texImage->Height);
}


/**
 * Map a 2D slice of a texture image into user space.
 * (x,y,w,h) defines a region of interest (ROI).  Reading/writing texels
 * outside of the ROI is undefined.
 *
 * \param texImage  the texture image
 * \param slice  the 3D image slice or array texture slice
 * \param x, y, w, h  region of interest
 * \param mode  bitmask of GL_MAP_READ_BIT, GL_MAP_WRITE_BIT
 * \param mapOut  returns start of mapping of region of interest
 * \param rowStrideOut  returns row stride (in bytes)
 */
void
_swrast_map_teximage(struct gl_context *ctx,
                     struct gl_texture_image *texImage,
                     GLuint slice,
                     GLuint x, GLuint y, GLuint w, GLuint h,
                     GLbitfield mode,
                     GLubyte **mapOut,
                     GLint *rowStrideOut)
{
   struct swrast_texture_image *swImage;
   GLubyte *map;
   GLint stride, texelSize;
   GLuint bw, bh;

   _mesa_check_map_teximage(texImage, slice, x, y, w, h);

   swImage = swrast_texture_image(texImage);

   texelSize = _mesa_get_format_bytes(texImage->TexFormat);
   stride = _mesa_format_row_stride(texImage->TexFormat, texImage->Width);
   _mesa_get_format_block_size(texImage->TexFormat, &bw, &bh);

   assert(swImage->Data);

   map = swImage->Data;

   if (texImage->TexObject->Target == GL_TEXTURE_3D ||
       texImage->TexObject->Target == GL_TEXTURE_2D_ARRAY) {
      GLuint sliceSize = _mesa_format_image_size(texImage->TexFormat,
                                                 texImage->Width,
                                                 texImage->Height,
                                                 1);
      assert(slice < texImage->Depth);
      map += slice * sliceSize;
   }

   /* apply x/y offset to map address */
   map += stride * (y / bh) + texelSize * (x / bw);

   *mapOut = map;
   *rowStrideOut = stride;
}


void
_swrast_unmap_teximage(struct gl_context *ctx,
                       struct gl_texture_image *texImage,
                       GLuint slice)
{
   /* nop */
}



static gl_format
find_texture_format(const struct gl_texture_object *obj)
{
   GLuint level;
   for (level = 0; level < MAX_TEXTURE_LEVELS; level++) {
      if (obj->Image[0][level]) {
         return obj->Image[0][level]->TexFormat;
      }
   }
   return MESA_FORMAT_NONE;
}


/**
 * Map all images of the given texture object.  Plus, update the Fetch/Store
 * function pointers.
 * Called via ctx->Driver.MapTexture()
 * \param obj  the texture object whose images to map
 * \param swtexture  returns the info about the mapped texture (addr, stride)
 */
void
_swrast_map_texture(struct gl_context *ctx,
                    struct gl_texture_object *texObj)
{
   const GLuint numFaces = texObj->Target == GL_TEXTURE_CUBE_MAP ? 6 : 1;
   gl_format format = find_texture_format(texObj);
   GLuint level, face, slice;
   FetchTexelFunc fetch;
   StoreTexelFunc store;
   const GLuint texelSize = _mesa_get_format_bytes(format);

   /* determine texture format, get fetch/store functions */
   if (texObj->Sampler.sRGBDecode == GL_SKIP_DECODE_EXT &&
       _mesa_get_format_color_encoding(format) == GL_SRGB) {
      format = _mesa_get_srgb_format_linear(format);
   }

   fetch = _swrast_get_texel_fetch_func(format);
   store = _swrast_get_texel_store_func(format);

   for (face = 0; face < numFaces; face++) {
      for (level = 0; level < MAX_TEXTURE_LEVELS; level++) {
         struct swrast_texture_image *swImage =
            swrast_texture_image(texObj->Image[face][level]);
         if (swImage) {
            if (!swImage->SliceMaps) {
               swImage->SliceMaps =
                  (GLubyte **) calloc(swImage->Base.Depth, sizeof(GLubyte *));
               if (!swImage->SliceMaps) {
                  return; /* XXX error */
               }
            }

            /* map slices */
            for (slice = 0; slice < swImage->Base.Depth; slice++) {
               assert(face * slice == 0);   /* face or slice must be zero */
               ctx->Driver.MapTextureImage(ctx,
                                           &swImage->Base, slice,
                                           0, 0,
                                           swImage->Base.Width,
                                           swImage->Base.Height,
                                           GL_MAP_READ_BIT,
                                           &swImage->SliceMaps[slice],
                                           &swImage->RowStride);
            }

            swImage->TexelSize = texelSize;

            /* set Fetch/Store funcs */
            swImage->Fetch = fetch;
            swImage->Store = store;
         }
      }
   }
}


/**
 * Unmap all images of the given texture object.
 * Called via ctx->Driver.UnmapTexture()
 */
void
_swrast_unmap_texture(struct gl_context *ctx,
                      struct gl_texture_object *texObj)
{
   const GLuint numFaces = texObj->Target == GL_TEXTURE_CUBE_MAP ? 6 : 1;
   GLuint level, face, slice;

   for (face = 0; face < numFaces; face++) {
      for (level = 0; level < MAX_TEXTURE_LEVELS; level++) {
         struct swrast_texture_image *swImage =
            swrast_texture_image(texObj->Image[face][level]);
         if (swImage) {
            assert(swImage->SliceMaps);
            for (slice = 0; slice < swImage->Base.Depth; slice++) {
               assert(face * slice == 0);   /* face or slice must be zero */
               ctx->Driver.UnmapTextureImage(ctx, &swImage->Base, slice);
               swImage->SliceMaps[slice] = NULL;
            }
            swImage->RowStride = 0;
         }
      }
   }
}


/**
 * Map all texture images of all texture units.
 * And update the Fetch/Store function pointers.
 */
void
_swrast_map_textures(struct gl_context *ctx)
{
   GLbitfield enabledUnits = ctx->Texture._EnabledUnits;

   /* Loop over enabled texture units, mapping all texture images */
   while (enabledUnits) {
      const GLuint unit = _mesa_ffs(enabledUnits) - 1;
      struct gl_texture_object *texObj = ctx->Texture.Unit[unit]._Current;
      ASSERT(texObj);
      if (texObj) {
         ctx->Driver.MapTexture(ctx, texObj);
      }
      enabledUnits &= ~(1 << unit);
   }
}


/**
 * Unmap all texture images of all texture units
 */
void
_swrast_unmap_textures(struct gl_context *ctx)
{
   GLbitfield enabledUnits = ctx->Texture._EnabledUnits;

   /* Loop over enabled texture units, unmapping all texture images */
   while (enabledUnits) {
      const GLuint unit = _mesa_ffs(enabledUnits) - 1;
      struct gl_texture_object *texObj = ctx->Texture.Unit[unit]._Current;
      ASSERT(texObj);
      if (texObj) {
         ctx->Driver.UnmapTexture(ctx, texObj);
      }
      enabledUnits &= ~(1 << unit);
   }
}


static void
map_renderbuffer(struct gl_context *ctx,
                 struct gl_renderbuffer_attachment *att)
{
   struct swrast_texture_image *swImg =
      swrast_texture_image(_mesa_get_attachment_teximage(att));
   const GLenum target = att->Texture->Target;
   GLuint slice;

   if (!swImg->SliceMaps) {
      swImg->SliceMaps =
         (GLubyte **) calloc(swImg->Base.Depth, sizeof(GLubyte *));
      if (!swImg->SliceMaps) {
         return; /* XXX error */
      }
   }
   assert(swImg->SliceMaps);

   /* We don't use the Zoffset for 1D array textures */
   if (target == GL_TEXTURE_3D || target == GL_TEXTURE_2D_ARRAY)
      slice = att->Zoffset;
   else
      slice = 0;

   ctx->Driver.MapTextureImage(ctx,
                               &swImg->Base,
                               slice,
                               0, 0,
                               swImg->Base.Width, swImg->Base.Height,
                               GL_MAP_READ_BIT | GL_MAP_WRITE_BIT,
                               &swImg->SliceMaps[slice],
                               &swImg->RowStride);
}


static void
unmap_renderbuffer(struct gl_context *ctx,
                   struct gl_renderbuffer_attachment *att)
{
   const GLenum target = att->Texture->Target;
   struct gl_texture_image *texImage = _mesa_get_attachment_teximage(att);
   struct swrast_texture_image *swImg = swrast_texture_image(texImage);
   GLuint slice;

   /* We don't use the Zoffset for 1D array textures */
   if (target == GL_TEXTURE_3D || target == GL_TEXTURE_2D_ARRAY)
      slice = att->Zoffset;
   else
      slice = 0;

   /* just unmap the texture image buffer */
   ctx->Driver.UnmapTextureImage(ctx, texImage, slice);
   swImg->SliceMaps[slice] = NULL;
}


static void
map_renderbuffers(struct gl_context *ctx, struct gl_framebuffer *fb)
{
   GLuint buf;

   for (buf = 0; buf < Elements(fb->Attachment); buf++) {
      struct gl_renderbuffer_attachment *att = &fb->Attachment[buf];
      if (att->Texture) {
         map_renderbuffer(ctx, att);
      }
   }
}


static void
unmap_renderbuffers(struct gl_context *ctx, struct gl_framebuffer *fb)
{
   GLuint buf;

   for (buf = 0; buf < Elements(fb->Attachment); buf++) {
      struct gl_renderbuffer_attachment *att = &fb->Attachment[buf];
      if (att->Texture) {
         unmap_renderbuffer(ctx, att);
      }
   }
}


/**
 * Map all the renderbuffers (or the textures they point at) prior
 * to software rendering.
 */
void
_swrast_map_renderbuffers(struct gl_context *ctx)
{
   map_renderbuffers(ctx, ctx->DrawBuffer);

   if (ctx->ReadBuffer != ctx->DrawBuffer) {
      map_renderbuffers(ctx, ctx->ReadBuffer);
   }
}


/**
 * Unmap all the renderbuffers (or the textures they point at) after
 * software rendering.
 */
void
_swrast_unmap_renderbuffers(struct gl_context *ctx)
{
   unmap_renderbuffers(ctx, ctx->DrawBuffer);

   if (ctx->ReadBuffer != ctx->DrawBuffer) {
      unmap_renderbuffers(ctx, ctx->ReadBuffer);
   }
}



/**
 * Allocate a new swrast_texture_image (a subclass of gl_texture_image).
 * Called via ctx->Driver.NewTextureImage().
 */
struct gl_texture_image *
_swrast_new_texture_image( struct gl_context *ctx )
{
   (void) ctx;
   return (struct gl_texture_image *) CALLOC_STRUCT(swrast_texture_image);
}


/**
 * Free a swrast_texture_image (a subclass of gl_texture_image).
 * Called via ctx->Driver.DeleteTextureImage().
 */
void
_swrast_delete_texture_image(struct gl_context *ctx,
                             struct gl_texture_image *texImage)
{
   _swrast_free_texture_image_buffer(ctx, texImage);
   free(texImage);
}


/**
 * Called via ctx->Driver.AllocTextureImageBuffer()
 */
GLboolean
_swrast_alloc_texture_image_buffer(struct gl_context *ctx,
                                   struct gl_texture_image *texImage,
                                   gl_format format, GLsizei width,
                                   GLsizei height, GLsizei depth)
{
   struct swrast_texture_image *swImage = swrast_texture_image(texImage);
   GLuint bytes = _mesa_format_image_size(format, width, height, depth);

   /* This _should_ be true (revisit if these ever fail) */
   assert(texImage->Width == width);
   assert(texImage->Height == height);
   assert(texImage->Depth == depth);

   swImage->_IsPowerOfTwo = is_power_of_two_teximage(texImage);

   assert(!swImage->Data);
   swImage->Data = _mesa_align_malloc(bytes, 512);

   if (texImage->TexObject->Target == GL_TEXTURE_RECTANGLE_NV) {
      /* scale = 1.0 since texture coords directly map to texels */
      swImage->WidthScale = 1.0;
      swImage->HeightScale = 1.0;
      swImage->DepthScale = 1.0;
   }
   else {
      swImage->WidthScale = (GLfloat) width;
      swImage->HeightScale = (GLfloat) height;
      swImage->DepthScale = (GLfloat) depth;
   }

   return swImage->Data != NULL;
}


/**
 * Called via ctx->Driver.FreeTextureImageBuffer()
 */
void
_swrast_free_texture_image_buffer(struct gl_context *ctx,
                                  struct gl_texture_image *texImage)
{
   struct swrast_texture_image *swImage = swrast_texture_image(texImage);

   if (swImage->Data && !texImage->IsClientData) {
      _mesa_align_free(swImage->Data);
   }

   swImage->Data = NULL;

   if (swImage->SliceMaps) {
      free(swImage->SliceMaps);
      swImage->SliceMaps = NULL;
   }
}
