/**************************************************************************
 * 
 * Copyright 2003 Tungsten Graphics, Inc., Cedar Park, Texas.
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
 * State validation for vertex/fragment shaders.
 * Note that we have to delay most vertex/fragment shader translation
 * until rendering time since the linkage between the vertex outputs and
 * fragment inputs can vary depending on the pairing of shaders.
 *
 * Authors:
 *   Brian Paul
 */



#include "main/imports.h"
#include "main/mtypes.h"
#include "main/macros.h"
#include "shader/program.h"

#include "pipe/p_context.h"
#include "pipe/p_shader_tokens.h"

#include "util/u_simple_shaders.h"

#include "cso_cache/cso_context.h"

#include "st_context.h"
#include "st_atom.h"
#include "st_program.h"
#include "st_atom_shader.h"
#include "st_mesa_to_tgsi.h"


/**
 * This represents a vertex program, especially translated to match
 * the inputs of a particular fragment shader.
 */
struct translated_vertex_program
{
   struct st_vertex_program *master;

   /** The fragment/geometry shader "signature" this vertex
    * shader is meant for: */
   GLbitfield linked_inputs;

   /** Compared against master vertex program's serialNo: */
   GLuint serialNo;

   /** Maps VERT_RESULT_x to slot */
   GLuint output_to_slot[VERT_RESULT_MAX];
   ubyte output_to_semantic_name[VERT_RESULT_MAX];
   ubyte output_to_semantic_index[VERT_RESULT_MAX];

   /** Pointer to the translated vertex program */
   struct st_vertex_program *vp;

   struct translated_vertex_program *next;  /**< next in linked list */
};

struct translated_geometry_program
{
   struct st_geometry_program *master;

   /** The fragment shader "signature" this vertex
    * shader is meant for: */
   GLbitfield frag_inputs;

   /** Compared against master vertex program's serialNo: */
   GLuint serialNo;

   /** Maps VERT_RESULT_x to slot */
   GLuint output_to_slot[VERT_RESULT_MAX];
   ubyte output_to_semantic_name[VERT_RESULT_MAX];
   ubyte output_to_semantic_index[VERT_RESULT_MAX];

   /** Pointer to the translated vertex program */
   struct st_geometry_program *gp;

   struct translated_geometry_program *next;  /**< next in linked list */
};


/**
 * Given a vertex program output attribute, return the corresponding
 * fragment program input attribute.
 * \return -1 for vertex outputs that have no corresponding fragment input
 */
static GLint
vp_out_to_fp_in(GLuint vertResult)
{
   if (vertResult >= VERT_RESULT_TEX0 &&
       vertResult < VERT_RESULT_TEX0 + MAX_TEXTURE_COORD_UNITS)
      return FRAG_ATTRIB_TEX0 + (vertResult - VERT_RESULT_TEX0);

   if (vertResult >= VERT_RESULT_VAR0 &&
       vertResult < VERT_RESULT_VAR0 + MAX_VARYING)
      return FRAG_ATTRIB_VAR0 + (vertResult - VERT_RESULT_VAR0);

   switch (vertResult) {
   case VERT_RESULT_HPOS:
      return FRAG_ATTRIB_WPOS;
   case VERT_RESULT_COL0:
      return FRAG_ATTRIB_COL0;
   case VERT_RESULT_COL1:
      return FRAG_ATTRIB_COL1;
   case VERT_RESULT_FOGC:
      return FRAG_ATTRIB_FOGC;
   default:
      /* Back-face colors, edge flags, etc */
      return -1;
   }
}

/**
 * Given a geometry program output attribute, return the corresponding
 * fragment program input attribute.
 * \return -1 for geometry outputs that have no corresponding fragment input
 */
static GLint
gp_out_to_fp_in(GLuint geomResult)
{
   if (geomResult >= GEOM_RESULT_TEX0 &&
       geomResult < GEOM_RESULT_TEX0 + MAX_TEXTURE_COORD_UNITS)
      return FRAG_ATTRIB_TEX0 + (geomResult - GEOM_RESULT_TEX0);

   if (geomResult >= GEOM_RESULT_VAR0 &&
       geomResult < GEOM_RESULT_VAR0 + MAX_VARYING - 2)
      return FRAG_ATTRIB_VAR0 + (geomResult - GEOM_RESULT_VAR0);

   switch (geomResult) {
   case GEOM_RESULT_POS:
      return FRAG_ATTRIB_WPOS;
   case GEOM_RESULT_COL0:
      return FRAG_ATTRIB_COL0;
   case GEOM_RESULT_COL1:
      return FRAG_ATTRIB_COL1;
   case GEOM_RESULT_FOGC:
      return FRAG_ATTRIB_FOGC;
   default:
      /* Back-face colors, primitive id, etc */
      return -1;
   }
}

