/**************************************************************************
 *
 * Copyright 2009 VMWare Inc.
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

#include "draw_gs.h"

#include "draw_private.h"
#include "draw_context.h"

#include "tgsi/tgsi_parse.h"
#include "pipe/p_shader_tokens.h"

#include "util/u_math.h"
#include "util/u_memory.h"

#define MAX_PRIM_VERTICES 6
/* fixme: move it from here */
#define MAX_PRIMITIVES 128

boolean
draw_gs_init( struct draw_context *draw )
{
   tgsi_exec_machine_init(&draw->gs.machine);

   draw->gs.machine.Inputs = align_malloc(MAX_PRIM_VERTICES *
                                          PIPE_MAX_ATTRIBS * sizeof(struct tgsi_exec_vector), 16);
   if (!draw->gs.machine.Inputs)
      return FALSE;

   draw->gs.machine.Outputs = align_malloc(MAX_PRIM_VERTICES *
                                           PIPE_MAX_ATTRIBS * sizeof(struct tgsi_exec_vector), 16);
   if (!draw->gs.machine.Outputs)
      return FALSE;

   draw->gs.machine.Primitives = align_malloc(MAX_PRIMITIVES * sizeof(struct tgsi_exec_vector), 16);
   if (!draw->gs.machine.Primitives)
      return FALSE;

   return TRUE;
}


struct draw_geometry_shader *
draw_create_geometry_shader(struct draw_context *draw,
                            const struct pipe_geometry_shader_state *state)
{
   struct draw_geometry_shader *gs;

   gs = CALLOC_STRUCT(draw_geometry_shader);

   if (!gs)
      return NULL;

   gs->state = *state;
   gs->state.shader.tokens = tgsi_dup_tokens(state->shader.tokens);
   if (!gs->state.shader.tokens) {
      FREE(gs);
      return NULL;
   }

   tgsi_scan_shader(state->shader.tokens, &gs->info);

   gs->machine = &draw->gs.machine;

   return gs;
}

void draw_bind_geometry_shader(struct draw_context *draw,
                               struct draw_geometry_shader *dgs)
{
   draw_do_flush(draw, DRAW_FLUSH_STATE_CHANGE);

   if (dgs) {
      draw->gs.geometry_shader = dgs;
      draw->gs.num_gs_outputs = dgs->info.num_outputs;
      draw->gs.position_output = dgs->position_output;
      draw_geometry_shader_prepare(dgs, draw);
   }
   else {
      draw->gs.geometry_shader = NULL;
      draw->gs.num_gs_outputs = 0;
   }
}

