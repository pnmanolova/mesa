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

/*
 * Render-to-texture code for GL_EXT_framebuffer_object
 */

#include "main/context.h"
#include "main/colormac.h"
#include "main/fbobject.h"
#include "main/macros.h"
#include "main/teximage.h"
#include "main/renderbuffer.h"
#include "swrast/swrast.h"
#include "swrast/s_context.h"


/**
 * Derived from gl_renderbuffer class
 */
struct texture_renderbuffer
{
   struct gl_renderbuffer Base;   /**< Base class object */
   struct swrast_texture_image *TexImage;
   FetchTexelFunc Fetch;
   StoreTexelFunc Store;
   GLint Yoffset; /**< Layer for 1D array textures. */
   GLint Zoffset; /**< Layer for 2D array textures, or slice for 3D textures */
};


/**
 * Get row of values from the renderbuffer that wraps a texture image.
 */
static void
texture_get_row(struct gl_context *ctx, struct gl_renderbuffer *rb, GLuint count,
                GLint x, GLint y, void *values)
{
   const struct texture_renderbuffer *trb
      = (const struct texture_renderbuffer *) rb;
   struct swrast_texture_image *swImg = trb->TexImage;
   const GLint z = trb->Zoffset;
   GLuint i;

   ASSERT(trb->TexImage->Base.Width == rb->Width);
   ASSERT(trb->TexImage->Base.Height == rb->Height);

   y += trb->Yoffset;

   if (rb->DataType == CHAN_TYPE) {
      GLchan *rgbaOut = (GLchan *) values;
      for (i = 0; i < count; i++) {
         GLfloat rgba[4];
         trb->Fetch(swImg, x + i, y, z, rgba);
         UNCLAMPED_FLOAT_TO_RGBA_CHAN(rgbaOut + 4 * i, rgba);
      }
   }
   else if (rb->DataType == GL_UNSIGNED_SHORT) {
      GLushort *zValues = (GLushort *) values;
      for (i = 0; i < count; i++) {
         GLfloat flt;
         trb->Fetch(swImg, x + i, y, z, &flt);
         zValues[i] = (GLushort) (flt * 0xffff);
      }
   }
   else if (rb->DataType == GL_UNSIGNED_INT) {
      GLuint *zValues = (GLuint *) values;
      /*
      const GLdouble scale = (GLdouble) 0xffffffff;
      */
      for (i = 0; i < count; i++) {
         GLfloat flt;
         trb->Fetch(swImg, x + i, y, z, &flt);
#if 0
         /* this should work, but doesn't (overflow due to low precision) */
         zValues[i] = (GLuint) (flt * scale);
#else
         /* temporary hack */
         zValues[i] = ((GLuint) (flt * 0xffffff)) << 8;
#endif
      }
   }
   else if (rb->DataType == GL_UNSIGNED_INT_24_8_EXT) {
      GLuint *zValues = (GLuint *) values;
      for (i = 0; i < count; i++) {
         GLfloat flt;
         trb->Fetch(swImg, x + i, y, z, &flt);
         zValues[i] = ((GLuint) (flt * 0xffffff)) << 8;
      }
   }
   else if (rb->DataType == GL_UNSIGNED_INT_8_24_REV_MESA) {
      GLuint *zValues = (GLuint *) values;
      for (i = 0; i < count; i++) {
         GLfloat flt;
         trb->Fetch(swImg, x + i, y, z, &flt);
         zValues[i] = (GLuint) (flt * 0xffffff);
      }
   }
   else {
      _mesa_problem(ctx, "invalid rb->DataType in texture_get_row");
   }
}


