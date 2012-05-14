/**************************************************************************
 *
 * Copyright 2012 VMware, Inc.
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


#include <stdio.h>

#include "pipe/p_defines.h"
#include "util/u_memory.h"
#include "util/u_prim.h"
#include "util/u_string.h"
#include "draw/draw_vertex.h"
#include "draw/draw_vbuf.h"


struct vbuf_ply_render
{
   struct vbuf_render base;
   struct vbuf_render *render;
   void *vertex_buffer;
   const struct vertex_info *vertex_info;
   unsigned prim;
};


/** cast wrapper */
static struct vbuf_ply_render *
vbuf_ply_render(struct vbuf_render *vbr)
{
   return (struct vbuf_ply_render *) vbr;
}


static const struct vertex_info *
ply_get_vertex_info(struct vbuf_render *vbr)
{
   struct vbuf_ply_render *ply = vbuf_ply_render(vbr);

   ply->vertex_info = ply->render->get_vertex_info(ply->render);
   return ply->vertex_info;
}


static boolean
ply_allocate_vertices(struct vbuf_render *vbr,
                          ushort vertex_size, ushort nr_vertices)
{

   struct vbuf_ply_render *ply = vbuf_ply_render(vbr);
   
   return ply->render->allocate_vertices(ply->render, vertex_size, nr_vertices);
}


static void
ply_release_vertices(struct vbuf_render *vbr)
{
   struct vbuf_ply_render *ply = vbuf_ply_render(vbr);
   
   return ply->render->release_vertices(ply->render);
}


static void *
ply_map_vertices(struct vbuf_render *vbr)
{
   struct vbuf_ply_render *ply = vbuf_ply_render(vbr);
   ply->vertex_buffer = ply->render->map_vertices(ply->render);
   return ply->vertex_buffer;
}


static void 
ply_unmap_vertices(struct vbuf_render *vbr, 
                       ushort min_index,
                       ushort max_index )
{
   struct vbuf_ply_render *ply = vbuf_ply_render(vbr);
   ply->render->unmap_vertices(ply->render, min_index, max_index);
}


static void
ply_set_primitive(struct vbuf_render *vbr, unsigned prim)
{
   vbuf_ply_render(vbr)->prim = prim;
}


typedef const float (*const_float4_ptr)[4];


static INLINE const_float4_ptr get_vert( const void *vertex_buffer,
                                         int index,
                                         int stride )
{
   return (const_float4_ptr)((char *)vertex_buffer + index * stride);
}


static INLINE uint
prims_for_vertices(uint prim, uint nr)
{
   switch (prim) {
   case PIPE_PRIM_POINTS:
      return nr;
   case PIPE_PRIM_LINES:
      return nr / 2;
   case PIPE_PRIM_LINE_STRIP:
      return nr - 1;
   case PIPE_PRIM_LINE_LOOP:
      return nr;
   case PIPE_PRIM_TRIANGLES:
      return nr / 3;
   case PIPE_PRIM_TRIANGLE_STRIP:
   case PIPE_PRIM_TRIANGLE_FAN:
      return nr - 2 ;
   case PIPE_PRIM_POLYGON:
      return 1;
   case PIPE_PRIM_QUADS:
      return nr / 4;
   case PIPE_PRIM_QUAD_STRIP:
      return (nr - 2) / 2;
   default:
      return 0;
   }
}


static unsigned ply_no = 0;

static FILE *
ply_open(struct vbuf_ply_render *ply, const ushort *indices, uint start, uint nr)
{
   const unsigned stride = ply->vertex_info->size * sizeof(float);
   const void *vertex_buffer =
      (void *) get_vert(ply->vertex_buffer, start, stride);
   char filename[256];
   FILE *fp;
   unsigned i;

   util_snprintf(filename, sizeof filename, "%04u.ply", ply_no++);

   fp = fopen(filename, "wt");
   if (!fp) {
      return NULL;
   }

   fprintf(fp, "ply\n");
   fprintf(fp, "format ascii 1.0\n");
   fprintf(fp, "comment %s, prim = %s\n", indices ? "elements" : "arrays", u_prim_name(ply->prim));
   fprintf(fp, "element vertex %u\n", nr);
   fprintf(fp, "property float x\n");
   fprintf(fp, "property float y\n");
   fprintf(fp, "property float z\n");
   fprintf(fp, "element face %u\n", prims_for_vertices(ply->prim, nr));
   fprintf(fp, "property list uchar int vertex_index\n");
   fprintf(fp, "end_header\n");
   for (i = 0; i < nr; ++i) {
      const float (*v)[4] =  get_vert(vertex_buffer, i, stride);
      fprintf(fp, "%.9g %.9g %.9g\n", v[0][0], v[0][1], v[0][2]);
   }

   return fp;
}


