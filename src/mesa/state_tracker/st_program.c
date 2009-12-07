/**************************************************************************
 * 
 * Copyright 2007 Tungsten Graphics, Inc., Cedar Park, Texas.
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
 /*
  * Authors:
  *   Keith Whitwell <keith@tungstengraphics.com>
  *   Brian Paul
  */


#include "main/imports.h"
#include "main/mtypes.h"
#include "shader/prog_print.h"
#include "shader/programopt.h"

#include "pipe/p_context.h"
#include "pipe/p_defines.h"
#include "pipe/p_shader_tokens.h"
#include "draw/draw_context.h"
#include "tgsi/tgsi_dump.h"

#include "st_debug.h"
#include "st_context.h"
#include "st_atom.h"
#include "st_program.h"
#include "st_mesa_to_tgsi.h"
#include "cso_cache/cso_context.h"

   /* Clean out any old compilations:
    */
void
st_vp_release_varients( struct st_context *st,
                        struct st_vertex_program *stvp )
{
   struct st_vp_varient *vpv;

   for (vpv = stvp->varients; vpv; ) {
      struct st_vp_varient *next = vpv->next;

      if (vpv->driver_shader) 
         cso_delete_vertex_shader(st->cso_context, vpv->driver_shader);
      
      if (vpv->draw_shader)
         draw_delete_vertex_shader( st->draw, vpv->draw_shader );
      
      if (vpv->state.tokens)
         st_free_tokens(vpv->state.tokens);
      
      FREE( vpv );

      vpv = next;
   }

   stvp->varients = NULL;
}




/**
 * Translate a Mesa vertex shader into a TGSI shader.
 * \param outputMapping  to map vertex program output registers (VERT_RESULT_x)
 *       to TGSI output slots
 * \param tokensOut  destination for TGSI tokens
 * \return  pointer to cached pipe_shader object.
 */
void
st_prepare_vertex_program(struct st_context *st,
                            struct st_vertex_program *stvp)
{
   GLuint attr;

   stvp->num_inputs = 0;
   stvp->num_outputs = 0;

   if (stvp->Base.IsPositionInvariant)
      _mesa_insert_mvp_code(st->ctx, &stvp->Base);

   /*
    * Determine number of inputs, the mappings between VERT_ATTRIB_x
    * and TGSI generic input indexes, plus input attrib semantic info.
    */
   for (attr = 0; attr < VERT_ATTRIB_MAX; attr++) {
      if (stvp->Base.Base.InputsRead & (1 << attr)) {
         stvp->input_to_index[attr] = stvp->num_inputs;
         stvp->index_to_input[stvp->num_inputs] = attr;
         stvp->num_inputs++;
      }
   }

   /* Compute mapping of vertex program outputs to slots.
    */
   for (attr = 0; attr < VERT_RESULT_MAX; attr++) {
      if ((stvp->Base.Base.OutputsWritten & (1 << attr)) == 0) {
         stvp->result_to_output[attr] = ~0;
      }
      else {
         unsigned slot = stvp->num_outputs++;

         stvp->result_to_output[attr] = slot;

         switch (attr) {
         case VERT_RESULT_HPOS:
            stvp->output_semantic_name[slot] = TGSI_SEMANTIC_POSITION;
            stvp->output_semantic_index[slot] = 0;
            break;
         case VERT_RESULT_COL0:
            stvp->output_semantic_name[slot] = TGSI_SEMANTIC_COLOR;
            stvp->output_semantic_index[slot] = 0;
            break;
         case VERT_RESULT_COL1:
            stvp->output_semantic_name[slot] = TGSI_SEMANTIC_COLOR;
            stvp->output_semantic_index[slot] = 1;
            break;
         case VERT_RESULT_BFC0:
            stvp->output_semantic_name[slot] = TGSI_SEMANTIC_BCOLOR;
            stvp->output_semantic_index[slot] = 0;
            break;
         case VERT_RESULT_BFC1:
            stvp->output_semantic_name[slot] = TGSI_SEMANTIC_BCOLOR;
            stvp->output_semantic_index[slot] = 1;
            break;
         case VERT_RESULT_FOGC:
            stvp->output_semantic_name[slot] = TGSI_SEMANTIC_FOG;
            stvp->output_semantic_index[slot] = 0;
            break;
         case VERT_RESULT_PSIZ:
            stvp->output_semantic_name[slot] = TGSI_SEMANTIC_PSIZE;
            stvp->output_semantic_index[slot] = 0;
            break;
         case VERT_RESULT_EDGE:
            assert(0);
            break;

         case VERT_RESULT_TEX0:
         case VERT_RESULT_TEX1:
         case VERT_RESULT_TEX2:
         case VERT_RESULT_TEX3:
         case VERT_RESULT_TEX4:
         case VERT_RESULT_TEX5:
         case VERT_RESULT_TEX6:
         case VERT_RESULT_TEX7:
            stvp->output_semantic_name[slot] = TGSI_SEMANTIC_GENERIC;
            stvp->output_semantic_index[slot] = attr - VERT_RESULT_TEX0;
            break;

         case VERT_RESULT_VAR0:
         default:
            assert(attr < VERT_RESULT_MAX);
            stvp->output_semantic_name[slot] = TGSI_SEMANTIC_GENERIC;
            stvp->output_semantic_index[slot] = (FRAG_ATTRIB_VAR0 - 
                                                FRAG_ATTRIB_TEX0 +
                                                attr - 
                                                VERT_RESULT_VAR0);
            break;
         }
      }
   }
}