static void
texture_get_values(struct gl_context *ctx, struct gl_renderbuffer *rb, GLuint count,
                   const GLint x[], const GLint y[], void *values)
{
   const struct texture_renderbuffer *trb
      = (const struct texture_renderbuffer *) rb;
   struct swrast_texture_image *swImg = trb->TexImage;
   const GLint z = trb->Zoffset;
   GLuint i;

   if (rb->DataType == CHAN_TYPE) {
      GLchan *rgbaOut = (GLchan *) values;
      for (i = 0; i < count; i++) {
         GLfloat rgba[4];
         trb->Fetch(swImg, x[i], y[i] + trb->Yoffset,
				    z, rgba);
         UNCLAMPED_FLOAT_TO_RGBA_CHAN(rgbaOut + 4 * i, rgba);
      }
   }
   else if (rb->DataType == GL_UNSIGNED_SHORT) {
      GLushort *zValues = (GLushort *) values;
      for (i = 0; i < count; i++) {
         GLfloat flt;
         trb->Fetch(swImg, x[i], y[i] + trb->Yoffset,
				    z, &flt);
         zValues[i] = (GLushort) (flt * 0xffff);
      }
   }
   else if (rb->DataType == GL_UNSIGNED_INT) {
      GLuint *zValues = (GLuint *) values;
      for (i = 0; i < count; i++) {
         GLfloat flt;
         trb->Fetch(swImg, x[i], y[i] + trb->Yoffset,
				    z, &flt);
#if 0
         zValues[i] = (GLuint) (flt * 0xffffffff);
#else
         zValues[i] = ((GLuint) (flt * 0xffffff)) << 8;
#endif
      }
   }
   else if (rb->DataType == GL_UNSIGNED_INT_24_8_EXT) {
      GLuint *zValues = (GLuint *) values;
      for (i = 0; i < count; i++) {
         GLfloat flt;
         trb->Fetch(swImg, x[i], y[i] + trb->Yoffset,
				    z, &flt);
         zValues[i] = ((GLuint) (flt * 0xffffff)) << 8;
      }
   }
   else if (rb->DataType == GL_UNSIGNED_INT_8_24_REV_MESA) {
      GLuint *zValues = (GLuint *) values;
      for (i = 0; i < count; i++) {
         GLfloat flt;
         trb->Fetch(swImg, x[i], y[i] + trb->Yoffset,
				    z, &flt);
         zValues[i] = (GLuint) (flt * 0xffffff);
      }
   }
   else {
      _mesa_problem(ctx, "invalid rb->DataType in texture_get_values");
   }
}


/**
 * Put row of values into a renderbuffer that wraps a texture image.
 */
static void
texture_put_row(struct gl_context *ctx, struct gl_renderbuffer *rb, GLuint count,
                GLint x, GLint y, const void *values, const GLubyte *mask)
{
   const struct texture_renderbuffer *trb
      = (const struct texture_renderbuffer *) rb;
   struct swrast_texture_image *swImg = trb->TexImage;
   const GLint z = trb->Zoffset;
   GLuint i;

   y += trb->Yoffset;

   if (rb->DataType == CHAN_TYPE) {
      const GLchan *rgba = (const GLchan *) values;
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            trb->Store(swImg, x + i, y, z, rgba);
         }
         rgba += 4;
      }
   }
   else if (rb->DataType == GL_UNSIGNED_SHORT) {
      const GLushort *zValues = (const GLushort *) values;
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            trb->Store(swImg, x + i, y, z, zValues + i);
         }
      }
   }
   else if (rb->DataType == GL_UNSIGNED_INT) {
      const GLuint *zValues = (const GLuint *) values;
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            trb->Store(swImg, x + i, y, z, zValues + i);
         }
      }
   }
   else if (rb->DataType == GL_UNSIGNED_INT_24_8_EXT) {
      const GLuint *zValues = (const GLuint *) values;
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            GLfloat flt = (GLfloat) ((zValues[i] >> 8) * (1.0 / 0xffffff));
            trb->Store(swImg, x + i, y, z, &flt);
         }
      }
   }
   else if (rb->DataType == GL_UNSIGNED_INT_8_24_REV_MESA) {
      const GLuint *zValues = (const GLuint *) values;
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            GLfloat flt = (GLfloat) ((zValues[i] & 0xffffff) * (1.0 / 0xffffff));
            trb->Store(swImg, x + i, y, z, &flt);
         }
      }
   }
   else {
      _mesa_problem(ctx, "invalid rb->DataType in texture_put_row");
   }
}