/**
 * draw elements / indexed primitives
 */
static void
ply_draw_elements(struct vbuf_render *vbr, const ushort *indices, uint nr)
{
   struct vbuf_ply_render *ply = vbuf_ply_render(vbr);
   FILE *fp;
   unsigned i;

   fp = ply_open(ply, indices, 0, nr);
   if (fp) {

      switch (ply->prim) {
      case PIPE_PRIM_POINTS:
#if 0
         for (i = 0; i < nr; i++) {
            ply->point( ply,
                          get_vert(vertex_buffer, indices[i-0], stride) );
         }
#endif
         break;

      case PIPE_PRIM_LINES:
#if 0
         for (i = 1; i < nr; i += 2) {
            ply->line( ply,
                         get_vert(vertex_buffer, indices[i-1], stride),
                         get_vert(vertex_buffer, indices[i-0], stride) );
         }
#endif
         break;

      case PIPE_PRIM_LINE_STRIP:
#if 0
         for (i = 1; i < nr; i ++) {
            ply->line( ply,
                         get_vert(vertex_buffer, indices[i-1], stride),
                         get_vert(vertex_buffer, indices[i-0], stride) );
         }
#endif
         break;

      case PIPE_PRIM_LINE_LOOP:
#if 0
         for (i = 1; i < nr; i ++) {
            ply->line( ply,
                         get_vert(vertex_buffer, indices[i-1], stride),
                         get_vert(vertex_buffer, indices[i-0], stride) );
         }
         if (nr) {
            ply->line( ply,
                         get_vert(vertex_buffer, indices[nr-1], stride),
                         get_vert(vertex_buffer, indices[0], stride) );
         }
#endif
         break;

      case PIPE_PRIM_TRIANGLES:
#if 0
         for (i = 2; i < nr; i += 3) {
            ply->triangle( ply,
                             get_vert(vertex_buffer, indices[i-2], stride),
                             get_vert(vertex_buffer, indices[i-1], stride),
                             get_vert(vertex_buffer, indices[i-0], stride) );
         }
#endif
         break;

      case PIPE_PRIM_TRIANGLE_STRIP:
         for (i = 2; i < nr; ++i) {
            fprintf(fp, "3 %u %u %u\n", 
                          i-2,
                          i+(i&1)-1,
                          i-(i&1));
         }
         fprintf(fp, "\n");
         break;

      case PIPE_PRIM_TRIANGLE_FAN:
#if 0
         if (flatshade_first) {
            for (i = 2; i < nr; i += 1) {
               /* emit first non-spoke vertex as first vertex */
               ply->triangle( ply,
                                get_vert(vertex_buffer, indices[i-1], stride),
                                get_vert(vertex_buffer, indices[i-0], stride),
                                get_vert(vertex_buffer, indices[0], stride) );
            }
         }
         else {
            for (i = 2; i < nr; i += 1) {
               /* emit last non-spoke vertex as last vertex */
               ply->triangle( ply,
                                get_vert(vertex_buffer, indices[0], stride),
                                get_vert(vertex_buffer, indices[i-1], stride),
                                get_vert(vertex_buffer, indices[i-0], stride) );
            }
         }
#endif
         break;

      case PIPE_PRIM_QUADS:
#if 0
         /* GL quads don't follow provoking vertex convention */
         if (flatshade_first) { 
            /* emit last quad vertex as first triangle vertex */
            for (i = 3; i < nr; i += 4) {
               ply->triangle( ply,
                                get_vert(vertex_buffer, indices[i-0], stride),
                                get_vert(vertex_buffer, indices[i-3], stride),
                                get_vert(vertex_buffer, indices[i-2], stride) );

               ply->triangle( ply,
                                get_vert(vertex_buffer, indices[i-0], stride),
                                get_vert(vertex_buffer, indices[i-2], stride),
                                get_vert(vertex_buffer, indices[i-1], stride) );
            }
         }
         else {
            /* emit last quad vertex as last triangle vertex */
            for (i = 3; i < nr; i += 4) {
               ply->triangle( ply,
                             get_vert(vertex_buffer, indices[i-3], stride),
                             get_vert(vertex_buffer, indices[i-2], stride),
                             get_vert(vertex_buffer, indices[i-0], stride) );

               ply->triangle( ply,
                                get_vert(vertex_buffer, indices[i-2], stride),
                                get_vert(vertex_buffer, indices[i-1], stride),
                                get_vert(vertex_buffer, indices[i-0], stride) );
            }
         }
#endif
         break;

      case PIPE_PRIM_QUAD_STRIP:
#if 0
         /* GL quad strips don't follow provoking vertex convention */
         if (flatshade_first) { 
            /* emit last quad vertex as first triangle vertex */
            for (i = 3; i < nr; i += 2) {
               ply->triangle( ply,
                                get_vert(vertex_buffer, indices[i-0], stride),
                                get_vert(vertex_buffer, indices[i-3], stride),
                                get_vert(vertex_buffer, indices[i-2], stride) );
               ply->triangle( ply,
                                get_vert(vertex_buffer, indices[i-0], stride),
                                get_vert(vertex_buffer, indices[i-1], stride),
                                get_vert(vertex_buffer, indices[i-3], stride) );
            }
         }
         else {
            /* emit last quad vertex as last triangle vertex */
            for (i = 3; i < nr; i += 2) {
               ply->triangle( ply,
                                get_vert(vertex_buffer, indices[i-3], stride),
                                get_vert(vertex_buffer, indices[i-2], stride),
                                get_vert(vertex_buffer, indices[i-0], stride) );
               ply->triangle( ply,
                                get_vert(vertex_buffer, indices[i-1], stride),
                                get_vert(vertex_buffer, indices[i-3], stride),
                                get_vert(vertex_buffer, indices[i-0], stride) );
            }
         }
#endif
         break;

      case PIPE_PRIM_POLYGON:
#if 0
         /* Almost same as tri fan but the _first_ vertex specifies the flat
          * shading color.
          */
         if (flatshade_first) { 
            /* emit first polygon  vertex as first triangle vertex */
            for (i = 2; i < nr; i += 1) {
               ply->triangle( ply,
                                get_vert(vertex_buffer, indices[0], stride),
                                get_vert(vertex_buffer, indices[i-1], stride),
                                get_vert(vertex_buffer, indices[i-0], stride) );
            }
         }
         else {
            /* emit first polygon  vertex as last triangle vertex */
            for (i = 2; i < nr; i += 1) {
               ply->triangle( ply,
                                get_vert(vertex_buffer, indices[i-1], stride),
                                get_vert(vertex_buffer, indices[i-0], stride),
                                get_vert(vertex_buffer, indices[0], stride) );
            }
         }
#endif
         break;

      default:
         assert(0);
      }

      fclose(fp);
   }

   ply->render->draw_elements(ply->render, indices, nr);
}