/**
 * Given a vertex program output attribute, return the corresponding
 * geometry program input attribute.
 * \return -1 for vertex outputs that have no corresponding geometry input
 */
static GLint
vp_out_to_gp_in(GLuint vertResult)
{
   if (vertResult >= VERT_RESULT_TEX0 &&
       vertResult < VERT_RESULT_TEX0 + MAX_TEXTURE_COORD_UNITS)
      return GEOM_ATTRIB_TEX_COORD;

   if (vertResult >= VERT_RESULT_VAR0 &&
       vertResult < VERT_RESULT_VAR0 + MAX_VARYING)
      return GEOM_ATTRIB_VAR0 + (vertResult - VERT_RESULT_VAR0);

   switch (vertResult) {
   case VERT_RESULT_HPOS:
      return GEOM_ATTRIB_POSITION;
   case VERT_RESULT_COL0:
      return GEOM_ATTRIB_COLOR0;
   case VERT_RESULT_COL1:
      return GEOM_ATTRIB_COLOR1;
   case VERT_RESULT_FOGC:
      return GEOM_ATTRIB_FOG_FRAG_COORD;
   case VERT_RESULT_PSIZ:
      return GEOM_ATTRIB_POINT_SIZE;
   case VERT_RESULT_BFC0:
      return GEOM_ATTRIB_SECONDARY_COLOR0;
   case VERT_RESULT_BFC1:
      return GEOM_ATTRIB_SECONDARY_COLOR1;
   default:
      /* edge flags, etc */
      return -1;
   }
}

static INLINE int num_vertices_for_prim(int prim)
{
   switch(prim) {
   case PIPE_PRIM_POINTS:
      return 1;
   case PIPE_PRIM_LINES:
      return 2;
   case PIPE_PRIM_LINE_LOOP:
      return 2;
   case PIPE_PRIM_LINE_STRIP:
      return 2;
   case PIPE_PRIM_TRIANGLES:
      return 3;
   case PIPE_PRIM_TRIANGLE_STRIP:
      return 3;
   case PIPE_PRIM_TRIANGLE_FAN:
      return 3;
   case PIPE_PRIM_LINES_ADJACENCY:
   case PIPE_PRIM_LINE_STRIP_ADJACENCY:
      return 4;
   case PIPE_PRIM_TRIANGLES_ADJACENCY:
   case PIPE_PRIM_TRIANGLE_STRIP_ADJACENCY:
      return 6;
   default:
      assert(!"Bad geometry shader input");
      return 0;
   }
}

/**
 * Find a translated vertex program that corresponds to stvp and
 * has outputs matched to stfp's inputs.
 * This performs vertex and fragment translation (to TGSI) when needed.
 */
static struct translated_vertex_program *
find_translated_vp(struct st_context *st,
                   struct st_vertex_program *stvp,
                   struct st_fragment_program *stfp,
                   struct st_geometry_program *stgp)
{
   static const GLuint UNUSED = ~0;
   struct translated_vertex_program *xvp;
   struct translated_geometry_program *xgp;
   const GLbitfield fragInputsRead = stfp->Base.Base.InputsRead;
   GLbitfield geomInputsRead = 0, linkedInputs = fragInputsRead;

   if (stgp) {
      geomInputsRead = stgp->Base.Base.InputsRead;
      linkedInputs = geomInputsRead;
   }

   /*
    * Translate fragment program if needed.
    */
   if (!stfp->state.tokens) {
      GLuint inAttr, numIn = 0;

      for (inAttr = 0; inAttr < FRAG_ATTRIB_MAX; inAttr++) {
         if (fragInputsRead & (1 << inAttr)) {
            stfp->input_to_slot[inAttr] = numIn;
            numIn++;
         }
         else {
            stfp->input_to_slot[inAttr] = UNUSED;
         }
      }

      stfp->num_input_slots = numIn;

      assert(stfp->Base.Base.NumInstructions > 1);

      st_translate_fragment_program(st, stfp, stfp->input_to_slot);
   }