struct st_vp_varient *
st_translate_vertex_program(struct st_context *st,
                            struct st_vertex_program *stvp,
                            const struct st_vp_varient_key *key)
{
   struct st_vp_varient *vpv = CALLOC_STRUCT(st_vp_varient);
   struct pipe_context *pipe = st->pipe;

   vpv->state.tokens = 
      st_translate_mesa_program(st->ctx,
                                TGSI_PROCESSOR_VERTEX,
                                &stvp->Base.Base,
                                /* inputs */
                                stvp->num_inputs,
                                stvp->input_to_index,
                                NULL, /* input semantic name */
                                NULL, /* input semantic index */
                                NULL,
                                /* outputs */
                                stvp->num_outputs,
                                stvp->result_to_output,
                                stvp->output_semantic_name,
                                stvp->output_semantic_index );

   vpv->driver_shader = pipe->create_vs_state(pipe, &vpv->state);

   if ((ST_DEBUG & DEBUG_TGSI) && (ST_DEBUG & DEBUG_MESA)) {
      _mesa_print_program(&stvp->Base.Base);
      debug_printf("\n");
   }

   if (ST_DEBUG & DEBUG_TGSI) {
      tgsi_dump( vpv->state.tokens, 0 );
      debug_printf("\n");
   }

   return vpv;
}



/**
 * Translate a Mesa fragment shader into a TGSI shader.
 * \param inputMapping  to map fragment program input registers to TGSI
 *                      input slots
 * \return  pointer to cached pipe_shader object.
 */