/**
 * This function is hit when the draw module is working in pass-through mode.
 * It's up to us to convert the vertex array into point/line/tri prims.
 */
static void
ply_draw_arrays(struct vbuf_render *vbr, uint start, uint nr)
{
   struct vbuf_ply_render *ply = vbuf_ply_render(vbr);
   FILE *fp;
   unsigned i;

   fp = ply_open(ply, NULL, start, nr);
   if (fp) {

      switch (ply->prim) {
      case PIPE_PRIM_POINTS:
#if 0
         for (i = 0; i < nr; i++) {
            ply->point( ply,
                          get_vert(vertex_buffer, i-0, stride) );
         }
#endif
         fprintf(fp, "%u", nr); 
         for (i = 0; i < nr; ++i) {
            fprintf(fp, " %u", i);
         }
         fprintf(fp, "\n");
         break;

      case PIPE_PRIM_LINES:
#if 0
         for (i = 1; i < nr; i += 2) {
            ply->line( ply,
                         get_vert(vertex_buffer, i-1, stride),
                         get_vert(vertex_buffer, i-0, stride) );
         }
#endif
         break;

      case PIPE_PRIM_LINE_STRIP:
#if 0
         for (i = 1; i < nr; i ++) {
            ply->line( ply,
                         get_vert(vertex_buffer, i-1, stride),
                         get_vert(vertex_buffer, i-0, stride) );
         }
#endif
         break;

      case PIPE_PRIM_LINE_LOOP:
#if 0
         for (i = 1; i < nr; i ++) {
            ply->line( ply,
                         get_vert(vertex_buffer, i-1, stride),
                         get_vert(vertex_buffer, i-0, stride) );
         }
         if (nr) {
            ply->line( ply,
                         get_vert(vertex_buffer, nr-1, stride),
                         get_vert(vertex_buffer, 0, stride) );
         }
#endif
         break;

      case PIPE_PRIM_TRIANGLES:
#if 0
         for (i = 2; i < nr; i += 3) {
            ply->triangle( ply,
                             get_vert(vertex_buffer, i-2, stride),
                             get_vert(vertex_buffer, i-1, stride),
                             get_vert(vertex_buffer, i-0, stride) );
         }
#endif
         break;

      case PIPE_PRIM_TRIANGLE_STRIP:
#if 0
         if (flatshade_first) {
            for (i = 2; i < nr; i++) {
               /* emit first triangle vertex as first triangle vertex */
               ply->triangle( ply,
                                get_vert(vertex_buffer, i-2, stride),
                                get_vert(vertex_buffer, i+(i&1)-1, stride),
                                get_vert(vertex_buffer, i-(i&1), stride) );
            }
         }
         else {
            for (i = 2; i < nr; i++) {
               /* emit last triangle vertex as last triangle vertex */
               ply->triangle( ply,
                                get_vert(vertex_buffer, i+(i&1)-2, stride),
                                get_vert(vertex_buffer, i-(i&1)-1, stride),
                                get_vert(vertex_buffer, i-0, stride) );
            }
         }
#endif
         break;

      case PIPE_PRIM_TRIANGLE_FAN:
         for (i = 2; i < nr; i += 1) {
            fprintf(fp, "3 %u %u %u\n", 0, i - 1, i);
         }
         break;

      case PIPE_PRIM_QUADS:
#if 0
         /* GL quads don't follow provoking vertex convention */
         if (flatshade_first) { 
            /* emit last quad vertex as first triangle vertex */
            for (i = 3; i < nr; i += 4) {
               ply->triangle( ply,
                                get_vert(vertex_buffer, i-0, stride),
                                get_vert(vertex_buffer, i-3, stride),
                                get_vert(vertex_buffer, i-2, stride) );
               ply->triangle( ply,
                                get_vert(vertex_buffer, i-0, stride),
                                get_vert(vertex_buffer, i-2, stride),
                                get_vert(vertex_buffer, i-1, stride) );
            }
         }
         else {
            /* emit last quad vertex as last triangle vertex */
            for (i = 3; i < nr; i += 4) {
               ply->triangle( ply,
                                get_vert(vertex_buffer, i-3, stride),
                                get_vert(vertex_buffer, i-2, stride),
                                get_vert(vertex_buffer, i-0, stride) );
               ply->triangle( ply,
                                get_vert(vertex_buffer, i-2, stride),
                                get_vert(vertex_buffer, i-1, stride),
                                get_vert(vertex_buffer, i-0, stride) );
            }
         }
#endif
         break;

      case PIPE_PRIM_QUAD_STRIP:
#if 0
         /* GL quad strips don't follow provoking vertex convention */
         if (flatshade_first) { 
            /* emit last quad vertex as first triangle vertex */
            for (i = 3; i < nr; i += 2) {
               ply->triangle( ply,
                                get_vert(vertex_buffer, i-0, stride),
                                get_vert(vertex_buffer, i-3, stride),
                                get_vert(vertex_buffer, i-2, stride) );
               ply->triangle( ply,
                                get_vert(vertex_buffer, i-0, stride),
                                get_vert(vertex_buffer, i-1, stride),
                                get_vert(vertex_buffer, i-3, stride) );
            }
         }
         else {
            /* emit last quad vertex as last triangle vertex */
            for (i = 3; i < nr; i += 2) {
               ply->triangle( ply,
                                get_vert(vertex_buffer, i-3, stride),
                                get_vert(vertex_buffer, i-2, stride),
                                get_vert(vertex_buffer, i-0, stride) );
               ply->triangle( ply,
                                get_vert(vertex_buffer, i-1, stride),
                                get_vert(vertex_buffer, i-3, stride),
                                get_vert(vertex_buffer, i-0, stride) );
            }
         }
#endif
         break;

      case PIPE_PRIM_POLYGON:
         /* Almost same as tri fan but the _first_ vertex specifies the flat
          * shading color.
          */
         fprintf(fp, "%u", nr); 
         for (i = 0; i < nr; ++i) {
            fprintf(fp, " %u", i);
         }
         fprintf(fp, "\n");
         break;

      default:
         assert(0);
      }

      fclose(fp);
   }

   ply->render->draw_arrays(ply->render, start, nr);
}



static void
ply_vbuf_destroy(struct vbuf_render *vbr)
{
   struct vbuf_ply_render *ply = vbuf_ply_render(vbr);
   if (ply->render) {
      ply->render->destroy(ply->render);
   }
   FREE(ply);
}


struct vbuf_render *
draw_vbuf_ply_create(struct vbuf_render *render)
{
   struct vbuf_ply_render *ply;

   ply = CALLOC_STRUCT(vbuf_ply_render);
   if (!ply) {
      return render;
   }

   ply->base.max_indices = render->max_indices;
   ply->base.max_vertex_buffer_bytes = render->max_vertex_buffer_bytes;

   ply->base.get_vertex_info = ply_get_vertex_info;
   ply->base.allocate_vertices = ply_allocate_vertices;
   ply->base.map_vertices = ply_map_vertices;
   ply->base.unmap_vertices = ply_unmap_vertices;
   ply->base.set_primitive = ply_set_primitive;
   ply->base.draw_elements = ply_draw_elements;
   ply->base.draw_arrays = ply_draw_arrays;
   ply->base.release_vertices = ply_release_vertices;
   ply->base.destroy = ply_vbuf_destroy;
   ply->render = render;

   return &ply->base;
}