/**
 * Put row of RGB values into a renderbuffer that wraps a texture image.
 */
static void
texture_put_row_rgb(struct gl_context *ctx, struct gl_renderbuffer *rb, GLuint count,
                GLint x, GLint y, const void *values, const GLubyte *mask)
{
   const struct texture_renderbuffer *trb
      = (const struct texture_renderbuffer *) rb;
   struct swrast_texture_image *swImg = trb->TexImage;
   const GLint z = trb->Zoffset;
   GLuint i;

   y += trb->Yoffset;

   if (rb->DataType == CHAN_TYPE) {
      const GLchan *rgb = (const GLchan *) values;
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            trb->Store(swImg, x + i, y, z, rgb);
         }
         rgb += 3;
      }
   }
   else if (rb->DataType == GL_UNSIGNED_SHORT) {
      const GLushort *zValues = (const GLushort *) values;
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            trb->Store(swImg, x + i, y, z, zValues + i);
         }
      }
   }
   else if (rb->DataType == GL_UNSIGNED_INT) {
      const GLuint *zValues = (const GLuint *) values;
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            trb->Store(swImg, x + i, y, z, zValues + i);
         }
      }
   }
   else if (rb->DataType == GL_UNSIGNED_INT_24_8_EXT) {
      const GLuint *zValues = (const GLuint *) values;
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            GLfloat flt = (GLfloat) ((zValues[i] >> 8) * (1.0 / 0xffffff));
            trb->Store(swImg, x + i, y, z, &flt);
         }
      }
   }
   else if (rb->DataType == GL_UNSIGNED_INT_8_24_REV_MESA) {
      const GLuint *zValues = (const GLuint *) values;
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            GLfloat flt = (GLfloat) ((zValues[i] & 0xffffff) * (1.0 / 0xffffff));
            trb->Store(swImg, x + i, y, z, &flt);
         }
      }
   }
   else {
      _mesa_problem(ctx, "invalid rb->DataType in texture_put_row");
   }
}


static void
texture_put_mono_row(struct gl_context *ctx, struct gl_renderbuffer *rb, GLuint count,
                     GLint x, GLint y, const void *value, const GLubyte *mask)
{
   const struct texture_renderbuffer *trb
      = (const struct texture_renderbuffer *) rb;
   struct swrast_texture_image *swImg = trb->TexImage;
   const GLint z = trb->Zoffset;
   GLuint i;

   y += trb->Yoffset;

   if (rb->DataType == CHAN_TYPE) {
      const GLchan *rgba = (const GLchan *) value;
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            trb->Store(swImg, x + i, y, z, rgba);
         }
      }
   }
   else if (rb->DataType == GL_UNSIGNED_SHORT) {
      const GLushort zValue = *((const GLushort *) value);
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            trb->Store(swImg, x + i, y, z, &zValue);
         }
      }
   }
   else if (rb->DataType == GL_UNSIGNED_INT) {
      const GLuint zValue = *((const GLuint *) value);
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            trb->Store(swImg, x + i, y, z, &zValue);
         }
      }
   }
   else if (rb->DataType == GL_UNSIGNED_INT_24_8_EXT) {
      const GLuint zValue = *((const GLuint *) value);
      const GLfloat flt = (GLfloat) ((zValue >> 8) * (1.0 / 0xffffff));
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            trb->Store(swImg, x + i, y, z, &flt);
         }
      }
   }
   else if (rb->DataType == GL_UNSIGNED_INT_8_24_REV_MESA) {
      const GLuint zValue = *((const GLuint *) value);
      const GLfloat flt = (GLfloat) ((zValue & 0xffffff) * (1.0 / 0xffffff));
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            trb->Store(swImg, x + i, y, z, &flt);
         }
      }
   }
   else {
      _mesa_problem(ctx, "invalid rb->DataType in texture_put_mono_row");
   }
}