   /* See if we've got a translated geometry program whose outputs match
    * the fragment  program's inputs.
    * XXX This could be a hash lookup, using InputsRead as the key.
    */
   if (stgp) {
      for (xgp = stfp->geometry_programs; xgp; xgp = xgp->next) {
         if (xgp->master == stgp && xgp->frag_inputs == fragInputsRead)
            break;
      }
      /* No?  Allocate translated gp object now */
      if (!xgp) {
         xgp = ST_CALLOC_STRUCT(translated_geometry_program);
         xgp->frag_inputs = fragInputsRead;
         xgp->master = stgp;

         xgp->next = stfp->geometry_programs;
         stfp->geometry_programs = xgp;
      }

      if (!stgp->state.shader.tokens) {
         GLuint inAttr, numIn = 0;

         for (inAttr = 0; inAttr < GEOM_ATTRIB_MAX; inAttr++) {
            if (geomInputsRead & (1 << inAttr)) {
               stgp->input_to_slot[inAttr] = numIn;
               switch(inAttr) {
               case GEOM_ATTRIB_VERTICES:
               case GEOM_ATTRIB_PRIMITIVE_ID:
                  ++numIn;
                  break;
               default:
                  numIn += num_vertices_for_prim(stgp->Base.InputType);
                  break;
               }
#if 0
               debug_printf("%d attr = slot %d\n",
                            inAttr, stgp->input_to_slot[inAttr]);
#endif
            }
            else {
               stgp->input_to_slot[inAttr] = UNUSED;
            }
         }

         stgp->num_input_slots = numIn;
         assert(stgp->Base.Base.NumInstructions > 1);

         if (xgp->serialNo != stgp->serialNo) {
            GLuint outAttr;
            const GLbitfield outputsWritten = stgp->Base.Base.OutputsWritten;
            GLuint numGpOuts = 0;
            GLboolean emitPntSize = GL_FALSE, emitBFC0 = GL_FALSE, emitBFC1 = GL_FALSE;
            GLbitfield usedGenerics = 0x0;
            GLbitfield usedOutputSlots = 0x0;

            /* Compute mapping of geometry program outputs to slots, which depends
             * on the fragment program's input->slot mapping.
             */
            for (outAttr = 0; outAttr < GEOM_RESULT_MAX; outAttr++) {
               /* set defaults: */
               xgp->output_to_slot[outAttr] = UNUSED;
               xgp->output_to_semantic_name[outAttr] = TGSI_SEMANTIC_COUNT;
               xgp->output_to_semantic_index[outAttr] = 99;

               if (outAttr == GEOM_RESULT_POS) {
                  /* always put xformed position into slot zero */
                  GLuint slot = 0;
                  xgp->output_to_slot[GEOM_RESULT_POS] = slot;
                  xgp->output_to_semantic_name[outAttr] = TGSI_SEMANTIC_POSITION;
                  xgp->output_to_semantic_index[outAttr] = 0;
                  numGpOuts++;
                  usedOutputSlots |= (1 << slot);
               }
               else if (outputsWritten & (1 << outAttr)) {
                  /* see if the frag prog wants this geom output */
                  GLint fpInAttrib = gp_out_to_fp_in(outAttr);
                  if (fpInAttrib >= 0) {
                     GLuint fpInSlot = stfp->input_to_slot[fpInAttrib];
                     if (fpInSlot != ~0) {
                        /* match this gp output to the fp input */
                        GLuint gpOutSlot = stfp->input_map[fpInSlot];
                        xgp->output_to_slot[outAttr] = gpOutSlot;
                        xgp->output_to_semantic_name[outAttr] = stfp->input_semantic_name[fpInSlot];
                        xgp->output_to_semantic_index[outAttr] = stfp->input_semantic_index[fpInSlot];
                        numGpOuts++;
                        usedOutputSlots |= (1 << gpOutSlot);
                     }
                     else {
#if 0 /*debug*/
                        printf("GP output %d not used by FP\n", outAttr);
#endif
                     }
                  }
                  else if (outAttr == GEOM_RESULT_PSIZ)
                     emitPntSize = GL_TRUE;
                  else if (outAttr == GEOM_RESULT_SCOL0)
                     emitBFC0 = GL_TRUE;
                  else if (outAttr == GEOM_RESULT_SCOL1)
                     emitBFC1 = GL_TRUE;
               }
#if 0 /*debug*/
               printf("assign gp output_to_slot[%d] = %d\n", outAttr,
                      xgp->output_to_slot[outAttr]);
#endif
            }

            /* must do these last */
            if (emitPntSize) {
               GLuint slot = numGpOuts++;
               xgp->output_to_slot[GEOM_RESULT_PSIZ] = slot;
               xgp->output_to_semantic_name[GEOM_RESULT_PSIZ] = TGSI_SEMANTIC_PSIZE;
               xgp->output_to_semantic_index[GEOM_RESULT_PSIZ] = 0;
               usedOutputSlots |= (1 << slot);
            }
            if (emitBFC0) {
               GLuint slot = numGpOuts++;
               xgp->output_to_slot[GEOM_RESULT_SCOL0] = slot;
               xgp->output_to_semantic_name[GEOM_RESULT_SCOL0] = TGSI_SEMANTIC_COLOR;
               xgp->output_to_semantic_index[GEOM_RESULT_SCOL0] = 0;
               usedOutputSlots |= (1 << slot);
            }
            if (emitBFC1) {
               GLuint slot = numGpOuts++;
               xgp->output_to_slot[GEOM_RESULT_SCOL1] = slot;
               xgp->output_to_semantic_name[GEOM_RESULT_SCOL1] = TGSI_SEMANTIC_COLOR;
               xgp->output_to_semantic_index[GEOM_RESULT_SCOL1] = 1;
               usedOutputSlots |= (1 << slot);
            }

            /* build usedGenerics mask */
            usedGenerics = 0x0;
            for (outAttr = 0; outAttr < GEOM_RESULT_MAX; outAttr++) {
               if (xgp->output_to_semantic_name[outAttr] == TGSI_SEMANTIC_GENERIC) {
                  usedGenerics |= (1 << xgp->output_to_semantic_index[outAttr]);
               }
            }

            /* For each vertex geometry output that doesn't match up to a fragment
             * program input, map the geometry program output to a free slot and
             * free generic attribute.
             */
            for (outAttr = 0; outAttr < GEOM_RESULT_MAX; outAttr++) {
               if (outputsWritten & (1 << outAttr)) {
                  if (xgp->output_to_slot[outAttr] == UNUSED) {
                     GLint freeGeneric = _mesa_ffs(~usedGenerics) - 1;
                     GLint freeSlot = _mesa_ffs(~usedOutputSlots) - 1;
                     usedGenerics |= (1 << freeGeneric);
                     usedOutputSlots |= (1 << freeSlot);
                     xgp->output_to_slot[outAttr] = freeSlot;
                     xgp->output_to_semantic_name[outAttr] = TGSI_SEMANTIC_GENERIC;
                     xgp->output_to_semantic_index[outAttr] = freeGeneric;
                  }
               }

#if 0 /*debug*/
               printf("gp output_to_slot[%d] = %d\n", outAttr,
                      xgp->output_to_slot[outAttr]);
#endif
            }
         }

         st_translate_geometry_program(st, stgp, stgp->input_to_slot,
                                       xgp->output_to_slot,
                                       xgp->output_to_semantic_name,
                                       xgp->output_to_semantic_index);

         xgp->gp = stgp;

         /* translated GP is up to date now */
         xgp->serialNo = stgp->serialNo;
      }
   }
   /* See if we've got a translated vertex program whose outputs match
    * the fragment or geometry program's inputs.
    * XXX This could be a hash lookup, using InputsRead as the key.
    */
#define LP_FUNC(FP, GP) ((stgp)?GP:FP)
#define LP(X) ((stgp)?stgp->X:stfp->X)
#define LP_SET(X, Y) do { if (stgp) stgp->X = Y; else stfp->X = Y; } while(0)