void
st_translate_fragment_program(struct st_context *st,
                              struct st_fragment_program *stfp,
                              const GLuint inputMapping[])
{
   struct pipe_context *pipe = st->pipe;
   GLuint outputMapping[FRAG_RESULT_MAX];
   GLuint defaultInputMapping[FRAG_ATTRIB_MAX];
   GLuint interpMode[16];  /* XXX size? */
   GLuint attr;
   const GLbitfield inputsRead = stfp->Base.Base.InputsRead;
   GLuint vslot = 0;

   uint fs_num_inputs = 0;

   ubyte fs_output_semantic_name[PIPE_MAX_SHADER_OUTPUTS];
   ubyte fs_output_semantic_index[PIPE_MAX_SHADER_OUTPUTS];
   uint fs_num_outputs = 0;

   /* which vertex output goes to the first fragment input: */
   if (inputsRead & FRAG_BIT_WPOS)
      vslot = 0;
   else
      vslot = 1;

   /*
    * Convert Mesa program inputs to TGSI input register semantics.
    */
   for (attr = 0; attr < FRAG_ATTRIB_MAX; attr++) {
      if (inputsRead & (1 << attr)) {
         const GLuint slot = fs_num_inputs;

         defaultInputMapping[attr] = slot;

         stfp->input_map[slot] = vslot++;

         fs_num_inputs++;

         switch (attr) {
         case FRAG_ATTRIB_WPOS:
            stfp->input_semantic_name[slot] = TGSI_SEMANTIC_POSITION;
            stfp->input_semantic_index[slot] = 0;
            interpMode[slot] = TGSI_INTERPOLATE_LINEAR;
            break;
         case FRAG_ATTRIB_COL0:
            stfp->input_semantic_name[slot] = TGSI_SEMANTIC_COLOR;
            stfp->input_semantic_index[slot] = 0;
            interpMode[slot] = TGSI_INTERPOLATE_LINEAR;
            break;
         case FRAG_ATTRIB_COL1:
            stfp->input_semantic_name[slot] = TGSI_SEMANTIC_COLOR;
            stfp->input_semantic_index[slot] = 1;
            interpMode[slot] = TGSI_INTERPOLATE_LINEAR;
            break;
         case FRAG_ATTRIB_FOGC:
            stfp->input_semantic_name[slot] = TGSI_SEMANTIC_FOG;
            stfp->input_semantic_index[slot] = 0;
            interpMode[slot] = TGSI_INTERPOLATE_PERSPECTIVE;
            break;
         case FRAG_ATTRIB_FACE:
            stfp->input_semantic_name[slot] = TGSI_SEMANTIC_FACE;
            stfp->input_semantic_index[slot] = 0;
            interpMode[slot] = TGSI_INTERPOLATE_CONSTANT;
            break;

            /* In most cases, there is nothing special about these
             * inputs, so adopt a convention to use the generic
             * semantic name and the mesa FRAG_ATTRIB_ number as the
             * index. 
             * 
             * All that is required is that the vertex shader labels
             * its own outputs similarly, and that the vertex shader
             * generates at least every output required by the
             * fragment shader plus fixed-function hardware (such as
             * BFC).
             * 
             * There is no requirement that semantic indexes start at
             * zero or be restricted to a particular range -- nobody
             * should be building tables based on semantic index.
             */
         case FRAG_ATTRIB_TEX0:
         case FRAG_ATTRIB_TEX1:
         case FRAG_ATTRIB_TEX2:
         case FRAG_ATTRIB_TEX3:
         case FRAG_ATTRIB_TEX4:
         case FRAG_ATTRIB_TEX5:
         case FRAG_ATTRIB_TEX6:
         case FRAG_ATTRIB_TEX7:
         case FRAG_ATTRIB_PNTC:
         case FRAG_ATTRIB_VAR0:
         default:
            /* Actually, let's try and zero-base this just for
             * readability of the generated TGSI.
             */
            assert(attr >= FRAG_ATTRIB_TEX0);
            stfp->input_semantic_index[slot] = (attr - FRAG_ATTRIB_TEX0);
            stfp->input_semantic_name[slot] = TGSI_SEMANTIC_GENERIC;
            interpMode[slot] = TGSI_INTERPOLATE_PERSPECTIVE;
            break;
         }
      }
   }

   /*
    * Semantics and mapping for outputs
    */
   {
      uint numColors = 0;
      GLbitfield64 outputsWritten = stfp->Base.Base.OutputsWritten;

      /* if z is written, emit that first */
      if (outputsWritten & (1 << FRAG_RESULT_DEPTH)) {
         fs_output_semantic_name[fs_num_outputs] = TGSI_SEMANTIC_POSITION;
         fs_output_semantic_index[fs_num_outputs] = 0;
         outputMapping[FRAG_RESULT_DEPTH] = fs_num_outputs;
         fs_num_outputs++;
         outputsWritten &= ~(1 << FRAG_RESULT_DEPTH);
      }

      /* handle remaning outputs (color) */
      for (attr = 0; attr < FRAG_RESULT_MAX; attr++) {
         if (outputsWritten & (1 << attr)) {
            switch (attr) {
            case FRAG_RESULT_DEPTH:
               /* handled above */
               assert(0);
               break;
            default:
               assert(attr == FRAG_RESULT_COLOR ||
                      (FRAG_RESULT_DATA0 <= attr && attr < FRAG_RESULT_MAX));
               fs_output_semantic_name[fs_num_outputs] = TGSI_SEMANTIC_COLOR;
               fs_output_semantic_index[fs_num_outputs] = numColors;
               outputMapping[attr] = fs_num_outputs;
               numColors++;
               break;
            }

            fs_num_outputs++;
         }
      }
   }

   if (!inputMapping)
      inputMapping = defaultInputMapping;

   stfp->state.tokens = 
      st_translate_mesa_program(st->ctx,
                                TGSI_PROCESSOR_FRAGMENT,
                                &stfp->Base.Base,
                                /* inputs */
                                fs_num_inputs,
                                inputMapping,
                                stfp->input_semantic_name,
                                stfp->input_semantic_index,
                                interpMode,
                                /* outputs */
                                fs_num_outputs,
                                outputMapping,
                                fs_output_semantic_name,
                                fs_output_semantic_index );

   stfp->driver_shader = pipe->create_fs_state(pipe, &stfp->state);

   if ((ST_DEBUG & DEBUG_TGSI) && (ST_DEBUG & DEBUG_MESA)) {
      _mesa_print_program(&stfp->Base.Base);
      debug_printf("\n");
   }

   if (ST_DEBUG & DEBUG_TGSI) {
      tgsi_dump( stfp->state.tokens, 0/*TGSI_DUMP_VERBOSE*/ );
      debug_printf("\n");
   }
}

