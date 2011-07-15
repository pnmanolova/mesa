#include "swrast/swrast.h"
#include "main/renderbuffer.h"
#include "main/texobj.h"
#include "main/teximage.h"
#include "main/mipmap.h"
#include "drivers/common/meta.h"
#include "intel_context.h"
#include "intel_mipmap_tree.h"
#include "intel_tex.h"

#define FILE_DEBUG_FLAG DEBUG_TEXTURE

static struct gl_texture_image *
intelNewTextureImage(struct gl_context * ctx)
{
   DBG("%s\n", __FUNCTION__);
   (void) ctx;
   return (struct gl_texture_image *) CALLOC_STRUCT(intel_texture_image);
}


static struct gl_texture_object *
intelNewTextureObject(struct gl_context * ctx, GLuint name, GLenum target)
{
   struct intel_texture_object *obj = CALLOC_STRUCT(intel_texture_object);

   DBG("%s\n", __FUNCTION__);
   _mesa_initialize_texture_object(&obj->base, name, target);

   return &obj->base;
}

static void 
intelDeleteTextureObject(struct gl_context *ctx,
			 struct gl_texture_object *texObj)
{
   struct intel_context *intel = intel_context(ctx);
   struct intel_texture_object *intelObj = intel_texture_object(texObj);

   if (intelObj->mt)
      intel_miptree_release(intel, &intelObj->mt);

   _mesa_delete_texture_object(ctx, texObj);
}


static GLboolean
intelAllocTextureImageBuffer(struct gl_context *ctx,
                             struct gl_texture_image *texImage,
                             gl_format format, GLsizei width,
                             GLsizei height, GLsizei depth)
{
   struct intel_texture_image *intelImage = intel_texture_image(texImage);

   if (intelImage->ImageOffsets)
      free(intelImage->ImageOffsets);

   intelImage->ImageOffsets = (GLuint *) malloc(depth * sizeof(GLuint));

   return GL_TRUE;
}


static void
intelFreeTextureImageBuffer(struct gl_context * ctx, struct gl_texture_image *texImage)
{
   struct intel_context *intel = intel_context(ctx);
   struct intel_texture_image *intelImage = intel_texture_image(texImage);

   DBG("%s\n", __FUNCTION__);

   if (intelImage->mt) {
      intel_miptree_release(intel, &intelImage->mt);
   }

   if (intelImage->Data) {
      _mesa_align_free(intelImage->Data);
      intelImage->Data = NULL;
   }

   if (intelImage->depth_rb) {
      _mesa_reference_renderbuffer(&intelImage->depth_rb, NULL);
   }

   if (intelImage->stencil_rb) {
      _mesa_reference_renderbuffer(&intelImage->stencil_rb, NULL);
   }
}


/**
 * Map texture memory/buffer into user space.
 * Note: the region of interest parameters are ignored here.
 * \param mapOut  returns start of mapping of region of interest
 * \param rowStrideOut  returns row stride in bytes
 */
static void
intelMapTextureImage(struct gl_context *ctx,
                     struct gl_texture_image *texImage,
                     GLuint slice,
                     GLuint x, GLuint y, GLuint w, GLuint h,
                     GLbitfield mode,
                     GLubyte **mapOut,
                     GLint *rowStrideOut)
{
   struct intel_context *intel = intel_context(ctx);
   struct intel_texture_image *intelImage = intel_texture_image(texImage);
   GLuint texelSize;
   GLint rowStride;

   texelSize = _mesa_get_format_bytes(texImage->TexFormat);

   if (intelImage->mt) {
      /* texture data is stored in a region */
      GLubyte *map = intel_miptree_image_map(intel,
                                             intelImage->mt,
                                             texImage->Face,
                                             texImage->Level,
                                             &rowStride,
                                             intelImage->ImageOffsets);
      *mapOut = map + intelImage->ImageOffsets[slice] * texelSize
         + rowStride * y + x * texelSize;

      *rowStrideOut = rowStride;
   }
   else {
      /* texture data is in malloc'd memory */
      GLubyte *map = intelImage->Data;
      GLuint width = intelImage->base.Base.Width;
      GLuint height = intelImage->base.Base.Height;
      GLint stride = _mesa_format_row_stride(texImage->TexFormat, width);

      assert(map);

      *mapOut = map + slice * stride * height + stride * y + x * texelSize;

      *rowStrideOut = stride;
   }
}


static void
intelUnmapTextureImage(struct gl_context *ctx,
                       struct gl_texture_image *texImage, GLuint slice)
{
   struct intel_context *intel = intel_context(ctx);
   struct intel_texture_image *intelImage = intel_texture_image(texImage);
   intel_region_unmap(intel, intelImage->mt->region);
}



/**
 * Called via ctx->Driver.GenerateMipmap()
 * This is basically a wrapper for _mesa_meta_GenerateMipmap() which checks
 * if we'll be using software mipmap generation.  In that case, we need to
 * map/unmap the base level texture image.
 */
static void
intelGenerateMipmap(struct gl_context *ctx, GLenum target,
                    struct gl_texture_object *texObj)
{
   if (_mesa_meta_check_generate_mipmap_fallback(ctx, target, texObj)) {
      /* sw path: need to map texture images */
#if 0
      struct intel_context *intel = intel_context(ctx);
      struct intel_texture_object *intelObj = intel_texture_object(texObj);
      struct gl_texture_image *first_image = texObj->Image[0][texObj->BaseLevel];

      fallback_debug("%s - fallback to swrast\n", __FUNCTION__);

      intel_tex_map_level_images(intel, intelObj, texObj->BaseLevel);
      _mesa_generate_mipmap(ctx, target, texObj);
      intel_tex_unmap_level_images(intel, intelObj, texObj->BaseLevel);

      if (!_mesa_is_format_compressed(first_image->TexFormat)) {
         GLuint nr_faces = (texObj->Target == GL_TEXTURE_CUBE_MAP) ? 6 : 1;
         GLuint face, i;
         for (face = 0; face < nr_faces; face++) {
            for (i = texObj->BaseLevel + 1; i < texObj->MaxLevel; i++) {
               struct intel_texture_image *intelImage =
                  intel_texture_image(texObj->Image[face][i]);
               if (!intelImage)
                  break;
               /* Unreference the miptree to signal that the new Data is a
                * bare pointer from mesa.
                */
               intel_miptree_release(intel, &intelImage->mt);
            }
         }
      }
#else
      _mesa_generate_mipmap(ctx, target, texObj);
#endif
   }
   else {
      _mesa_meta_GenerateMipmap(ctx, target, texObj);
   }
}


void
intelInitTextureFuncs(struct dd_function_table *functions)
{
   functions->GenerateMipmap = intelGenerateMipmap;

   functions->NewTextureObject = intelNewTextureObject;
   functions->NewTextureImage = intelNewTextureImage;
   functions->DeleteTexture = intelDeleteTextureObject;
   functions->AllocTextureImageBuffer = intelAllocTextureImageBuffer;
   functions->FreeTextureImageBuffer = intelFreeTextureImageBuffer;
   functions->MapTextureImage = intelMapTextureImage;
   functions->UnmapTextureImage = intelUnmapTextureImage;
}