   for (xvp = LP(vertex_programs); xvp; xvp = xvp->next) {
      if (xvp->master == stvp && xvp->linked_inputs == linkedInputs)
         break;
   }

   /* No?  Allocate translated vp object now */
   if (!xvp) {
      xvp = ST_CALLOC_STRUCT(translated_vertex_program);
      xvp->linked_inputs = linkedInputs;
      xvp->master = stvp;

      xvp->next = LP(vertex_programs);
      LP_SET(vertex_programs, xvp);
   }

   /* See if we need to translate vertex program to TGSI form */
   if (xvp->serialNo != stvp->serialNo) {
      GLuint outAttr;
      const GLbitfield outputsWritten = stvp->Base.Base.OutputsWritten;
      GLuint numVpOuts = 0;
      GLboolean emitPntSize = GL_FALSE, emitBFC0 = GL_FALSE, emitBFC1 = GL_FALSE;
      GLbitfield usedGenerics = 0x0;
      GLbitfield usedOutputSlots = 0x0;

      /* Compute mapping of vertex program outputs to slots, which depends
       * on the fragment program's input->slot mapping.
       */
      for (outAttr = 0; outAttr < VERT_RESULT_MAX; outAttr++) {
         /* set defaults: */
         xvp->output_to_slot[outAttr] = UNUSED;
         xvp->output_to_semantic_name[outAttr] = TGSI_SEMANTIC_COUNT;
         xvp->output_to_semantic_index[outAttr] = 99;

         if (outAttr == VERT_RESULT_HPOS) {
            /* always put xformed position into slot zero */
            GLuint slot = 0;
            xvp->output_to_slot[VERT_RESULT_HPOS] = slot;
            xvp->output_to_semantic_name[outAttr] = TGSI_SEMANTIC_POSITION;
            xvp->output_to_semantic_index[outAttr] = 0;
            numVpOuts++;
            usedOutputSlots |= (1 << slot);
         }
         else if (outputsWritten & (1 << outAttr)) {
            /* see if the frag prog wants this vert output */
            GLint lpInAttrib = LP_FUNC(vp_out_to_fp_in(outAttr), vp_out_to_gp_in(outAttr));
            if (lpInAttrib >= 0) {
               GLuint lpInSlot = LP(input_to_slot[lpInAttrib]);
               if (lpInSlot != ~0) {
                  /* match this vp output to the lp input */
                  GLuint vpOutSlot = LP(input_map[lpInSlot]);
                  xvp->output_to_slot[outAttr] = vpOutSlot;
                  xvp->output_to_semantic_name[outAttr] = LP(input_semantic_name[lpInSlot]);
                  xvp->output_to_semantic_index[outAttr] = LP(input_semantic_index[lpInSlot]);
                  numVpOuts++;
                  usedOutputSlots |= (1 << vpOutSlot);
               }
               else {
#if 0 /*debug*/
                  printf("VP output %d not used by LP\n", outAttr);
#endif
               }
            }
            else if (outAttr == VERT_RESULT_PSIZ)
               emitPntSize = GL_TRUE;
            else if (outAttr == VERT_RESULT_BFC0)
               emitBFC0 = GL_TRUE;
            else if (outAttr == VERT_RESULT_BFC1)
               emitBFC1 = GL_TRUE;
         }
#if 0 /*debug*/
         printf("assign vp output_to_slot[%d] = %d\n", outAttr, 
                xvp->output_to_slot[outAttr]);
#endif
      }

      /* must do these last */
      if (emitPntSize) {
         GLuint slot = numVpOuts++;
         xvp->output_to_slot[VERT_RESULT_PSIZ] = slot;
         xvp->output_to_semantic_name[VERT_RESULT_PSIZ] = TGSI_SEMANTIC_PSIZE;
         xvp->output_to_semantic_index[VERT_RESULT_PSIZ] = 0;
         usedOutputSlots |= (1 << slot);
      }
      if (emitBFC0) {
         GLuint slot = numVpOuts++;
         xvp->output_to_slot[VERT_RESULT_BFC0] = slot;
         xvp->output_to_semantic_name[VERT_RESULT_BFC0] = TGSI_SEMANTIC_COLOR;
         xvp->output_to_semantic_index[VERT_RESULT_BFC0] = 0;
         usedOutputSlots |= (1 << slot);
      }
      if (emitBFC1) {
         GLuint slot = numVpOuts++;
         xvp->output_to_slot[VERT_RESULT_BFC1] = slot;
         xvp->output_to_semantic_name[VERT_RESULT_BFC1] = TGSI_SEMANTIC_COLOR;
         xvp->output_to_semantic_index[VERT_RESULT_BFC1] = 1;
         usedOutputSlots |= (1 << slot);
      }

      /* build usedGenerics mask */
      usedGenerics = 0x0;
      for (outAttr = 0; outAttr < VERT_RESULT_MAX; outAttr++) {
         if (xvp->output_to_semantic_name[outAttr] == TGSI_SEMANTIC_GENERIC) {
            usedGenerics |= (1 << xvp->output_to_semantic_index[outAttr]);
         }
      }

      /* For each vertex program output that doesn't match up to a fragment
       * program input, map the vertex program output to a free slot and
       * free generic attribute.
       */
      for (outAttr = 0; outAttr < VERT_RESULT_MAX; outAttr++) {
         if (outputsWritten & (1 << outAttr)) {
            if (xvp->output_to_slot[outAttr] == UNUSED) {
               GLint freeGeneric = _mesa_ffs(~usedGenerics) - 1;
               GLint freeSlot = _mesa_ffs(~usedOutputSlots) - 1;
               usedGenerics |= (1 << freeGeneric);
               usedOutputSlots |= (1 << freeSlot);
               xvp->output_to_slot[outAttr] = freeSlot;
               xvp->output_to_semantic_name[outAttr] = TGSI_SEMANTIC_GENERIC;
               xvp->output_to_semantic_index[outAttr] = freeGeneric;
            }
         }

#if 0 /*debug*/
         printf("vp output_to_slot[%d] = %d\n", outAttr, 
                xvp->output_to_slot[outAttr]);
#endif
      }

      assert(stvp->Base.Base.NumInstructions > 1);

      st_translate_vertex_program(st, stvp, xvp->output_to_slot,
                                  xvp->output_to_semantic_name,
                                  xvp->output_to_semantic_index);

      xvp->vp = stvp;

      /* translated VP is up to date now */
      xvp->serialNo = stvp->serialNo;
   }

