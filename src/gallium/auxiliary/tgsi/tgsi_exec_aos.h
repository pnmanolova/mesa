/**************************************************************************
 *
 * Copyright 2009 VMWare, Inc.
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

#ifndef TGSI_EXEC_AOS_H
#define TGSI_EXEC_AOS_H

#include "tgsi/tgsi_exec.h"
#include "pipe/p_compiler.h"

#if defined __cplusplus
extern "C" {
#endif

/**
  * Registers may be treated as float, signed int or unsigned int.
  */
union tgsi_exec_channel_aos
{
   float    f;
   int      i;
   unsigned u;
};

/**
  * A vector[RGBA] of channels[4 pixels]
  */
struct tgsi_exec_vector_aos
{
   union tgsi_exec_channel_aos xyzw[NUM_CHANNELS];
};

/**
 * Information for sampling textures, which must be implemented
 * by code outside the TGSI executor.
 */
struct tgsi_sampler_aos
{
   /** Get sample */
   void (*get_samples)(struct tgsi_sampler_aos *sampler,
                       const float s,
                       const float t,
                       const float p,
                       float lodbias,
                       float rgba[NUM_CHANNELS]);
};

/**
 * Run-time virtual machine state for executing TGSI shader.
 */
struct tgsi_exec_machine_aos
{
   /* Total = program temporaries + internal temporaries
    *         + 1 padding to align to 16 bytes
    */
   struct tgsi_exec_vector_aos    _Temps[TGSI_EXEC_NUM_TEMPS +
                                      TGSI_EXEC_NUM_TEMP_EXTRAS + 1];

   /*
    * This will point to _Temps after aligning to 16B boundary.
    */
   struct tgsi_exec_vector_aos    *Temps;
   struct tgsi_exec_vector_aos    *Addrs;

   struct tgsi_sampler_aos      **Samplers;

   float                         Imms[TGSI_EXEC_NUM_IMMEDIATES][4];
   unsigned                      ImmLimit;
   const float                   (*Consts)[4];
   struct tgsi_exec_vector_aos   *Inputs;
   struct tgsi_exec_vector_aos   *Outputs;
   const struct tgsi_token       *Tokens;   /**< Declarations, instructions */
   unsigned                      Processor; /**< TGSI_PROCESSOR_x */

   /* GEOMETRY processor only. */
   unsigned                      *Primitives;

   /* Conditional execution masks */
   uint CondMask;  /**< For IF/ELSE/ENDIF */
   uint LoopMask;  /**< For BGNLOOP/ENDLOOP */
   uint ContMask;  /**< For loop CONT statements */
   uint FuncMask;  /**< For function calls */
   uint ExecMask;  /**< = CondMask & LoopMask */

   /** Condition mask stack (for nested conditionals) */
   uint CondStack[TGSI_EXEC_MAX_COND_NESTING];
   int CondStackTop;

   /** Loop mask stack (for nested loops) */
   uint LoopStack[TGSI_EXEC_MAX_LOOP_NESTING];
   int LoopStackTop;

   /** Loop continue mask stack (see comments in tgsi_exec.c) */
   uint ContStack[TGSI_EXEC_MAX_LOOP_NESTING];
   int ContStackTop;

   /** Function execution mask stack (for executing subroutine code) */
   uint FuncStack[TGSI_EXEC_MAX_CALL_NESTING];
   int FuncStackTop;

   /** Function call stack for saving/restoring the program counter */
   uint CallStack[TGSI_EXEC_MAX_CALL_NESTING];
   int CallStackTop;

   struct tgsi_full_instruction *Instructions;
   uint NumInstructions;

   struct tgsi_full_declaration *Declarations;
   uint NumDeclarations;

   struct tgsi_exec_labels Labels;
};

void
tgsi_exec_machine_aos_init(
   struct tgsi_exec_machine_aos *mach );


void
tgsi_exec_machine_aos_bind_shader(
   struct tgsi_exec_machine_aos *mach,
   const struct tgsi_token *tokens,
   uint numSamplers,
   struct tgsi_sampler_aos **samplers);

uint
tgsi_exec_machine_aos_run(
   struct tgsi_exec_machine_aos *mach );


void
tgsi_exec_machine_aos_free_data(struct tgsi_exec_machine_aos *mach);


static INLINE void
tgsi_set_kill_mask_aos(struct tgsi_exec_machine_aos *mach, unsigned mask)
{
   mach->Temps[TGSI_EXEC_TEMP_KILMASK_I].xyzw[TGSI_EXEC_TEMP_KILMASK_C].u =
      mask;
}

#if defined __cplusplus
} /* extern "C" */
#endif

#endif /* TGSI_EXEC_AOS_H */