static void
texture_put_values(struct gl_context *ctx, struct gl_renderbuffer *rb, GLuint count,
                   const GLint x[], const GLint y[], const void *values,
                   const GLubyte *mask)
{
   const struct texture_renderbuffer *trb
      = (const struct texture_renderbuffer *) rb;
   struct swrast_texture_image *swImg = trb->TexImage;
   const GLint z = trb->Zoffset;
   GLuint i;

   if (rb->DataType == CHAN_TYPE) {
      const GLchan *rgba = (const GLchan *) values;
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            trb->Store(swImg, x[i], y[i] + trb->Yoffset, z, rgba);
         }
         rgba += 4;
      }
   }
   else if (rb->DataType == GL_UNSIGNED_SHORT) {
      const GLushort *zValues = (const GLushort *) values;
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            trb->Store(swImg, x[i], y[i] + trb->Yoffset, z, zValues + i);
         }
      }
   }
   else if (rb->DataType == GL_UNSIGNED_INT) {
      const GLuint *zValues = (const GLuint *) values;
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            trb->Store(swImg, x[i], y[i] + trb->Yoffset, z, zValues + i);
         }
      }
   }
   else if (rb->DataType == GL_UNSIGNED_INT_24_8_EXT) {
      const GLuint *zValues = (const GLuint *) values;
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            GLfloat flt = (GLfloat) ((zValues[i] >> 8) * (1.0 / 0xffffff));
            trb->Store(swImg, x[i], y[i] + trb->Yoffset, z, &flt);
         }
      }
   }
   else if (rb->DataType == GL_UNSIGNED_INT_8_24_REV_MESA) {
      const GLuint *zValues = (const GLuint *) values;
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            GLfloat flt = (GLfloat) ((zValues[i] & 0xffffff) * (1.0 / 0xffffff));
            trb->Store(swImg, x[i], y[i] + trb->Yoffset, z, &flt);
         }
      }
   }
   else {
      _mesa_problem(ctx, "invalid rb->DataType in texture_put_values");
   }
}


static void
texture_put_mono_values(struct gl_context *ctx, struct gl_renderbuffer *rb,
                        GLuint count, const GLint x[], const GLint y[],
                        const void *value, const GLubyte *mask)
{
   const struct texture_renderbuffer *trb
      = (const struct texture_renderbuffer *) rb;
   struct swrast_texture_image *swImg = trb->TexImage;
   const GLint z = trb->Zoffset;
   GLuint i;

   if (rb->DataType == CHAN_TYPE) {
      const GLchan *rgba = (const GLchan *) value;
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            trb->Store(swImg, x[i], y[i] + trb->Yoffset, z, rgba);
         }
      }
   }
   else if (rb->DataType == GL_UNSIGNED_INT) {
      const GLuint zValue = *((const GLuint *) value);
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            trb->Store(swImg, x[i], y[i] + trb->Yoffset, z, &zValue);
         }
      }
   }
   else if (rb->DataType == GL_UNSIGNED_SHORT) {
      const GLushort zValue = *((const GLushort *) value);
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            trb->Store(swImg, x[i], y[i] + trb->Yoffset, z, &zValue);
         }
      }
   }
   else if (rb->DataType == GL_UNSIGNED_INT_24_8_EXT) {
      const GLuint zValue = *((const GLuint *) value);
      const GLfloat flt = (GLfloat) ((zValue >> 8) * (1.0 / 0xffffff));
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            trb->Store(swImg, x[i], y[i] + trb->Yoffset, z, &flt);
         }
      }
   }
   else if (rb->DataType == GL_UNSIGNED_INT_8_24_REV_MESA) {
      const GLuint zValue = *((const GLuint *) value);
      const GLfloat flt = (GLfloat) ((zValue & 0xffffff) * (1.0 / 0xffffff));
      for (i = 0; i < count; i++) {
         if (!mask || mask[i]) {
            trb->Store(swImg, x[i], y[i] + trb->Yoffset, z, &flt);
         }
      }
   }
   else {
      _mesa_problem(ctx, "invalid rb->DataType in texture_put_mono_values");
   }
}