   return xvp;
}


void
st_free_translated_vertex_programs(struct st_context *st,
                                   struct translated_vertex_program *xvp)
{
   struct translated_vertex_program *next;

   while (xvp) {
      next = xvp->next;
      _mesa_free(xvp);
      xvp = next;
   }
}


static void *
get_passthrough_fs(struct st_context *st)
{
   if (!st->passthrough_fs) {
      st->passthrough_fs =
         util_make_fragment_passthrough_shader(st->pipe);
   }

   return st->passthrough_fs;
}


static void
update_linkage( struct st_context *st )
{
   struct st_vertex_program *stvp;
   struct st_fragment_program *stfp;
   struct st_geometry_program *stgp = NULL;
   struct translated_vertex_program *xvp;

   /* find active shader and params -- Should be covered by
    * ST_NEW_VERTEX_PROGRAM
    */
   assert(st->ctx->VertexProgram._Current);
   stvp = st_vertex_program(st->ctx->VertexProgram._Current);
   assert(stvp->Base.Base.Target == GL_VERTEX_PROGRAM_ARB);

   assert(st->ctx->FragmentProgram._Current);
   stfp = st_fragment_program(st->ctx->FragmentProgram._Current);
   assert(stfp->Base.Base.Target == GL_FRAGMENT_PROGRAM_ARB);

   if (st->ctx->GeometryProgram._Current) {
      stgp = st_geometry_program(st->ctx->GeometryProgram._Current);
      assert(stgp->Base.Base.Target == MESA_GEOMETRY_PROGRAM);
   }

   xvp = find_translated_vp(st, stvp, stfp, stgp);

   st_reference_vertprog(st, &st->vp, stvp);
   st_reference_fragprog(st, &st->fp, stfp);
   st_reference_geomprog(st, &st->gp, stgp);

   cso_set_vertex_shader_handle(st->cso_context, stvp->driver_shader);

   if (st->missing_textures) {
      /* use a pass-through frag shader that uses no textures */
      void *fs = get_passthrough_fs(st);
      cso_set_fragment_shader_handle(st->cso_context, fs);
   }
   else {
      cso_set_fragment_shader_handle(st->cso_context, stfp->driver_shader);
   }
   if (stgp && stgp->driver_shader)
      cso_set_geometry_shader_handle(st->cso_context, stgp->driver_shader);

   st->vertex_result_to_slot = xvp->output_to_slot;
}


const struct st_tracked_state st_update_shader = {
   "st_update_shader",					/* name */
   {							/* dirty */
      0,						/* mesa */
      ST_NEW_VERTEX_PROGRAM | ST_NEW_FRAGMENT_PROGRAM | ST_NEW_GEOMETRY_PROGRAM	/* st */
   },
   update_linkage					/* update */
};