void
st_prepare_geometry_program(struct st_context *st,
                            struct st_geometry_program *stgp)
{
   GLuint attr;

   stgp->num_inputs = 0;
   stgp->num_outputs = 0;

/*
 * Determine number of inputs, the mappings between VERT_ATTRIB_x
 * and TGSI generic input indexes, plus input attrib semantic info.
 */
   for (attr = 0; attr < GEOM_ATTRIB_MAX; attr++) {
      if (stgp->Base.Base.InputsRead & (1 << attr)) {
         stgp->input_to_index[attr] = stgp->num_inputs;
         stgp->index_to_input[stgp->num_inputs] = attr;
         stgp->num_inputs++;
      }
   }

   /* Compute mapping of vertex program outputs to slots.
    */
   for (attr = 0; attr < GEOM_RESULT_MAX; attr++) {
      if ((stgp->Base.Base.OutputsWritten & (1 << attr)) == 0) {
         stgp->result_to_output[attr] = ~0;
      }
      else {
         unsigned slot = stgp->num_outputs++;

         stgp->result_to_output[attr] = slot;

         switch (attr) {
         case GEOM_RESULT_HPOS:
            stgp->output_semantic_name[slot] = TGSI_SEMANTIC_POSITION;
            stgp->output_semantic_index[slot] = 0;
            break;
         case GEOM_RESULT_COL0:
            stgp->output_semantic_name[slot] = TGSI_SEMANTIC_COLOR;
            stgp->output_semantic_index[slot] = 0;
            break;
         case GEOM_RESULT_COL1:
            stgp->output_semantic_name[slot] = TGSI_SEMANTIC_COLOR;
            stgp->output_semantic_index[slot] = 1;
            break;
         case GEOM_RESULT_BFC0:
            stgp->output_semantic_name[slot] = TGSI_SEMANTIC_BCOLOR;
            stgp->output_semantic_index[slot] = 0;
            break;
         case GEOM_RESULT_BFC1:
            stgp->output_semantic_name[slot] = TGSI_SEMANTIC_BCOLOR;
            stgp->output_semantic_index[slot] = 1;
            break;
         case GEOM_RESULT_FOGC:
            stgp->output_semantic_name[slot] = TGSI_SEMANTIC_FOG;
            stgp->output_semantic_index[slot] = 0;
            break;
         case GEOM_RESULT_PSIZ:
            stgp->output_semantic_name[slot] = TGSI_SEMANTIC_PSIZE;
            stgp->output_semantic_index[slot] = 0;
            break;
         case GEOM_RESULT_EDGE:
            assert(0);
            break;

         case GEOM_RESULT_TEX0:
         case GEOM_RESULT_TEX1:
         case GEOM_RESULT_TEX2:
         case GEOM_RESULT_TEX3:
         case GEOM_RESULT_TEX4:
         case GEOM_RESULT_TEX5:
         case GEOM_RESULT_TEX6:
         case GEOM_RESULT_TEX7:
            stgp->output_semantic_name[slot] = TGSI_SEMANTIC_GENERIC;
            stgp->output_semantic_index[slot] = attr - GEOM_RESULT_TEX0;
            break;

         case GEOM_RESULT_VAR0:
         default:
            assert(attr < GEOM_RESULT_MAX);
            stgp->output_semantic_name[slot] = TGSI_SEMANTIC_GENERIC;
            stgp->output_semantic_index[slot] = (FRAG_ATTRIB_VAR0 -
                                                 FRAG_ATTRIB_TEX0 +
                                                 attr -
                                                 GEOM_RESULT_VAR0);
            break;
         }
      }
   }
}