static void
delete_texture_wrapper(struct gl_renderbuffer *rb)
{
   ASSERT(rb->RefCount == 0);
   free(rb);
}


/**
 * This function creates a renderbuffer object which wraps a texture image.
 * The new renderbuffer is plugged into the given attachment point.
 * This allows rendering into the texture as if it were a renderbuffer.
 */
static void
wrap_texture(struct gl_context *ctx, struct gl_renderbuffer_attachment *att)
{
   struct texture_renderbuffer *trb;
   const GLuint name = 0;

   ASSERT(att->Type == GL_TEXTURE);
   ASSERT(att->Renderbuffer == NULL);

   trb = CALLOC_STRUCT(texture_renderbuffer);
   if (!trb) {
      _mesa_error(ctx, GL_OUT_OF_MEMORY, "wrap_texture");
      return;
   }

   /* init base gl_renderbuffer fields */
   _mesa_init_renderbuffer(&trb->Base, name);
   /* plug in our texture_renderbuffer-specific functions */
   trb->Base.Delete = delete_texture_wrapper;
   trb->Base.AllocStorage = NULL; /* illegal! */
   trb->Base.GetRow = texture_get_row;
   trb->Base.GetValues = texture_get_values;
   trb->Base.PutRow = texture_put_row;
   trb->Base.PutRowRGB = texture_put_row_rgb;
   trb->Base.PutMonoRow = texture_put_mono_row;
   trb->Base.PutValues = texture_put_values;
   trb->Base.PutMonoValues = texture_put_mono_values;

   /* update attachment point */
   _mesa_reference_renderbuffer(&att->Renderbuffer, &(trb->Base));
}

/**
 * Update the renderbuffer wrapper for rendering to a texture.
 * For example, update the width, height of the RB based on the texture size,
 * update the internal format info, etc.
 */