void draw_delete_geometry_shader(struct draw_context *draw,
                                 struct draw_geometry_shader *dvs)
{
   FREE(dvs);
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

static void draw_fetch_geometry_input(struct draw_geometry_shader *shader,
                                      int start_primitive,
                                      int num_primitives,
                                      const float (*input)[4],
                                      unsigned input_stride,
                                      unsigned inputs_from_vs)
{
   struct tgsi_exec_machine *machine = shader->machine;
   unsigned slot, vs_slot, k, j;
   unsigned num_vertices = num_vertices_for_prim(shader->state.input_type);

   for (k = 0; k < num_primitives; ++k) {
      debug_printf("%d) Prim\n", start_primitive + k);
      for (j = 0; j < num_vertices; j++) {
         int idx = ((start_primitive + k) * num_vertices + j) * inputs_from_vs;
         debug_printf("\t%d)(%d) Input vert:\n", idx, j);

         for (slot = 0, vs_slot = 0; slot < shader->info.num_inputs; slot++) {
            if (shader->info.input_semantic_name[slot] == TGSI_SEMANTIC_VERTICES) {
               machine->Inputs[idx + slot].xyzw[0].f[j] = (float)num_vertices;
               machine->Inputs[idx + slot].xyzw[1].f[j] = (float)num_vertices;
               machine->Inputs[idx + slot].xyzw[2].f[j] = (float)num_vertices;
               machine->Inputs[idx + slot].xyzw[3].f[j] = (float)num_vertices;
            }
            else {
#if 1
               debug_printf("\t%d: %f %f %f %f\n", slot,
                            input[vs_slot][0],
                            input[vs_slot][1],
                            input[vs_slot][2],
                            input[vs_slot][3]);
               assert(!util_is_inf_or_nan(input[vs_slot][0]));
               assert(!util_is_inf_or_nan(input[vs_slot][1]));
               assert(!util_is_inf_or_nan(input[vs_slot][2]));
               assert(!util_is_inf_or_nan(input[vs_slot][3]));
#endif

               machine->Inputs[idx + slot].xyzw[0].f[j] = input[vs_slot][0];
               machine->Inputs[idx + slot].xyzw[1].f[j] = input[vs_slot][1];
               machine->Inputs[idx + slot].xyzw[2].f[j] = input[vs_slot][2];
               machine->Inputs[idx + slot].xyzw[3].f[j] = input[vs_slot][3];
               ++vs_slot;
            }
         }

         input = (const float (*)[4])((const char *)input + input_stride);
      }
   }
}

void draw_geometry_shader_run(struct draw_geometry_shader *shader,
                              const float (*input)[4],
                              float (*output)[4],
                              const float (*constants)[4],
                              unsigned count,
                              unsigned input_stride,
                              unsigned output_stride)
{
   struct tgsi_exec_machine *machine = shader->machine;
   unsigned int i, j;
   unsigned slot;
   unsigned num_vertices = num_vertices_for_prim(shader->state.input_type);
   unsigned num_primitives = count/num_vertices;
   unsigned inputs_from_vs = 0;

   machine->Consts = constants;

   for (i = 0; i < shader->info.num_inputs; ++i) {
      if (shader->info.input_semantic_name[i] != TGSI_SEMANTIC_VERTICES &&
          shader->info.input_semantic_name[i] != TGSI_SEMANTIC_PRIMID)
         ++inputs_from_vs;
   }

   for (i = 0; i < num_primitives; i += MAX_TGSI_PRIMITIVES) {
      unsigned int max_primitives = MIN2(MAX_TGSI_PRIMITIVES, num_primitives - i);

      draw_fetch_geometry_input(shader, i, max_primitives, input, input_stride, inputs_from_vs);

      tgsi_set_exec_mask(machine,
                         1,
                         max_primitives > 1,
                         max_primitives > 2,
                         max_primitives > 3);

      /* run interpreter */
      tgsi_exec_machine_run(machine);

      /* Unswizzle all output results.
       */
      for (j = 0; j < num_vertices; j++) {
         for (slot = 0; slot < shader->info.num_outputs; slot++) {
            output[slot][0] = machine->Outputs[slot].xyzw[0].f[j];
            output[slot][1] = machine->Outputs[slot].xyzw[1].f[j];
            output[slot][2] = machine->Outputs[slot].xyzw[2].f[j];
            output[slot][3] = machine->Outputs[slot].xyzw[3].f[j];

         }

#if 0
	 debug_printf("%d) Post xform vert:\n", i + j);
	 for (slot = 0; slot < shader->info.num_outputs; slot++) {
	    debug_printf("\t%d: %f %f %f %f\n", slot,
			 output[slot][0],
			 output[slot][1],
			 output[slot][2],
			 output[slot][3]);
            assert(!util_is_inf_or_nan(output[slot][0]));
         }
#endif

	 output = (float (*)[4])((char *)output + output_stride);
      }
   }
}

void draw_geometry_shader_delete(struct draw_geometry_shader *shader)
{
   FREE((void*) shader->state.shader.tokens);
   FREE(shader);
}

void draw_geometry_shader_prepare(struct draw_geometry_shader *shader,
                                  struct draw_context *draw)
{
    if (shader->machine->Tokens != shader->state.shader.tokens) {
       tgsi_exec_machine_bind_shader(shader->machine,
                                     shader->state.shader.tokens,
                                     draw->gs.num_samplers,
                                     draw->gs.samplers);
    }
}