void
st_translate_geometry_program(struct st_context *st,
                              struct st_geometry_program *stgp,
                              const GLuint inputMapping[],
                              const GLuint outputMapping[],
                              const ubyte *outputSemanticName,
                              const ubyte *outputSemanticIndex)
{
   struct pipe_context *pipe = st->pipe;
   GLuint defaultInputMapping[GEOM_ATTRIB_MAX];
   GLuint defaultOutputMapping[GEOM_RESULT_MAX];
   struct pipe_geometry_shader_state gs;
   GLuint attr;
   const GLbitfield inputsRead = stgp->Base.Base.InputsRead;
   GLuint vslot = 0;
   GLuint num_generic = 0;

   uint gs_num_inputs = 0;
   uint gs_builtin_inputs = 0;
   uint gs_array_offset = 0;

   ubyte gs_output_semantic_name[PIPE_MAX_SHADER_OUTPUTS];
   ubyte gs_output_semantic_index[PIPE_MAX_SHADER_OUTPUTS];
   uint gs_num_outputs = 0;

   GLbitfield input_flags[MAX_PROGRAM_INPUTS];
   GLbitfield output_flags[MAX_PROGRAM_OUTPUTS];

   GLint i;

   memset(&gs, 0, sizeof(gs));
   memset(input_flags, 0, sizeof(input_flags));
   memset(output_flags, 0, sizeof(output_flags));

   /* which vertex output goes to the first geometry input */
   if (inputsRead & GEOM_BIT_VERTICES)
      vslot = 0;
   else
      vslot = 1;

   /*
    * Convert Mesa program inputs to TGSI input register semantics.
    */
   for (attr = 0; attr < GEOM_ATTRIB_MAX; attr++) {
      if (inputsRead & (1 << attr)) {
         const GLuint slot = gs_num_inputs;

         gs_num_inputs++;

         defaultInputMapping[attr] = slot;

         stgp->input_map[slot + gs_array_offset] = vslot - gs_builtin_inputs;
         stgp->input_to_index[attr] = vslot;
         stgp->index_to_input[vslot] = attr;
         ++vslot;

         if (attr != GEOM_ATTRIB_VERTICES &&
             attr != GEOM_ATTRIB_PRIMITIVE_ID) {
            gs_array_offset += 2;
         } else
            ++gs_builtin_inputs;

#if 1
         debug_printf("input map at %d = %d\n",
                      slot + gs_array_offset, stgp->input_map[slot + gs_array_offset]);
#endif

         switch (attr) {
         case GEOM_ATTRIB_VERTICES:
            stgp->input_semantic_name[slot] = TGSI_SEMANTIC_VERTICES;
            stgp->input_semantic_index[slot] = 0;
            break;
         case GEOM_ATTRIB_PRIMITIVE_ID:
            stgp->input_semantic_name[slot] = TGSI_SEMANTIC_PRIMID;
            stgp->input_semantic_index[slot] = 0;
            break;
         case GEOM_ATTRIB_POSITION:
            stgp->input_semantic_name[slot] = TGSI_SEMANTIC_POSITION;
            stgp->input_semantic_index[slot] = 0;
            break;
         case GEOM_ATTRIB_COLOR0:
            stgp->input_semantic_name[slot] = TGSI_SEMANTIC_COLOR;
            stgp->input_semantic_index[slot] = 0;
            break;
         case GEOM_ATTRIB_COLOR1:
            stgp->input_semantic_name[slot] = TGSI_SEMANTIC_COLOR;
            stgp->input_semantic_index[slot] = 1;
            break;
         case GEOM_ATTRIB_FOG_FRAG_COORD:
            stgp->input_semantic_name[slot] = TGSI_SEMANTIC_FOG;
            stgp->input_semantic_index[slot] = 0;
            break;
         case GEOM_ATTRIB_TEX_COORD:
            stgp->input_semantic_name[slot] = TGSI_SEMANTIC_GENERIC;
            stgp->input_semantic_index[slot] = num_generic++;
            break;
         case GEOM_ATTRIB_VAR0:
            /* fall-through */
         default:
            stgp->input_semantic_name[slot] = TGSI_SEMANTIC_GENERIC;
            stgp->input_semantic_index[slot] = num_generic++;
         }

         input_flags[slot] = stgp->Base.Base.InputFlags[attr];
      }
   }

#if 0
   if (outputMapping && outputSemanticName) {
      printf("GEOM_RESULT  written  out_slot  semantic_name  semantic_index\n");
      for (attr = 0; attr < GEOM_RESULT_MAX; attr++) {
         printf("    %-2d          %c       %3d          %2d              %2d\n",
                attr,
                ((stgp->Base.Base.OutputsWritten & (1 << attr)) ? 'Y' : ' '),
                outputMapping[attr],
                outputSemanticName[attr],
                outputSemanticIndex[attr]);
      }
   }
#endif

   /* initialize output semantics to defaults */
   for (i = 0; i < PIPE_MAX_SHADER_OUTPUTS; i++) {
      gs_output_semantic_name[i] = TGSI_SEMANTIC_GENERIC;
      gs_output_semantic_index[i] = 0;
      output_flags[i] = 0x0;
   }

   num_generic = 0;
   /*
    * Determine number of outputs, the (default) output register
    * mapping and the semantic information for each output.
    */
   for (attr = 0; attr < GEOM_RESULT_MAX; attr++) {
      if (stgp->Base.Base.OutputsWritten & (1 << attr)) {
         GLuint slot;

         /* XXX
          * Pass in the fragment program's input's semantic info.
          * Use the generic semantic indexes from there, instead of
          * guessing below.
          */
         if (outputMapping) {
            slot = outputMapping[attr];
            assert(slot != ~0);
         }
         else {
            slot = gs_num_outputs;
            gs_num_outputs++;
            defaultOutputMapping[attr] = slot;
         }

         switch (attr) {
         case GEOM_RESULT_POS:
            assert(slot == 0);
            gs_output_semantic_name[slot] = TGSI_SEMANTIC_POSITION;
            gs_output_semantic_index[slot] = 0;
            break;
         case GEOM_RESULT_COL0:
            gs_output_semantic_name[slot] = TGSI_SEMANTIC_COLOR;
            gs_output_semantic_index[slot] = 0;
            break;
         case GEOM_RESULT_COL1:
            gs_output_semantic_name[slot] = TGSI_SEMANTIC_COLOR;
            gs_output_semantic_index[slot] = 1;
            break;
         case GEOM_RESULT_SCOL0:
            gs_output_semantic_name[slot] = TGSI_SEMANTIC_BCOLOR;
            gs_output_semantic_index[slot] = 0;
            break;
         case GEOM_RESULT_SCOL1:
            gs_output_semantic_name[slot] = TGSI_SEMANTIC_BCOLOR;
            gs_output_semantic_index[slot] = 1;
            break;
         case GEOM_RESULT_FOGC:
            gs_output_semantic_name[slot] = TGSI_SEMANTIC_FOG;
            gs_output_semantic_index[slot] = 0;
            break;
         case GEOM_RESULT_PSIZ:
            gs_output_semantic_name[slot] = TGSI_SEMANTIC_PSIZE;
            gs_output_semantic_index[slot] = 0;
            break;
         case GEOM_RESULT_TEX0:
         case GEOM_RESULT_TEX1:
         case GEOM_RESULT_TEX2:
         case GEOM_RESULT_TEX3:
         case GEOM_RESULT_TEX4:
         case GEOM_RESULT_TEX5:
         case GEOM_RESULT_TEX6:
         case GEOM_RESULT_TEX7:
            /* fall-through */
         case GEOM_RESULT_VAR0:
            /* fall-through */
         default:
            assert(slot < Elements(gs_output_semantic_name));
            if (outputSemanticName) {
               /* use provided semantic into */
               assert(outputSemanticName[attr] != TGSI_SEMANTIC_COUNT);
               gs_output_semantic_name[slot] = outputSemanticName[attr];
               gs_output_semantic_index[slot] = outputSemanticIndex[attr];
            }
            else {
               /* use default semantic info */
               gs_output_semantic_name[slot] = TGSI_SEMANTIC_GENERIC;
               gs_output_semantic_index[slot] = num_generic++;
            }
         }

         assert(slot < Elements(output_flags));
         output_flags[slot] = stgp->Base.Base.OutputFlags[attr];
      }
   }

   assert(gs_output_semantic_name[0] == TGSI_SEMANTIC_POSITION);

   if (outputMapping) {
      /* find max output slot referenced to compute gs_num_outputs */
      GLuint maxSlot = 0;
      for (attr = 0; attr < GEOM_RESULT_MAX; attr++) {
         if (outputMapping[attr] != ~0 && outputMapping[attr] > maxSlot)
            maxSlot = outputMapping[attr];
      }
      gs_num_outputs = maxSlot + 1;
   }
   else {
      outputMapping = defaultOutputMapping;
   }

#if 0 /* debug */
   {
      GLuint i;
      printf("outputMapping? %d\n", outputMapping ? 1 : 0);
      if (outputMapping) {
         printf("attr -> slot\n");
         for (i = 0; i < 16;  i++) {
            printf(" %2d       %3d\n", i, outputMapping[i]);
         }
      }
      printf("slot    sem_name  sem_index\n");
      for (i = 0; i < gs_num_outputs; i++) {
         printf(" %2d         %d         %d\n",
                i,
                gs_output_semantic_name[i],
                gs_output_semantic_index[i]);
      }
   }
#endif

   /* free old shader state, if any */
   if (stgp->state.shader.tokens) {
      _mesa_free((void *) stgp->state.shader.tokens);
      stgp->state.shader.tokens = NULL;
   }
   if (stgp->driver_shader) {
      cso_delete_geometry_shader(st->cso_context, stgp->driver_shader);
      stgp->driver_shader = NULL;
   }

   if (!inputMapping)
      inputMapping = defaultInputMapping;
   /* XXX: fix static allocation of tokens:
    */
   gs.shader.tokens = st_translate_mesa_program(st->ctx,
                                          TGSI_PROCESSOR_GEOMETRY,
                                          &stgp->Base.Base,
                                          /* inputs */
                                          gs_num_inputs,
                                          inputMapping,
                                          stgp->input_semantic_name,
                                          stgp->input_semantic_index,
                                          NULL,
                                          input_flags,
                                          /* outputs */
                                          gs_num_outputs,
                                          outputMapping,
                                          gs_output_semantic_name,
                                          gs_output_semantic_index,
                                          output_flags);

   gs.vertices_out = stgp->Base.VerticesOut;
   gs.input_type = stgp->Base.InputType;
   gs.output_type = stgp->Base.OutputType;

   stgp->num_inputs = gs_num_inputs;
   stgp->state = gs; /* struct copy */
   stgp->driver_shader = pipe->create_gs_state(pipe, &gs);

   if ((ST_DEBUG & DEBUG_TGSI) && (ST_DEBUG & DEBUG_MESA)) {
      _mesa_print_program(&stgp->Base.Base);
      debug_printf("\n");
   }

   if (ST_DEBUG & DEBUG_TGSI) {
      tgsi_dump(gs.shader.tokens, 0);
      debug_printf("\n");
   }
}

/**
 * Debug- print current shader text
 */
void
st_print_shaders(GLcontext *ctx)
{
   struct gl_shader_program *shProg = ctx->Shader.CurrentProgram;
   if (shProg) {
      GLuint i;
      for (i = 0; i < shProg->NumShaders; i++) {
         printf("GLSL shader %u of %u:\n", i, shProg->NumShaders);
         printf("%s\n", shProg->Shaders[i]->Source);
      }
   }
}