static void
update_wrapper(struct gl_context *ctx, struct gl_renderbuffer_attachment *att)
{
   struct texture_renderbuffer *trb
      = (struct texture_renderbuffer *) att->Renderbuffer;

   (void) ctx;
   ASSERT(trb);

   trb->TexImage = swrast_texture_image(_mesa_get_attachment_teximage(att));
   ASSERT(trb->TexImage);

   if (att->Texture->Target == GL_TEXTURE_1D_ARRAY_EXT) {
      trb->Yoffset = att->Zoffset;
      trb->Zoffset = 0;
   }
   else {
      trb->Yoffset = 0;
      trb->Zoffset = att->Zoffset;
   }

   trb->Fetch = trb->TexImage->Fetch;
   trb->Store = trb->TexImage->Store;

   trb->Base.Width = trb->TexImage->Base.Width;
   trb->Base.Height = trb->TexImage->Base.Height;
   trb->Base.InternalFormat = trb->TexImage->Base.InternalFormat;
   trb->Base.Format = trb->TexImage->Base.TexFormat;

   /* XXX may need more special cases here */
   switch (trb->TexImage->Base.TexFormat) {
   case MESA_FORMAT_Z24_S8:
      trb->Base.DataType = GL_UNSIGNED_INT_24_8_EXT;
      trb->Base._BaseFormat = GL_DEPTH_STENCIL;
      break;
   case MESA_FORMAT_S8_Z24:
      trb->Base.DataType = GL_UNSIGNED_INT_8_24_REV_MESA;
      trb->Base._BaseFormat = GL_DEPTH_STENCIL;
      break;
   case MESA_FORMAT_Z24_X8:
      trb->Base.DataType = GL_UNSIGNED_INT_24_8_EXT;
      trb->Base._BaseFormat = GL_DEPTH_COMPONENT;
      break;
   case MESA_FORMAT_X8_Z24:
      trb->Base.DataType = GL_UNSIGNED_INT_8_24_REV_MESA;
      trb->Base._BaseFormat = GL_DEPTH_COMPONENT;
      break;
   case MESA_FORMAT_Z16:
      trb->Base.DataType = GL_UNSIGNED_SHORT;
      trb->Base._BaseFormat = GL_DEPTH_COMPONENT;
      break;
   case MESA_FORMAT_Z32:
      trb->Base.DataType = GL_UNSIGNED_INT;
      trb->Base._BaseFormat = GL_DEPTH_COMPONENT;
      break;
   /* SRGB formats pre EXT_framebuffer_sRGB don't do sRGB translations on FBO readback */
   case MESA_FORMAT_SRGB8:
      trb->Fetch = _swrast_get_texel_fetch_func(MESA_FORMAT_RGB888);
      trb->Base.DataType = CHAN_TYPE;
      trb->Base._BaseFormat = GL_RGBA;
      break;
   case MESA_FORMAT_SRGBA8:
      trb->Fetch = _swrast_get_texel_fetch_func(MESA_FORMAT_RGBA8888);
      trb->Base.DataType = CHAN_TYPE;
      trb->Base._BaseFormat = GL_RGBA;
      break;
   case MESA_FORMAT_SARGB8:
      trb->Fetch = _swrast_get_texel_fetch_func(MESA_FORMAT_ARGB8888);
      trb->Base.DataType = CHAN_TYPE;
      trb->Base._BaseFormat = GL_RGBA;
      break;
   default:
      trb->Base.DataType = CHAN_TYPE;
      trb->Base._BaseFormat = GL_RGBA;
   }
   trb->Base.Data = NULL;
}



/**
 * Called when rendering to a texture image begins, or when changing
 * the dest mipmap level, cube face, etc.
 * This is a fallback routine for software render-to-texture.
 *
 * Called via the glRenderbufferTexture1D/2D/3D() functions
 * and elsewhere (such as glTexImage2D).
 *
 * The image we're rendering into is
 * att->Texture->Image[att->CubeMapFace][att->TextureLevel];
 * It'll never be NULL.
 *
 * \param fb  the framebuffer object the texture is being bound to
 * \param att  the fb attachment point of the texture
 *
 * \sa _mesa_framebuffer_renderbuffer
 */
void
_swrast_render_texture(struct gl_context *ctx,
                       struct gl_framebuffer *fb,
                       struct gl_renderbuffer_attachment *att)
{
   struct swrast_texture_image *swImg =
      swrast_texture_image(_mesa_get_attachment_teximage(att));
   const GLenum target = swImg->Base.TexObject->Target;
   gl_format format;
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

   format = swImg->Base.TexFormat;
   swImg->TexelSize = _mesa_get_format_bytes(format);
   swImg->Fetch = _swrast_get_texel_fetch_func(format);
   swImg->Store = _swrast_get_texel_store_func(format);

   if (!att->Renderbuffer) {
      wrap_texture(ctx, att);
   }
   update_wrapper(ctx, att);
}


void
_swrast_finish_render_texture(struct gl_context *ctx,
                              struct gl_renderbuffer_attachment *att)
{
   struct gl_texture_image *texImage = _mesa_get_attachment_teximage(att);

   struct swrast_texture_image *swImg = swrast_texture_image(texImage);

   /* just unmap the texture image buffer */
   ctx->Driver.UnmapTextureImage(ctx, texImage, att->Zoffset);
   swImg->SliceMaps[0] = NULL;
}
