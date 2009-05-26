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

#include "tgsi_exec_aos.h"

#include "pipe/p_compiler.h"
#include "pipe/p_state.h"
#include "pipe/p_shader_tokens.h"
#include "tgsi/tgsi_dump.h"
#include "tgsi/tgsi_parse.h"
#include "tgsi/tgsi_util.h"
#include "util/u_memory.h"
#include "util/u_math.h"

#define FAST_MATH 1

#define TILE_TOP_LEFT     0
#define TILE_TOP_RIGHT    1
#define TILE_BOTTOM_LEFT  2
#define TILE_BOTTOM_RIGHT 3

#define CHAN_X  0
#define CHAN_Y  1
#define CHAN_Z  2
#define CHAN_W  3

/*
 * Shorthand locations of various utility registers (_I = Index, _C = Channel)
 */
#define TEMP_0_I           TGSI_EXEC_TEMP_00000000_I
#define TEMP_0_C           TGSI_EXEC_TEMP_00000000_C
#define TEMP_7F_I          TGSI_EXEC_TEMP_7FFFFFFF_I
#define TEMP_7F_C          TGSI_EXEC_TEMP_7FFFFFFF_C
#define TEMP_80_I          TGSI_EXEC_TEMP_80000000_I
#define TEMP_80_C          TGSI_EXEC_TEMP_80000000_C
#define TEMP_FF_I          TGSI_EXEC_TEMP_FFFFFFFF_I
#define TEMP_FF_C          TGSI_EXEC_TEMP_FFFFFFFF_C
#define TEMP_1_I           TGSI_EXEC_TEMP_ONE_I
#define TEMP_1_C           TGSI_EXEC_TEMP_ONE_C
#define TEMP_2_I           TGSI_EXEC_TEMP_TWO_I
#define TEMP_2_C           TGSI_EXEC_TEMP_TWO_C
#define TEMP_128_I         TGSI_EXEC_TEMP_128_I
#define TEMP_128_C         TGSI_EXEC_TEMP_128_C
#define TEMP_M128_I        TGSI_EXEC_TEMP_MINUS_128_I
#define TEMP_M128_C        TGSI_EXEC_TEMP_MINUS_128_C
#define TEMP_KILMASK_I     TGSI_EXEC_TEMP_KILMASK_I
#define TEMP_KILMASK_C     TGSI_EXEC_TEMP_KILMASK_C
#define TEMP_OUTPUT_I      TGSI_EXEC_TEMP_OUTPUT_I
#define TEMP_OUTPUT_C      TGSI_EXEC_TEMP_OUTPUT_C
#define TEMP_PRIMITIVE_I   TGSI_EXEC_TEMP_PRIMITIVE_I
#define TEMP_PRIMITIVE_C   TGSI_EXEC_TEMP_PRIMITIVE_C
#define TEMP_CC_I          TGSI_EXEC_TEMP_CC_I
#define TEMP_CC_C          TGSI_EXEC_TEMP_CC_C
#define TEMP_3_I           TGSI_EXEC_TEMP_THREE_I
#define TEMP_3_C           TGSI_EXEC_TEMP_THREE_C
#define TEMP_HALF_I        TGSI_EXEC_TEMP_HALF_I
#define TEMP_HALF_C        TGSI_EXEC_TEMP_HALF_C
#define TEMP_R0            TGSI_EXEC_TEMP_R0

#define IS_CHANNEL_ENABLED(INST, CHAN)\
   ((INST).FullDstRegisters[0].DstRegister.WriteMask & (1 << (CHAN)))

#define IS_CHANNEL_ENABLED2(INST, CHAN)\
   ((INST).FullDstRegisters[1].DstRegister.WriteMask & (1 << (CHAN)))

#define FOR_EACH_ENABLED_CHANNEL(INST, CHAN)\
   for (CHAN = 0; CHAN < NUM_CHANNELS; CHAN++)\
      if (IS_CHANNEL_ENABLED( INST, CHAN ))

#define FOR_EACH_ENABLED_CHANNEL2(INST, CHAN)\
   for (CHAN = 0; CHAN < NUM_CHANNELS; CHAN++)\
      if (IS_CHANNEL_ENABLED2( INST, CHAN ))


/** The execution mask depends on the conditional mask and the loop mask */
#define UPDATE_EXEC_MASK(MACH) \
      MACH->ExecMask = MACH->CondMask & MACH->LoopMask & MACH->ContMask & MACH->FuncMask


static const union tgsi_exec_channel_aos ZeroVec =
   { 0.0 };


#ifdef DEBUG
static void
check_inf_or_nan(const union tgsi_exec_channel_aos *chan)
{
   assert(!util_is_inf_or_nan(chan->f));
}
#endif


#ifdef DEBUG
static void
print_chan(const char *msg, const union tgsi_exec_channel_aos *chan)
{
   debug_printf("%s = %f\n",
                msg, chan->f);
}
#endif


#ifdef DEBUG
static void
print_temp(const struct tgsi_exec_machine_aos *mach, uint index)
{
   const struct tgsi_exec_vector_aos *tmp = &mach->Temps[index];
   debug_printf("Temp[%u] = {%f, %f, %f, %f}\n", index,
                tmp->xyzw[0].f, tmp->xyzw[1].f,
                tmp->xyzw[2].f, tmp->xyzw[3].f);
}
#endif


/**
 * Check if there's a potential src/dst register data dependency when
 * using SOA execution.
 * Example:
 *   MOV T, T.yxwz;
 * This would expand into:
 *   MOV t0, t1;
 *   MOV t1, t0;
 *   MOV t2, t3;
 *   MOV t3, t2;
 * The second instruction will have the wrong value for t0 if executed as-is.
 */
static boolean
tgsi_check_soa_dependencies(const struct tgsi_full_instruction *inst)
{
   uint i, chan;

   uint writemask = inst->FullDstRegisters[0].DstRegister.WriteMask;
   if (writemask == TGSI_WRITEMASK_X ||
       writemask == TGSI_WRITEMASK_Y ||
       writemask == TGSI_WRITEMASK_Z ||
       writemask == TGSI_WRITEMASK_W ||
       writemask == TGSI_WRITEMASK_NONE) {
      /* no chance of data dependency */
      return FALSE;
   }

   /* loop over src regs */
   for (i = 0; i < inst->Instruction.NumSrcRegs; i++) {
      if ((inst->FullSrcRegisters[i].SrcRegister.File ==
           inst->FullDstRegisters[0].DstRegister.File) &&
          (inst->FullSrcRegisters[i].SrcRegister.Index ==
           inst->FullDstRegisters[0].DstRegister.Index)) {
         /* loop over dest channels */
         uint channelsWritten = 0x0;
         FOR_EACH_ENABLED_CHANNEL(*inst, chan) {
            /* check if we're reading a channel that's been written */
            uint swizzle = tgsi_util_get_full_src_register_extswizzle(&inst->FullSrcRegisters[i], chan);
            if (swizzle <= TGSI_SWIZZLE_W &&
                (channelsWritten & (1 << swizzle))) {
               return TRUE;
            }

            channelsWritten |= (1 << chan);
         }
      }
   }
   return FALSE;
}


/**
 * Initialize machine state by expanding tokens to full instructions,
 * allocating temporary storage, setting up constants, etc.
 * After this, we can call tgsi_exec_machine_aos_run() many times.
 */
void 
tgsi_exec_machine_aos_bind_shader(
   struct tgsi_exec_machine_aos *mach,
   const struct tgsi_token *tokens,
   uint numSamplers,
   struct tgsi_sampler_aos **samplers)
{
   uint k;
   struct tgsi_parse_context parse;
   struct tgsi_exec_labels *labels = &mach->Labels;
   struct tgsi_full_instruction *instructions;
   struct tgsi_full_declaration *declarations;
   uint maxInstructions = 10, numInstructions = 0;
   uint maxDeclarations = 10, numDeclarations = 0;
   uint instno = 0;

#if 0
   tgsi_dump(tokens, 0);
#endif

   util_init_math();

   mach->Tokens = tokens;
   mach->Samplers = samplers;

   k = tgsi_parse_init (&parse, mach->Tokens);
   if (k != TGSI_PARSE_OK) {
      debug_printf( "Problem parsing!\n" );
      return;
   }

   mach->Processor = parse.FullHeader.Processor.Processor;
   mach->ImmLimit = 0;
   labels->count = 0;

   declarations = (struct tgsi_full_declaration *)
      MALLOC( maxDeclarations * sizeof(struct tgsi_full_declaration) );

   if (!declarations) {
      return;
   }

   instructions = (struct tgsi_full_instruction *)
      MALLOC( maxInstructions * sizeof(struct tgsi_full_instruction) );

   if (!instructions) {
      FREE( declarations );
      return;
   }

   while( !tgsi_parse_end_of_tokens( &parse ) ) {
      uint pointer = parse.Position;
      uint i;

      tgsi_parse_token( &parse );
      switch( parse.FullToken.Token.Type ) {
      case TGSI_TOKEN_TYPE_DECLARATION:
         /* save expanded declaration */
         if (numDeclarations == maxDeclarations) {
            declarations = REALLOC(declarations,
                                   maxDeclarations
                                   * sizeof(struct tgsi_full_declaration),
                                   (maxDeclarations + 10)
                                   * sizeof(struct tgsi_full_declaration));
            maxDeclarations += 10;
         }
         memcpy(declarations + numDeclarations,
                &parse.FullToken.FullDeclaration,
                sizeof(declarations[0]));
         numDeclarations++;
         break;

      case TGSI_TOKEN_TYPE_IMMEDIATE:
         {
            uint size = parse.FullToken.FullImmediate.Immediate.NrTokens - 1;
            assert( size % 4 == 0 );
            assert( mach->ImmLimit + size / 4 <= TGSI_EXEC_NUM_IMMEDIATES );

            for( i = 0; i < size; i++ ) {
               mach->Imms[mach->ImmLimit + i / 4][i % 4] = 
		  parse.FullToken.FullImmediate.u.ImmediateFloat32[i].Float;
            }
            mach->ImmLimit += size / 4;
         }
         break;

      case TGSI_TOKEN_TYPE_INSTRUCTION:
         assert( labels->count < MAX_LABELS );

         labels->labels[labels->count][0] = instno;
         labels->labels[labels->count][1] = pointer;
         labels->count++;

         /* save expanded instruction */
         if (numInstructions == maxInstructions) {
            instructions = REALLOC(instructions,
                                   maxInstructions
                                   * sizeof(struct tgsi_full_instruction),
                                   (maxInstructions + 10)
                                   * sizeof(struct tgsi_full_instruction));
            maxInstructions += 10;
         }
         memcpy(instructions + numInstructions,
                &parse.FullToken.FullInstruction,
                sizeof(instructions[0]));

#if 0
         if (tgsi_check_soa_dependencies(&parse.FullToken.FullInstruction)) {
            debug_printf("SOA dependency in instruction:\n");
            tgsi_dump_instruction(&parse.FullToken.FullInstruction,
                                  numInstructions);
         }
#else
         (void) tgsi_check_soa_dependencies;
#endif

         numInstructions++;
         break;

      default:
         assert( 0 );
      }
   }
   tgsi_parse_free (&parse);

   if (mach->Declarations) {
      FREE( mach->Declarations );
   }
   mach->Declarations = declarations;
   mach->NumDeclarations = numDeclarations;

   if (mach->Instructions) {
      FREE( mach->Instructions );
   }
   mach->Instructions = instructions;
   mach->NumInstructions = numInstructions;
}


void
tgsi_exec_machine_aos_init(
   struct tgsi_exec_machine_aos *mach )
{
   assert(mach->Processor == TGSI_PROCESSOR_VERTEX ||
          mach->Processor == TGSI_PROCESSOR_GEOMETRY);

   mach->Temps = (struct tgsi_exec_vector_aos *) tgsi_align_128bit( mach->_Temps);
   mach->Addrs = &mach->Temps[TGSI_EXEC_TEMP_ADDR];

   /* Setup constants. */
   mach->Temps[TEMP_0_I].xyzw[TEMP_0_C].u = 0x00000000;
   mach->Temps[TEMP_7F_I].xyzw[TEMP_7F_C].u = 0x7FFFFFFF;
   mach->Temps[TEMP_80_I].xyzw[TEMP_80_C].u = 0x80000000;
   mach->Temps[TEMP_FF_I].xyzw[TEMP_FF_C].u = 0xFFFFFFFF;
   mach->Temps[TEMP_1_I].xyzw[TEMP_1_C].f = 1.0f;
   mach->Temps[TEMP_2_I].xyzw[TEMP_2_C].f = 2.0f;
   mach->Temps[TEMP_128_I].xyzw[TEMP_128_C].f = 128.0f;
   mach->Temps[TEMP_M128_I].xyzw[TEMP_M128_C].f = -128.0f;
   mach->Temps[TEMP_3_I].xyzw[TEMP_3_C].f = 3.0f;
   mach->Temps[TEMP_HALF_I].xyzw[TEMP_HALF_C].f = 0.5f;

#ifdef DEBUG
   /* silence warnings */
   (void) print_chan;
   (void) print_temp;
#endif
}


void
tgsi_exec_machine_aos_free_data(struct tgsi_exec_machine_aos *mach)
{
   if (mach->Instructions) {
      FREE(mach->Instructions);
      mach->Instructions = NULL;
      mach->NumInstructions = 0;
   }
   if (mach->Declarations) {
      FREE(mach->Declarations);
      mach->Declarations = NULL;
      mach->NumDeclarations = 0;
   }
}


static void
micro_abs(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src )
{
   dst->f = fabsf( src->f );
}

static void
micro_add(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
   dst->f = src0->f + src1->f;
}

#if 0
static void
micro_iadd(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
   dst->i = src0->i + src1->i;
   dst->i[1] = src0->i[1] + src1->i[1];
   dst->i[2] = src0->i[2] + src1->i[2];
   dst->i[3] = src0->i[3] + src1->i[3];
}
#endif

static void
micro_and(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
   dst->u = src0->u & src1->u;
   dst->u = src0->u & src1->u;
   dst->u = src0->u & src1->u;
   dst->u = src0->u & src1->u;
}

static void
micro_ceil(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src )
{
   dst->f = ceilf( src->f );
}

static void
micro_cos(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src )
{
   dst->f = cosf( src->f );
}

static void
micro_div(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
   if (src1->f != 0)
      dst->f = src0->f / src1->f;
}

#if 0
static void
micro_udiv(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
   dst->u = src0->u / src1->u;
}
#endif

static void
micro_eq(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1,
   const union tgsi_exec_channel_aos *src2,
   const union tgsi_exec_channel_aos *src3 )
{
   dst->f = src0->f == src1->f ? src2->f : src3->f;
}

#if 0
static void
micro_ieq(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1,
   const union tgsi_exec_channel_aos *src2,
   const union tgsi_exec_channel_aos *src3 )
{
   dst->i = src0->i == src1->i ? src2->i : src3->i;
   dst->i[1] = src0->i[1] == src1->i[1] ? src2->i[1] : src3->i[1];
   dst->i[2] = src0->i[2] == src1->i[2] ? src2->i[2] : src3->i[2];
   dst->i[3] = src0->i[3] == src1->i[3] ? src2->i[3] : src3->i[3];
}
#endif

static void
micro_exp2(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src)
{
#if FAST_MATH
   dst->f = util_fast_exp2( src->f );
#else
   dst->f = powf( 2.0f, src->f );
#endif
}

#if 0
static void
micro_f2ut(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src )
{
   dst->u = (uint) src->f;
}
#endif

static void
micro_float_clamp(union tgsi_exec_channel_aos *dst,
                  const union tgsi_exec_channel_aos *src)
{
   if (src->f > 0.0f) {
      if (src->f > 1.884467e+019f)
         dst->f = 1.884467e+019f;
      else if (src->f < 5.42101e-020f)
         dst->f = 5.42101e-020f;
      else
         dst->f = src->f;
   }
   else {
      if (src->f < -1.884467e+019f)
         dst->f = -1.884467e+019f;
      else if (src->f > -5.42101e-020f)
         dst->f = -5.42101e-020f;
      else
         dst->f = src->f;
   }
}

static void
micro_flr(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src )
{
   dst->f = floorf( src->f );
}

static void
micro_frc(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src )
{
   dst->f = src->f - floorf( src->f );
}

static void
micro_i2f(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src )
{
   dst->f = (float) src->i;
}

static void
micro_lg2(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src )
{
#if FAST_MATH
   dst->f = util_fast_log2( src->f );
#else
   dst->f = logf( src->f ) * 1.442695f;
#endif
}

static void
micro_le(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1,
   const union tgsi_exec_channel_aos *src2,
   const union tgsi_exec_channel_aos *src3 )
{
   dst->f = src0->f <= src1->f ? src2->f : src3->f;
}

static void
micro_lt(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1,
   const union tgsi_exec_channel_aos *src2,
   const union tgsi_exec_channel_aos *src3 )
{
   dst->f = src0->f < src1->f ? src2->f : src3->f;
}

#if 0
static void
micro_ilt(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1,
   const union tgsi_exec_channel_aos *src2,
   const union tgsi_exec_channel_aos *src3 )
{
   dst->i = src0->i < src1->i ? src2->i : src3->i;
}
#endif

#if 0
static void
micro_ult(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1,
   const union tgsi_exec_channel_aos *src2,
   const union tgsi_exec_channel_aos *src3 )
{
   dst->u = src0->u < src1->u ? src2->u : src3->u;
}
#endif

static void
micro_max(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
   dst->f = src0->f > src1->f ? src0->f : src1->f;
}

#if 0
static void
micro_imax(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
   dst->i = src0->i > src1->i ? src0->i : src1->i;
}
#endif

#if 0
static void
micro_umax(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
   dst->u = src0->u > src1->u ? src0->u : src1->u;
}
#endif

static void
micro_min(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
   dst->f = src0->f < src1->f ? src0->f : src1->f;
}

#if 0
static void
micro_imin(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
   dst->i = src0->i < src1->i ? src0->i : src1->i;
}
#endif

#if 0
static void
micro_umin(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
   dst->u = src0->u < src1->u ? src0->u : src1->u;
}
#endif

#if 0
static void
micro_umod(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
   dst->u = src0->u % src1->u;
}
#endif

static void
micro_mul(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
   dst->f = src0->f * src1->f;
}

#if 0
static void
micro_imul(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
   dst->i = src0->i * src1->i;
}
#endif

#if 0
static void
micro_imul64(
   union tgsi_exec_channel_aos *dst0,
   union tgsi_exec_channel_aos *dst1,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
   dst1->i = src0->i * src1->i;
   dst0->i = 0;
}
#endif

#if 0
static void
micro_umul64(
   union tgsi_exec_channel_aos *dst0,
   union tgsi_exec_channel_aos *dst1,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
   dst1->u = src0->u * src1->u;
   dst0->u = 0;
}
#endif


#if 0
static void
micro_movc(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1,
   const union tgsi_exec_channel_aos *src2 )
{
   dst->u = src0->u ? src1->u : src2->u;
}
#endif

static void
micro_neg(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src )
{
   dst->f = -src->f;
}

#if 0
static void
micro_ineg(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src )
{
   dst->i = -src->i;
}
#endif

static void
micro_not(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src )
{
   dst->u = ~src->u;
}

static void
micro_or(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
   dst->u = src0->u | src1->u;
}

static void
micro_pow(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
#if FAST_MATH
   dst->f = util_fast_pow( src0->f, src1->f );
#else
   dst->f = powf( src0->f, src1->f );
#endif
}

static void
micro_rnd(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src )
{
   dst->f = floorf( src->f + 0.5f );
}

static void
micro_sgn(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src )
{
   dst->f = src->f < 0.0f ? -1.0f : src->f > 0.0f ? 1.0f : 0.0f;
}

static void
micro_shl(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
   dst->i = src0->i << src1->i;
}

static void
micro_ishr(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
   dst->i = src0->i >> src1->i;
}

static void
micro_trunc(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0 )
{
   dst->f = (float) (int) src0->f;
}

#if 0
static void
micro_ushr(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
   dst->u = src0->u >> src1->u;
}
#endif

static void
micro_sin(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src )
{
   dst->f = sinf( src->f );
}

static void
micro_sqrt( union tgsi_exec_channel_aos *dst,
            const union tgsi_exec_channel_aos *src )
{
   dst->f = sqrtf( src->f );
}

static void
micro_sub(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
   dst->f = src0->f - src1->f;
}

#if 0
static void
micro_u2f(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src )
{
   dst->f = (float) src->u;
}
#endif

static void
micro_xor(
   union tgsi_exec_channel_aos *dst,
   const union tgsi_exec_channel_aos *src0,
   const union tgsi_exec_channel_aos *src1 )
{
   dst->u = src0->u ^ src1->u;
}

static void
fetch_src_file_channel(
   const struct tgsi_exec_machine_aos *mach,
   const uint file,
   const uint swizzle,
   const union tgsi_exec_channel_aos *index,
   union tgsi_exec_channel_aos *chan )
{
   switch( swizzle ) {
   case TGSI_EXTSWIZZLE_X:
   case TGSI_EXTSWIZZLE_Y:
   case TGSI_EXTSWIZZLE_Z:
   case TGSI_EXTSWIZZLE_W:
      switch( file ) {
      case TGSI_FILE_CONSTANT:
         assert(mach->Consts);
         if (index->i < 0)
            chan->f = 0.0f;
         else
            chan->f = mach->Consts[index->i][swizzle];
         break;

      case TGSI_FILE_INPUT:
         chan->u = mach->Inputs[index->i].xyzw[swizzle].u;
         break;

      case TGSI_FILE_TEMPORARY:
         assert(index->i < TGSI_EXEC_NUM_TEMPS);
         chan->u = mach->Temps[index->i].xyzw[swizzle].u;
         break;

      case TGSI_FILE_IMMEDIATE:
         assert( index->i < (int) mach->ImmLimit );
         chan->f = mach->Imms[index->i][swizzle];
         break;

      case TGSI_FILE_ADDRESS:
         chan->u = mach->Addrs[index->i].xyzw[swizzle].u;
         break;

      case TGSI_FILE_OUTPUT:
         /* vertex/fragment output vars can be read too */
         chan->u = mach->Outputs[index->i].xyzw[swizzle].u;
         break;

      default:
         assert( 0 );
      }
      break;

   case TGSI_EXTSWIZZLE_ZERO:
      *chan = mach->Temps[TEMP_0_I].xyzw[TEMP_0_C];
      break;

   case TGSI_EXTSWIZZLE_ONE:
      *chan = mach->Temps[TEMP_1_I].xyzw[TEMP_1_C];
      break;

   default:
      assert( 0 );
   }
}

static void
fetch_source(
   const struct tgsi_exec_machine_aos *mach,
   union tgsi_exec_channel_aos *chan,
   const struct tgsi_full_src_register *reg,
   const uint chan_index )
{
   union tgsi_exec_channel_aos index;
   uint swizzle;

   /* We start with a direct index into a register file.
    *
    *    file[1],
    *    where:
    *       file = SrcRegister.File
    *       [1] = SrcRegister.Index
    */
   index.i = reg->SrcRegister.Index;

   /* There is an extra source register that indirectly subscripts
    * a register file. The direct index now becomes an offset
    * that is being added to the indirect register.
    *
    *    file[ind[2].x+1],
    *    where:
    *       ind = SrcRegisterInd.File
    *       [2] = SrcRegisterInd.Index
    *       .x = SrcRegisterInd.SwizzleX
    */
   if (reg->SrcRegister.Indirect) {
      union tgsi_exec_channel_aos index2;
      union tgsi_exec_channel_aos indir_index;
      const uint execmask = mach->ExecMask;

      /* which address register (always zero now) */
      index2.i = reg->SrcRegisterInd.Index;

      /* get current value of address register[swizzle] */
      swizzle = tgsi_util_get_src_register_swizzle( &reg->SrcRegisterInd, CHAN_X );
      fetch_src_file_channel(
         mach,
         reg->SrcRegisterInd.File,
         swizzle,
         &index2,
         &indir_index );

      /* add value of address register to the offset */
      index.i += (int) indir_index.f;

      /* for disabled execution channels, zero-out the index to
       * avoid using a potential garbage value.
       */
      if ((execmask & (1 << 0)) == 0)
         index.i = 0;
   }

   /* There is an extra source register that is a second
    * subscript to a register file. Effectively it means that
    * the register file is actually a 2D array of registers.
    *
    *    file[1][3] == file[1*sizeof(file[1])+3],
    *    where:
    *       [3] = SrcRegisterDim.Index
    */
   if (reg->SrcRegister.Dimension) {
      /* The size of the first-order array depends on the register file type.
       * We need to multiply the index to the first array to get an effective,
       * "flat" index that points to the beginning of the second-order array.
       */
      switch (reg->SrcRegister.File) {
      case TGSI_FILE_INPUT:
         index.i *= TGSI_EXEC_MAX_INPUT_ATTRIBS;
         break;
      case TGSI_FILE_CONSTANT:
         index.i *= TGSI_EXEC_MAX_CONST_BUFFER;
         break;
      default:
         assert( 0 );
      }

      index.i += reg->SrcRegisterDim.Index;

      /* Again, the second subscript index can be addressed indirectly
       * identically to the first one.
       * Nothing stops us from indirectly addressing the indirect register,
       * but there is no need for that, so we won't exercise it.
       *
       *    file[1][ind[4].y+3],
       *    where:
       *       ind = SrcRegisterDimInd.File
       *       [4] = SrcRegisterDimInd.Index
       *       .y = SrcRegisterDimInd.SwizzleX
       */
      if (reg->SrcRegisterDim.Indirect) {
         union tgsi_exec_channel_aos index2;
         union tgsi_exec_channel_aos indir_index;
         const uint execmask = mach->ExecMask;

         index2.i = reg->SrcRegisterDimInd.Index;

         swizzle = tgsi_util_get_src_register_swizzle( &reg->SrcRegisterDimInd, CHAN_X );
         fetch_src_file_channel(
            mach,
            reg->SrcRegisterDimInd.File,
            swizzle,
            &index2,
            &indir_index );

         index.i += (int) indir_index.f;

         /* for disabled execution channels, zero-out the index to
          * avoid using a potential garbage value.
          */
         if ((execmask & (1 << 0)) == 0)
            index.i = 0;
      }

      /* If by any chance there was a need for a 3D array of register
       * files, we would have to check whether SrcRegisterDim is followed
       * by a dimension register and continue the saga.
       */
   }

   swizzle = tgsi_util_get_full_src_register_extswizzle( reg, chan_index );
   fetch_src_file_channel(
      mach,
      reg->SrcRegister.File,
      swizzle,
      &index,
      chan );

   switch (tgsi_util_get_full_src_register_sign_mode( reg, chan_index )) {
   case TGSI_UTIL_SIGN_CLEAR:
      micro_abs( chan, chan );
      break;

   case TGSI_UTIL_SIGN_SET:
      micro_abs( chan, chan );
      micro_neg( chan, chan );
      break;

   case TGSI_UTIL_SIGN_TOGGLE:
      micro_neg( chan, chan );
      break;

   case TGSI_UTIL_SIGN_KEEP:
      break;
   }

   if (reg->SrcRegisterExtMod.Complement) {
      micro_sub( chan, &mach->Temps[TEMP_1_I].xyzw[TEMP_1_C], chan );
   }
}

static void
store_dest(
   struct tgsi_exec_machine_aos *mach,
   const union tgsi_exec_channel_aos *chan,
   const struct tgsi_full_dst_register *reg,
   const struct tgsi_full_instruction *inst,
   uint chan_index )
{
   union tgsi_exec_channel_aos null;
   union tgsi_exec_channel_aos *dst;
   uint execmask = mach->ExecMask;

#ifdef DEBUG
   check_inf_or_nan(chan);
#endif

   switch (reg->DstRegister.File) {
   case TGSI_FILE_NULL:
      dst = &null;
      break;

   case TGSI_FILE_OUTPUT:
      dst = &mach->Outputs[mach->Temps[TEMP_OUTPUT_I].xyzw[TEMP_OUTPUT_C].u
                           + reg->DstRegister.Index].xyzw[chan_index];
      break;

   case TGSI_FILE_TEMPORARY:
      assert( reg->DstRegister.Index < TGSI_EXEC_NUM_TEMPS );
      dst = &mach->Temps[reg->DstRegister.Index].xyzw[chan_index];
      break;

   case TGSI_FILE_ADDRESS:
      dst = &mach->Addrs[reg->DstRegister.Index].xyzw[chan_index];
      break;

   default:
      assert( 0 );
      return;
   }

   if (inst->InstructionExtNv.CondFlowEnable) {
      union tgsi_exec_channel_aos *cc = &mach->Temps[TEMP_CC_I].xyzw[TEMP_CC_C];
      uint swizzle;
      uint shift;
      uint mask;
      uint test;

      /* Only CC0 supported.
       */
      assert( inst->InstructionExtNv.CondFlowIndex < 1 );

      switch (chan_index) {
      case CHAN_X:
         swizzle = inst->InstructionExtNv.CondSwizzleX;
         break;
      case CHAN_Y:
         swizzle = inst->InstructionExtNv.CondSwizzleY;
         break;
      case CHAN_Z:
         swizzle = inst->InstructionExtNv.CondSwizzleZ;
         break;
      case CHAN_W:
         swizzle = inst->InstructionExtNv.CondSwizzleW;
         break;
      default:
         assert( 0 );
         return;
      }

      switch (swizzle) {
      case TGSI_SWIZZLE_X:
         shift = TGSI_EXEC_CC_X_SHIFT;
         mask = TGSI_EXEC_CC_X_MASK;
         break;
      case TGSI_SWIZZLE_Y:
         shift = TGSI_EXEC_CC_Y_SHIFT;
         mask = TGSI_EXEC_CC_Y_MASK;
         break;
      case TGSI_SWIZZLE_Z:
         shift = TGSI_EXEC_CC_Z_SHIFT;
         mask = TGSI_EXEC_CC_Z_MASK;
         break;
      case TGSI_SWIZZLE_W:
         shift = TGSI_EXEC_CC_W_SHIFT;
         mask = TGSI_EXEC_CC_W_MASK;
         break;
      default:
         assert( 0 );
         return;
      }

      switch (inst->InstructionExtNv.CondMask) {
      case TGSI_CC_GT:
         test = ~(TGSI_EXEC_CC_GT << shift) & mask;
         if (cc->u & test)
            execmask &= ~(1 << 0);
         break;

      case TGSI_CC_EQ:
         test = ~(TGSI_EXEC_CC_EQ << shift) & mask;
         if (cc->u & test)
            execmask &= ~(1 << 0);
         break;

      case TGSI_CC_LT:
         test = ~(TGSI_EXEC_CC_LT << shift) & mask;
         if (cc->u & test)
            execmask &= ~(1 << 0);
         break;

      case TGSI_CC_GE:
         test = ~((TGSI_EXEC_CC_GT | TGSI_EXEC_CC_EQ) << shift) & mask;
         if (cc->u & test)
            execmask &= ~(1 << 0);
         break;

      case TGSI_CC_LE:
         test = ~((TGSI_EXEC_CC_LT | TGSI_EXEC_CC_EQ) << shift) & mask;
         if (cc->u & test)
            execmask &= ~(1 << 0);
         break;

      case TGSI_CC_NE:
         test = ~((TGSI_EXEC_CC_GT | TGSI_EXEC_CC_LT | TGSI_EXEC_CC_UN) << shift) & mask;
         if (cc->u & test)
            execmask &= ~(1 << 0);
         break;

      case TGSI_CC_TR:
         break;

      case TGSI_CC_FL:
         execmask &= ~(1 << 0);
         break;

      default:
         assert( 0 );
         return;
      }
   }

   switch (inst->Instruction.Saturate) {
   case TGSI_SAT_NONE:
      if (execmask & (1 << 0))
         dst->i = chan->i;
      break;

   case TGSI_SAT_ZERO_ONE:
      if (execmask & (1 << 0)) {
         if (chan->f < 0.0f)
            dst->f = 0.0f;
         else if (chan->f > 1.0f)
            dst->f = 1.0f;
         else
            dst->i = chan->i;
      }
      break;

   case TGSI_SAT_MINUS_PLUS_ONE:
      if (execmask & (1 << 0)) {
         if (chan->f < -1.0f)
            dst->f = -1.0f;
         else if (chan->f > 1.0f)
            dst->f = 1.0f;
         else
            dst->i = chan->i;
      }
      break;

   default:
      assert( 0 );
   }

   if (inst->InstructionExtNv.CondDstUpdate) {
      union tgsi_exec_channel_aos *cc = &mach->Temps[TEMP_CC_I].xyzw[TEMP_CC_C];
      uint shift;
      uint mask;

      /* Only CC0 supported.
       */
      assert( inst->InstructionExtNv.CondDstIndex < 1 );

      switch (chan_index) {
      case CHAN_X:
         shift = TGSI_EXEC_CC_X_SHIFT;
         mask = ~TGSI_EXEC_CC_X_MASK;
         break;
      case CHAN_Y:
         shift = TGSI_EXEC_CC_Y_SHIFT;
         mask = ~TGSI_EXEC_CC_Y_MASK;
         break;
      case CHAN_Z:
         shift = TGSI_EXEC_CC_Z_SHIFT;
         mask = ~TGSI_EXEC_CC_Z_MASK;
         break;
      case CHAN_W:
         shift = TGSI_EXEC_CC_W_SHIFT;
         mask = ~TGSI_EXEC_CC_W_MASK;
         break;
      default:
         assert( 0 );
         return;
      }

      if (execmask & (1 << 0)) {
         cc->u &= mask;
         if (dst->f < 0.0f)
            cc->u |= TGSI_EXEC_CC_LT << shift;
         else if (dst->f > 0.0f)
            cc->u |= TGSI_EXEC_CC_GT << shift;
         else if (dst->f == 0.0f)
            cc->u |= TGSI_EXEC_CC_EQ << shift;
         else
            cc->u |= TGSI_EXEC_CC_UN << shift;
      }
   }
}

#define FETCH(VAL,INDEX,CHAN)\
    fetch_source (mach, VAL, &inst->FullSrcRegisters[INDEX], CHAN)

#define STORE(VAL,INDEX,CHAN)\
    store_dest (mach, VAL, &inst->FullDstRegisters[INDEX], inst, CHAN )


/**
 * Execute ARB-style KIL which is predicated by a src register.
 * Kill fragment if any of the four values is less than zero.
 */
static void
exec_kil(struct tgsi_exec_machine_aos *mach,
         const struct tgsi_full_instruction *inst)
{
   uint uniquemask;
   uint chan_index;
   uint kilmask = 0; /* bit 0 = pixel 0, bit 1 = pixel 1, etc */
   union tgsi_exec_channel_aos r[1];

   /* This mask stores component bits that were already tested. Note that
    * we test if the value is less than zero, so 1.0 and 0.0 need not to be
    * tested. */
   uniquemask = (1 << TGSI_EXTSWIZZLE_ZERO) | (1 << TGSI_EXTSWIZZLE_ONE);

   for (chan_index = 0; chan_index < 4; chan_index++)
   {
      uint swizzle;

      /* unswizzle channel */
      swizzle = tgsi_util_get_full_src_register_extswizzle (
                        &inst->FullSrcRegisters[0],
                        chan_index);

      /* check if the component has not been already tested */
      if (uniquemask & (1 << swizzle))
         continue;
      uniquemask |= 1 << swizzle;

      FETCH(&r[0], 0, chan_index);
      if (r[0].f < 0.0f)
         kilmask |= 1 << 0;
   }

   mach->Temps[TEMP_KILMASK_I].xyzw[TEMP_KILMASK_C].u |= kilmask;
}

/**
 * Execute NVIDIA-style KIL which is predicated by a condition code.
 * Kill fragment if the condition code is TRUE.
 */
static void
exec_kilp(struct tgsi_exec_machine_aos *mach,
          const struct tgsi_full_instruction *inst)
{
   uint kilmask; /* bit 0 = pixel 0, bit 1 = pixel 1, etc */

   if (inst->InstructionExtNv.CondFlowEnable) {
      uint swizzle[4];
      uint chan_index;

      kilmask = 0x0;

      swizzle[0] = inst->InstructionExtNv.CondSwizzleX;
      swizzle[1] = inst->InstructionExtNv.CondSwizzleY;
      swizzle[2] = inst->InstructionExtNv.CondSwizzleZ;
      swizzle[3] = inst->InstructionExtNv.CondSwizzleW;

      for (chan_index = 0; chan_index < 4; chan_index++)
      {
         uint i;

         for (i = 0; i < 4; i++) {
            /* TODO: evaluate the condition code */
            if (0)
               kilmask |= 1 << i;
         }
      }
   }
   else {
      /* "unconditional" kil */
      kilmask = mach->ExecMask;
   }
   mach->Temps[TEMP_KILMASK_I].xyzw[TEMP_KILMASK_C].u |= kilmask;
}


/*
 * Fetch a four texture samples using STR texture coordinates.
 */
static void
fetch_texel( struct tgsi_sampler_aos *sampler,
             const union tgsi_exec_channel_aos *s,
             const union tgsi_exec_channel_aos *t,
             const union tgsi_exec_channel_aos *p,
             float lodbias,  /* XXX should be float[4] */
             union tgsi_exec_channel_aos *r,
             union tgsi_exec_channel_aos *g,
             union tgsi_exec_channel_aos *b,
             union tgsi_exec_channel_aos *a )
{
   uint j;
   float rgba[NUM_CHANNELS];

   sampler->get_samples(sampler, s->f, t->f, p->f, lodbias, rgba);

   for (j = 0; j < 4; j++) {
      r->f = rgba[0];
      g->f = rgba[1];
      b->f = rgba[2];
      a->f = rgba[3];
   }
}


static void
exec_tex(struct tgsi_exec_machine_aos *mach,
         const struct tgsi_full_instruction *inst,
         boolean biasLod,
         boolean projected)
{
   const uint unit = inst->FullSrcRegisters[1].SrcRegister.Index;
   union tgsi_exec_channel_aos r[4];
   uint chan_index;
   float lodBias;

   /*   debug_printf("Sampler %u unit %u\n", sampler, unit); */

   switch (inst->InstructionExtTexture.Texture) {
   case TGSI_TEXTURE_1D:
   case TGSI_TEXTURE_SHADOW1D:

      FETCH(&r[0], 0, CHAN_X);

      if (projected) {
         FETCH(&r[1], 0, CHAN_W);
         micro_div( &r[0], &r[0], &r[1] );
      }

      if (biasLod) {
         FETCH(&r[1], 0, CHAN_W);
         lodBias = r[2].f;
      }
      else
         lodBias = 0.0;

      fetch_texel(mach->Samplers[unit],
                  &r[0], &ZeroVec, &ZeroVec, lodBias,  /* S, T, P, BIAS */
                  &r[0], &r[1], &r[2], &r[3]); /* R, G, B, A */
      break;

   case TGSI_TEXTURE_2D:
   case TGSI_TEXTURE_RECT:
   case TGSI_TEXTURE_SHADOW2D:
   case TGSI_TEXTURE_SHADOWRECT:

      FETCH(&r[0], 0, CHAN_X);
      FETCH(&r[1], 0, CHAN_Y);
      FETCH(&r[2], 0, CHAN_Z);

      if (projected) {
         FETCH(&r[3], 0, CHAN_W);
         micro_div( &r[0], &r[0], &r[3] );
         micro_div( &r[1], &r[1], &r[3] );
         micro_div( &r[2], &r[2], &r[3] );
      }

      if (biasLod) {
         FETCH(&r[3], 0, CHAN_W);
         lodBias = r[3].f;
      }
      else
         lodBias = 0.0;

      fetch_texel(mach->Samplers[unit],
                  &r[0], &r[1], &r[2], lodBias,  /* inputs */
                  &r[0], &r[1], &r[2], &r[3]);  /* outputs */
      break;

   case TGSI_TEXTURE_3D:
   case TGSI_TEXTURE_CUBE:

      FETCH(&r[0], 0, CHAN_X);
      FETCH(&r[1], 0, CHAN_Y);
      FETCH(&r[2], 0, CHAN_Z);

      if (projected) {
         FETCH(&r[3], 0, CHAN_W);
         micro_div( &r[0], &r[0], &r[3] );
         micro_div( &r[1], &r[1], &r[3] );
         micro_div( &r[2], &r[2], &r[3] );
      }

      if (biasLod) {
         FETCH(&r[3], 0, CHAN_W);
         lodBias = r[3].f;
      }
      else
         lodBias = 0.0;

      fetch_texel(mach->Samplers[unit],
                  &r[0], &r[1], &r[2], lodBias,
                  &r[0], &r[1], &r[2], &r[3]);
      break;

   default:
      assert (0);
   }

   FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
      STORE( &r[chan_index], 0, chan_index );
   }
}

typedef void (* eval_coef_func)(
   struct tgsi_exec_machine_aos *mach,
   unsigned attrib,
   unsigned chan );

static void
exec_declaration(
   struct tgsi_exec_machine_aos *mach,
   const struct tgsi_full_declaration *decl )
{
   if( mach->Processor == TGSI_PROCESSOR_FRAGMENT ) {
      if( decl->Declaration.File == TGSI_FILE_INPUT ) {
         unsigned first, last, mask;
         eval_coef_func eval;

         first = decl->DeclarationRange.First;
         last = decl->DeclarationRange.Last;
         mask = decl->Declaration.UsageMask;

         switch( decl->Declaration.Interpolate ) {
         case TGSI_INTERPOLATE_CONSTANT:
         case TGSI_INTERPOLATE_LINEAR:
         case TGSI_INTERPOLATE_PERSPECTIVE:
            debug_assert(!"Not fragment interpolation");
            break;

         default:
            eval = NULL;
            assert( 0 );
         }

         if( mask == TGSI_WRITEMASK_XYZW ) {
            unsigned i, j;

            for( i = first; i <= last; i++ ) {
               for( j = 0; j < NUM_CHANNELS; j++ ) {
                  eval( mach, i, j );
               }
            }
         }
         else {
            unsigned i, j;

            for( j = 0; j < NUM_CHANNELS; j++ ) {
               if( mask & (1 << j) ) {
                  for( i = first; i <= last; i++ ) {
                     eval( mach, i, j );
                  }
               }
            }
         }
      }
   }
}

static void
exec_instruction(
   struct tgsi_exec_machine_aos *mach,
   const struct tgsi_full_instruction *inst,
   int *pc )
{
   uint chan_index;
   union tgsi_exec_channel_aos r[10];

   (*pc)++;

   switch (inst->Instruction.Opcode) {
   case TGSI_OPCODE_ARL:
   case TGSI_OPCODE_FLOOR:
   /* TGSI_OPCODE_FLR */
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH( &r[0], 0, chan_index );
         micro_flr( &r[0], &r[0] );
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_MOV:
   case TGSI_OPCODE_SWZ:
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH( &r[0], 0, chan_index );
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_LIT:
      if (IS_CHANNEL_ENABLED( *inst, CHAN_X )) {
         STORE( &mach->Temps[TEMP_1_I].xyzw[TEMP_1_C], 0, CHAN_X );
      }

      if (IS_CHANNEL_ENABLED( *inst, CHAN_Y ) || IS_CHANNEL_ENABLED( *inst, CHAN_Z )) {
         FETCH( &r[0], 0, CHAN_X );
         if (IS_CHANNEL_ENABLED( *inst, CHAN_Y )) {
            micro_max( &r[0], &r[0], &mach->Temps[TEMP_0_I].xyzw[TEMP_0_C] );
            STORE( &r[0], 0, CHAN_Y );
         }

         if (IS_CHANNEL_ENABLED( *inst, CHAN_Z )) {
            FETCH( &r[1], 0, CHAN_Y );
            micro_max( &r[1], &r[1], &mach->Temps[TEMP_0_I].xyzw[TEMP_0_C] );

            FETCH( &r[2], 0, CHAN_W );
            micro_min( &r[2], &r[2], &mach->Temps[TEMP_128_I].xyzw[TEMP_128_C] );
            micro_max( &r[2], &r[2], &mach->Temps[TEMP_M128_I].xyzw[TEMP_M128_C] );
            micro_pow( &r[1], &r[1], &r[2] );
            micro_lt( &r[0], &mach->Temps[TEMP_0_I].xyzw[TEMP_0_C], &r[0], &r[1], &mach->Temps[TEMP_0_I].xyzw[TEMP_0_C] );
            STORE( &r[0], 0, CHAN_Z );
         }
      }

      if (IS_CHANNEL_ENABLED( *inst, CHAN_W )) {
         STORE( &mach->Temps[TEMP_1_I].xyzw[TEMP_1_C], 0, CHAN_W );
      }
      break;

   case TGSI_OPCODE_RCP:
   /* TGSI_OPCODE_RECIP */
      FETCH( &r[0], 0, CHAN_X );
      micro_div( &r[0], &mach->Temps[TEMP_1_I].xyzw[TEMP_1_C], &r[0] );
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_RSQ:
   /* TGSI_OPCODE_RECIPSQRT */
      FETCH( &r[0], 0, CHAN_X );
      micro_abs( &r[0], &r[0] );
      micro_sqrt( &r[0], &r[0] );
      micro_div( &r[0], &mach->Temps[TEMP_1_I].xyzw[TEMP_1_C], &r[0] );
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_EXP:
      FETCH( &r[0], 0, CHAN_X );
      micro_flr( &r[1], &r[0] );  /* r1 = floor(r0) */
      if (IS_CHANNEL_ENABLED( *inst, CHAN_X )) {
         micro_exp2( &r[2], &r[1] );       /* r2 = 2 ^ r1 */
         STORE( &r[2], 0, CHAN_X );        /* store r2 */
      }
      if (IS_CHANNEL_ENABLED( *inst, CHAN_Y )) {
         micro_sub( &r[2], &r[0], &r[1] ); /* r2 = r0 - r1 */
         STORE( &r[2], 0, CHAN_Y );        /* store r2 */
      }
      if (IS_CHANNEL_ENABLED( *inst, CHAN_Z )) {
         micro_exp2( &r[2], &r[0] );       /* r2 = 2 ^ r0 */
         STORE( &r[2], 0, CHAN_Z );        /* store r2 */
      }
      if (IS_CHANNEL_ENABLED( *inst, CHAN_W )) {
         STORE( &mach->Temps[TEMP_1_I].xyzw[TEMP_1_C], 0, CHAN_W );
      }
      break;

   case TGSI_OPCODE_LOG:
      FETCH( &r[0], 0, CHAN_X );
      micro_abs( &r[2], &r[0] );  /* r2 = abs(r0) */
      micro_lg2( &r[1], &r[2] );  /* r1 = lg2(r2) */
      micro_flr( &r[0], &r[1] );  /* r0 = floor(r1) */
      if (IS_CHANNEL_ENABLED( *inst, CHAN_X )) {
         STORE( &r[0], 0, CHAN_X );
      }
      if (IS_CHANNEL_ENABLED( *inst, CHAN_Y )) {
         micro_exp2( &r[0], &r[0] );       /* r0 = 2 ^ r0 */
         micro_div( &r[0], &r[2], &r[0] ); /* r0 = r2 / r0 */
         STORE( &r[0], 0, CHAN_Y );
      }
      if (IS_CHANNEL_ENABLED( *inst, CHAN_Z )) {
         STORE( &r[1], 0, CHAN_Z );
      }
      if (IS_CHANNEL_ENABLED( *inst, CHAN_W )) {
         STORE( &mach->Temps[TEMP_1_I].xyzw[TEMP_1_C], 0, CHAN_W );
      }
      break;

   case TGSI_OPCODE_MUL:
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index )
      {
         FETCH(&r[0], 0, chan_index);
         FETCH(&r[1], 1, chan_index);

         micro_mul( &r[0], &r[0], &r[1] );

         STORE(&r[0], 0, chan_index);
      }
      break;

   case TGSI_OPCODE_ADD:
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH( &r[0], 0, chan_index );
         FETCH( &r[1], 1, chan_index );
         micro_add( &r[0], &r[0], &r[1] );
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_DP3:
   /* TGSI_OPCODE_DOT3 */
      FETCH( &r[0], 0, CHAN_X );
      FETCH( &r[1], 1, CHAN_X );
      micro_mul( &r[0], &r[0], &r[1] );

      FETCH( &r[1], 0, CHAN_Y );
      FETCH( &r[2], 1, CHAN_Y );
      micro_mul( &r[1], &r[1], &r[2] );
      micro_add( &r[0], &r[0], &r[1] );

      FETCH( &r[1], 0, CHAN_Z );
      FETCH( &r[2], 1, CHAN_Z );
      micro_mul( &r[1], &r[1], &r[2] );
      micro_add( &r[0], &r[0], &r[1] );

      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         STORE( &r[0], 0, chan_index );
      }
      break;

    case TGSI_OPCODE_DP4:
    /* TGSI_OPCODE_DOT4 */
       FETCH(&r[0], 0, CHAN_X);
       FETCH(&r[1], 1, CHAN_X);

       micro_mul( &r[0], &r[0], &r[1] );

       FETCH(&r[1], 0, CHAN_Y);
       FETCH(&r[2], 1, CHAN_Y);

       micro_mul( &r[1], &r[1], &r[2] );
       micro_add( &r[0], &r[0], &r[1] );

       FETCH(&r[1], 0, CHAN_Z);
       FETCH(&r[2], 1, CHAN_Z);

       micro_mul( &r[1], &r[1], &r[2] );
       micro_add( &r[0], &r[0], &r[1] );

       FETCH(&r[1], 0, CHAN_W);
       FETCH(&r[2], 1, CHAN_W);

       micro_mul( &r[1], &r[1], &r[2] );
       micro_add( &r[0], &r[0], &r[1] );

      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_DST:
      if (IS_CHANNEL_ENABLED( *inst, CHAN_X )) {
         STORE( &mach->Temps[TEMP_1_I].xyzw[TEMP_1_C], 0, CHAN_X );
      }

      if (IS_CHANNEL_ENABLED( *inst, CHAN_Y )) {
         FETCH( &r[0], 0, CHAN_Y );
         FETCH( &r[1], 1, CHAN_Y);
         micro_mul( &r[0], &r[0], &r[1] );
         STORE( &r[0], 0, CHAN_Y );
      }

      if (IS_CHANNEL_ENABLED( *inst, CHAN_Z )) {
         FETCH( &r[0], 0, CHAN_Z );
         STORE( &r[0], 0, CHAN_Z );
      }

      if (IS_CHANNEL_ENABLED( *inst, CHAN_W )) {
         FETCH( &r[0], 1, CHAN_W );
         STORE( &r[0], 0, CHAN_W );
      }
      break;

   case TGSI_OPCODE_MIN:
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH(&r[0], 0, chan_index);
         FETCH(&r[1], 1, chan_index);

         /* XXX use micro_min()?? */
         micro_lt( &r[0], &r[0], &r[1], &r[0], &r[1] );

         STORE(&r[0], 0, chan_index);
      }
      break;

   case TGSI_OPCODE_MAX:
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH(&r[0], 0, chan_index);
         FETCH(&r[1], 1, chan_index);

         /* XXX use micro_max()?? */
         micro_lt( &r[0], &r[0], &r[1], &r[1], &r[0] );

         STORE(&r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_SLT:
   /* TGSI_OPCODE_SETLT */
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH( &r[0], 0, chan_index );
         FETCH( &r[1], 1, chan_index );
         micro_lt( &r[0], &r[0], &r[1], &mach->Temps[TEMP_1_I].xyzw[TEMP_1_C], &mach->Temps[TEMP_0_I].xyzw[TEMP_0_C] );
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_SGE:
   /* TGSI_OPCODE_SETGE */
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH( &r[0], 0, chan_index );
         FETCH( &r[1], 1, chan_index );
         micro_le( &r[0], &r[1], &r[0], &mach->Temps[TEMP_1_I].xyzw[TEMP_1_C], &mach->Temps[TEMP_0_I].xyzw[TEMP_0_C] );
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_MAD:
   /* TGSI_OPCODE_MADD */
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH( &r[0], 0, chan_index );
         FETCH( &r[1], 1, chan_index );
         micro_mul( &r[0], &r[0], &r[1] );
         FETCH( &r[1], 2, chan_index );
         micro_add( &r[0], &r[0], &r[1] );
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_SUB:
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH(&r[0], 0, chan_index);
         FETCH(&r[1], 1, chan_index);

         micro_sub( &r[0], &r[0], &r[1] );

         STORE(&r[0], 0, chan_index);
      }
      break;

   case TGSI_OPCODE_LERP:
   /* TGSI_OPCODE_LRP */
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH(&r[0], 0, chan_index);
         FETCH(&r[1], 1, chan_index);
         FETCH(&r[2], 2, chan_index);

         micro_sub( &r[1], &r[1], &r[2] );
         micro_mul( &r[0], &r[0], &r[1] );
         micro_add( &r[0], &r[0], &r[2] );

         STORE(&r[0], 0, chan_index);
      }
      break;

   case TGSI_OPCODE_CND:
      FOR_EACH_ENABLED_CHANNEL(*inst, chan_index) {
         FETCH(&r[0], 0, chan_index);
         FETCH(&r[1], 1, chan_index);
         FETCH(&r[2], 2, chan_index);
         micro_lt(&r[0], &mach->Temps[TEMP_HALF_I].xyzw[TEMP_HALF_C], &r[2], &r[0], &r[1]);
         STORE(&r[0], 0, chan_index);
      }
      break;

   case TGSI_OPCODE_CND0:
      FOR_EACH_ENABLED_CHANNEL(*inst, chan_index) {
         FETCH(&r[0], 0, chan_index);
         FETCH(&r[1], 1, chan_index);
         FETCH(&r[2], 2, chan_index);
         micro_le(&r[0], &mach->Temps[TEMP_0_I].xyzw[TEMP_0_C], &r[2], &r[0], &r[1]);
         STORE(&r[0], 0, chan_index);
      }
      break;

   case TGSI_OPCODE_DOT2ADD:
   /* TGSI_OPCODE_DP2A */
      FETCH( &r[0], 0, CHAN_X );
      FETCH( &r[1], 1, CHAN_X );
      micro_mul( &r[0], &r[0], &r[1] );

      FETCH( &r[1], 0, CHAN_Y );
      FETCH( &r[2], 1, CHAN_Y );
      micro_mul( &r[1], &r[1], &r[2] );
      micro_add( &r[0], &r[0], &r[1] );

      FETCH( &r[2], 2, CHAN_X );
      micro_add( &r[0], &r[0], &r[2] );

      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_INDEX:
      /* XXX: considered for removal */
      assert (0);
      break;

   case TGSI_OPCODE_NEGATE:
      /* XXX: considered for removal */
      assert (0);
      break;

   case TGSI_OPCODE_FRAC:
   /* TGSI_OPCODE_FRC */
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH( &r[0], 0, chan_index );
         micro_frc( &r[0], &r[0] );
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_CLAMP:
      FOR_EACH_ENABLED_CHANNEL(*inst, chan_index) {
         FETCH(&r[0], 0, chan_index);
         FETCH(&r[1], 1, chan_index);
         micro_max(&r[0], &r[0], &r[1]);
         FETCH(&r[1], 2, chan_index);
         micro_min(&r[0], &r[0], &r[1]);
         STORE(&r[0], 0, chan_index);
      }
      break;

   case TGSI_OPCODE_ROUND:
   case TGSI_OPCODE_ARR:
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH( &r[0], 0, chan_index );
         micro_rnd( &r[0], &r[0] );
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_EXPBASE2:
   /* TGSI_OPCODE_EX2 */
      FETCH(&r[0], 0, CHAN_X);

#if FAST_MATH
      micro_exp2( &r[0], &r[0] );
#else
      micro_pow( &r[0], &mach->Temps[TEMP_2_I].xyzw[TEMP_2_C], &r[0] );
#endif

      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_LOGBASE2:
   /* TGSI_OPCODE_LG2 */
      FETCH( &r[0], 0, CHAN_X );
      micro_lg2( &r[0], &r[0] );
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_POWER:
   /* TGSI_OPCODE_POW */
      FETCH(&r[0], 0, CHAN_X);
      FETCH(&r[1], 1, CHAN_X);

      micro_pow( &r[0], &r[0], &r[1] );

      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_CROSSPRODUCT:
   /* TGSI_OPCODE_XPD */
      FETCH(&r[0], 0, CHAN_Y);
      FETCH(&r[1], 1, CHAN_Z);

      micro_mul( &r[2], &r[0], &r[1] );

      FETCH(&r[3], 0, CHAN_Z);
      FETCH(&r[4], 1, CHAN_Y);

      micro_mul( &r[5], &r[3], &r[4] );
      micro_sub( &r[2], &r[2], &r[5] );

      if (IS_CHANNEL_ENABLED( *inst, CHAN_X )) {
         STORE( &r[2], 0, CHAN_X );
      }

      FETCH(&r[2], 1, CHAN_X);

      micro_mul( &r[3], &r[3], &r[2] );

      FETCH(&r[5], 0, CHAN_X);

      micro_mul( &r[1], &r[1], &r[5] );
      micro_sub( &r[3], &r[3], &r[1] );

      if (IS_CHANNEL_ENABLED( *inst, CHAN_Y )) {
         STORE( &r[3], 0, CHAN_Y );
      }

      micro_mul( &r[5], &r[5], &r[4] );
      micro_mul( &r[0], &r[0], &r[2] );
      micro_sub( &r[5], &r[5], &r[0] );

      if (IS_CHANNEL_ENABLED( *inst, CHAN_Z )) {
         STORE( &r[5], 0, CHAN_Z );
      }

      if (IS_CHANNEL_ENABLED( *inst, CHAN_W )) {
         STORE( &mach->Temps[TEMP_1_I].xyzw[TEMP_1_C], 0, CHAN_W );
      }
      break;

    case TGSI_OPCODE_MULTIPLYMATRIX:
       /* XXX: considered for removal */
       assert (0);
       break;

    case TGSI_OPCODE_ABS:
       FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
          FETCH(&r[0], 0, chan_index);

          micro_abs( &r[0], &r[0] );

          STORE(&r[0], 0, chan_index);
       }
       break;

   case TGSI_OPCODE_RCC:
      FETCH(&r[0], 0, CHAN_X);
      micro_div(&r[0], &mach->Temps[TEMP_1_I].xyzw[TEMP_1_C], &r[0]);
      micro_float_clamp(&r[0], &r[0]);
      FOR_EACH_ENABLED_CHANNEL(*inst, chan_index) {
         STORE(&r[0], 0, chan_index);
      }
      break;

   case TGSI_OPCODE_DPH:
      FETCH(&r[0], 0, CHAN_X);
      FETCH(&r[1], 1, CHAN_X);

      micro_mul( &r[0], &r[0], &r[1] );

      FETCH(&r[1], 0, CHAN_Y);
      FETCH(&r[2], 1, CHAN_Y);

      micro_mul( &r[1], &r[1], &r[2] );
      micro_add( &r[0], &r[0], &r[1] );

      FETCH(&r[1], 0, CHAN_Z);
      FETCH(&r[2], 1, CHAN_Z);

      micro_mul( &r[1], &r[1], &r[2] );
      micro_add( &r[0], &r[0], &r[1] );

      FETCH(&r[1], 1, CHAN_W);

      micro_add( &r[0], &r[0], &r[1] );

      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_COS:
      FETCH(&r[0], 0, CHAN_X);

      micro_cos( &r[0], &r[0] );

      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_DDX:
   case TGSI_OPCODE_DDY:
      debug_assert(!"Fragment processing not allowed on aos paths!");
      break;

   case TGSI_OPCODE_KILP:
      exec_kilp (mach, inst);
      break;

   case TGSI_OPCODE_KIL:
      exec_kil (mach, inst);
      break;

   case TGSI_OPCODE_PK2H:
      assert (0);
      break;

   case TGSI_OPCODE_PK2US:
      assert (0);
      break;

   case TGSI_OPCODE_PK4B:
      assert (0);
      break;

   case TGSI_OPCODE_PK4UB:
      assert (0);
      break;

   case TGSI_OPCODE_RFL:
      if (IS_CHANNEL_ENABLED(*inst, CHAN_X) ||
          IS_CHANNEL_ENABLED(*inst, CHAN_Y) ||
          IS_CHANNEL_ENABLED(*inst, CHAN_Z)) {
         /* r0 = dp3(src0, src0) */
         FETCH(&r[2], 0, CHAN_X);
         micro_mul(&r[0], &r[2], &r[2]);
         FETCH(&r[4], 0, CHAN_Y);
         micro_mul(&r[8], &r[4], &r[4]);
         micro_add(&r[0], &r[0], &r[8]);
         FETCH(&r[6], 0, CHAN_Z);
         micro_mul(&r[8], &r[6], &r[6]);
         micro_add(&r[0], &r[0], &r[8]);

         /* r1 = dp3(src0, src1) */
         FETCH(&r[3], 1, CHAN_X);
         micro_mul(&r[1], &r[2], &r[3]);
         FETCH(&r[5], 1, CHAN_Y);
         micro_mul(&r[8], &r[4], &r[5]);
         micro_add(&r[1], &r[1], &r[8]);
         FETCH(&r[7], 1, CHAN_Z);
         micro_mul(&r[8], &r[6], &r[7]);
         micro_add(&r[1], &r[1], &r[8]);

         /* r1 = 2 * r1 / r0 */
         micro_add(&r[1], &r[1], &r[1]);
         micro_div(&r[1], &r[1], &r[0]);

         if (IS_CHANNEL_ENABLED(*inst, CHAN_X)) {
            micro_mul(&r[2], &r[2], &r[1]);
            micro_sub(&r[2], &r[2], &r[3]);
            STORE(&r[2], 0, CHAN_X);
         }
         if (IS_CHANNEL_ENABLED(*inst, CHAN_Y)) {
            micro_mul(&r[4], &r[4], &r[1]);
            micro_sub(&r[4], &r[4], &r[5]);
            STORE(&r[4], 0, CHAN_Y);
         }
         if (IS_CHANNEL_ENABLED(*inst, CHAN_Z)) {
            micro_mul(&r[6], &r[6], &r[1]);
            micro_sub(&r[6], &r[6], &r[7]);
            STORE(&r[6], 0, CHAN_Z);
         }
      }
      if (IS_CHANNEL_ENABLED(*inst, CHAN_W)) {
         STORE(&mach->Temps[TEMP_1_I].xyzw[TEMP_1_C], 0, CHAN_W);
      }
      break;

   case TGSI_OPCODE_SEQ:
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH( &r[0], 0, chan_index );
         FETCH( &r[1], 1, chan_index );
         micro_eq( &r[0], &r[0], &r[1],
                   &mach->Temps[TEMP_1_I].xyzw[TEMP_1_C],
                   &mach->Temps[TEMP_0_I].xyzw[TEMP_0_C] );
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_SFL:
      FOR_EACH_ENABLED_CHANNEL(*inst, chan_index) {
         STORE(&mach->Temps[TEMP_0_I].xyzw[TEMP_0_C], 0, chan_index);
      }
      break;

   case TGSI_OPCODE_SGT:
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH( &r[0], 0, chan_index );
         FETCH( &r[1], 1, chan_index );
         micro_le( &r[0], &r[0], &r[1], &mach->Temps[TEMP_0_I].xyzw[TEMP_0_C], &mach->Temps[TEMP_1_I].xyzw[TEMP_1_C] );
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_SIN:
      FETCH( &r[0], 0, CHAN_X );
      micro_sin( &r[0], &r[0] );
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_SLE:
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH( &r[0], 0, chan_index );
         FETCH( &r[1], 1, chan_index );
         micro_le( &r[0], &r[0], &r[1], &mach->Temps[TEMP_1_I].xyzw[TEMP_1_C], &mach->Temps[TEMP_0_I].xyzw[TEMP_0_C] );
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_SNE:
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH( &r[0], 0, chan_index );
         FETCH( &r[1], 1, chan_index );
         micro_eq( &r[0], &r[0], &r[1], &mach->Temps[TEMP_0_I].xyzw[TEMP_0_C], &mach->Temps[TEMP_1_I].xyzw[TEMP_1_C] );
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_STR:
      FOR_EACH_ENABLED_CHANNEL(*inst, chan_index) {
         STORE(&mach->Temps[TEMP_1_I].xyzw[TEMP_1_C], 0, chan_index);
      }
      break;

   case TGSI_OPCODE_TEX:
      /* simple texture lookup */
      /* src[0] = texcoord */
      /* src[1] = sampler unit */
      exec_tex(mach, inst, FALSE, FALSE);
      break;

   case TGSI_OPCODE_TXB:
      /* Texture lookup with lod bias */
      /* src[0] = texcoord (src[0].w = LOD bias) */
      /* src[1] = sampler unit */
      exec_tex(mach, inst, TRUE, FALSE);
      break;

   case TGSI_OPCODE_TXD:
      /* Texture lookup with explict partial derivatives */
      /* src[0] = texcoord */
      /* src[1] = d[strq]/dx */
      /* src[2] = d[strq]/dy */
      /* src[3] = sampler unit */
      assert (0);
      break;

   case TGSI_OPCODE_TXL:
      /* Texture lookup with explit LOD */
      /* src[0] = texcoord (src[0].w = LOD) */
      /* src[1] = sampler unit */
      exec_tex(mach, inst, TRUE, FALSE);
      break;

   case TGSI_OPCODE_TXP:
      /* Texture lookup with projection */
      /* src[0] = texcoord (src[0].w = projection) */
      /* src[1] = sampler unit */
      exec_tex(mach, inst, FALSE, TRUE);
      break;

   case TGSI_OPCODE_UP2H:
      assert (0);
      break;

   case TGSI_OPCODE_UP2US:
      assert (0);
      break;

   case TGSI_OPCODE_UP4B:
      assert (0);
      break;

   case TGSI_OPCODE_UP4UB:
      assert (0);
      break;

   case TGSI_OPCODE_X2D:
      FETCH(&r[0], 1, CHAN_X);
      FETCH(&r[1], 1, CHAN_Y);
      if (IS_CHANNEL_ENABLED(*inst, CHAN_X) ||
          IS_CHANNEL_ENABLED(*inst, CHAN_Z)) {
         FETCH(&r[2], 2, CHAN_X);
         micro_mul(&r[2], &r[2], &r[0]);
         FETCH(&r[3], 2, CHAN_Y);
         micro_mul(&r[3], &r[3], &r[1]);
         micro_add(&r[2], &r[2], &r[3]);
         FETCH(&r[3], 0, CHAN_X);
         micro_add(&r[2], &r[2], &r[3]);
         if (IS_CHANNEL_ENABLED(*inst, CHAN_X)) {
            STORE(&r[2], 0, CHAN_X);
         }
         if (IS_CHANNEL_ENABLED(*inst, CHAN_Z)) {
            STORE(&r[2], 0, CHAN_Z);
         }
      }
      if (IS_CHANNEL_ENABLED(*inst, CHAN_Y) ||
          IS_CHANNEL_ENABLED(*inst, CHAN_W)) {
         FETCH(&r[2], 2, CHAN_Z);
         micro_mul(&r[2], &r[2], &r[0]);
         FETCH(&r[3], 2, CHAN_W);
         micro_mul(&r[3], &r[3], &r[1]);
         micro_add(&r[2], &r[2], &r[3]);
         FETCH(&r[3], 0, CHAN_Y);
         micro_add(&r[2], &r[2], &r[3]);
         if (IS_CHANNEL_ENABLED(*inst, CHAN_Y)) {
            STORE(&r[2], 0, CHAN_Y);
         }
         if (IS_CHANNEL_ENABLED(*inst, CHAN_W)) {
            STORE(&r[2], 0, CHAN_W);
         }
      }
      break;

   case TGSI_OPCODE_ARA:
      assert (0);
      break;

   case TGSI_OPCODE_BRA:
      assert (0);
      break;

   case TGSI_OPCODE_CAL:
      /* skip the call if no execution channels are enabled */
      if (mach->ExecMask) {
         /* do the call */

         /* push the Cond, Loop, Cont stacks */
         assert(mach->CondStackTop < TGSI_EXEC_MAX_COND_NESTING);
         mach->CondStack[mach->CondStackTop++] = mach->CondMask;
         assert(mach->LoopStackTop < TGSI_EXEC_MAX_LOOP_NESTING);
         mach->LoopStack[mach->LoopStackTop++] = mach->LoopMask;
         assert(mach->ContStackTop < TGSI_EXEC_MAX_LOOP_NESTING);
         mach->ContStack[mach->ContStackTop++] = mach->ContMask;

         assert(mach->FuncStackTop < TGSI_EXEC_MAX_CALL_NESTING);
         mach->FuncStack[mach->FuncStackTop++] = mach->FuncMask;

         /* note that PC was already incremented above */
         mach->CallStack[mach->CallStackTop++] = *pc;
         *pc = inst->InstructionExtLabel.Label;
      }
      break;

   case TGSI_OPCODE_RET:
      mach->FuncMask &= ~mach->ExecMask;
      UPDATE_EXEC_MASK(mach);

      if (mach->FuncMask == 0x0) {
         /* really return now (otherwise, keep executing */

         if (mach->CallStackTop == 0) {
            /* returning from main() */
            *pc = -1;
            return;
         }
         *pc = mach->CallStack[--mach->CallStackTop];

         /* pop the Cond, Loop, Cont stacks */
         assert(mach->CondStackTop > 0);
         mach->CondMask = mach->CondStack[--mach->CondStackTop];
         assert(mach->LoopStackTop > 0);
         mach->LoopMask = mach->LoopStack[--mach->LoopStackTop];
         assert(mach->ContStackTop > 0);
         mach->ContMask = mach->ContStack[--mach->ContStackTop];
         assert(mach->FuncStackTop > 0);
         mach->FuncMask = mach->FuncStack[--mach->FuncStackTop];

         UPDATE_EXEC_MASK(mach);
      }
      break;

   case TGSI_OPCODE_SSG:
   /* TGSI_OPCODE_SGN */
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH( &r[0], 0, chan_index );
         micro_sgn( &r[0], &r[0] );
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_CMP:
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH(&r[0], 0, chan_index);
         FETCH(&r[1], 1, chan_index);
         FETCH(&r[2], 2, chan_index);

         micro_lt( &r[0], &r[0], &mach->Temps[TEMP_0_I].xyzw[TEMP_0_C], &r[1], &r[2] );

         STORE(&r[0], 0, chan_index);
      }
      break;

   case TGSI_OPCODE_SCS:
      if( IS_CHANNEL_ENABLED( *inst, CHAN_X ) || IS_CHANNEL_ENABLED( *inst, CHAN_Y ) ) {
         FETCH( &r[0], 0, CHAN_X );
         if (IS_CHANNEL_ENABLED(*inst, CHAN_X)) {
            micro_cos(&r[1], &r[0]);
            STORE(&r[1], 0, CHAN_X);
         }
         if (IS_CHANNEL_ENABLED(*inst, CHAN_Y)) {
            micro_sin(&r[1], &r[0]);
            STORE(&r[1], 0, CHAN_Y);
         }
      }
      if( IS_CHANNEL_ENABLED( *inst, CHAN_Z ) ) {
         STORE( &mach->Temps[TEMP_0_I].xyzw[TEMP_0_C], 0, CHAN_Z );
      }
      if( IS_CHANNEL_ENABLED( *inst, CHAN_W ) ) {
         STORE( &mach->Temps[TEMP_1_I].xyzw[TEMP_1_C], 0, CHAN_W );
      }
      break;

   case TGSI_OPCODE_NRM:
      /* 3-component vector normalize */
      if(IS_CHANNEL_ENABLED(*inst, CHAN_X) ||
         IS_CHANNEL_ENABLED(*inst, CHAN_Y) ||
         IS_CHANNEL_ENABLED(*inst, CHAN_Z)) {
         /* r3 = sqrt(dp3(src0, src0)) */
         FETCH(&r[0], 0, CHAN_X);
         micro_mul(&r[3], &r[0], &r[0]);
         FETCH(&r[1], 0, CHAN_Y);
         micro_mul(&r[4], &r[1], &r[1]);
         micro_add(&r[3], &r[3], &r[4]);
         FETCH(&r[2], 0, CHAN_Z);
         micro_mul(&r[4], &r[2], &r[2]);
         micro_add(&r[3], &r[3], &r[4]);
         micro_sqrt(&r[3], &r[3]);

         if (IS_CHANNEL_ENABLED(*inst, CHAN_X)) {
            micro_div(&r[0], &r[0], &r[3]);
            STORE(&r[0], 0, CHAN_X);
         }
         if (IS_CHANNEL_ENABLED(*inst, CHAN_Y)) {
            micro_div(&r[1], &r[1], &r[3]);
            STORE(&r[1], 0, CHAN_Y);
         }
         if (IS_CHANNEL_ENABLED(*inst, CHAN_Z)) {
            micro_div(&r[2], &r[2], &r[3]);
            STORE(&r[2], 0, CHAN_Z);
         }
      }
      if (IS_CHANNEL_ENABLED(*inst, CHAN_W)) {
         STORE(&mach->Temps[TEMP_1_I].xyzw[TEMP_1_C], 0, CHAN_W);
      }
      break;

   case TGSI_OPCODE_NRM4:
      /* 4-component vector normalize */
      {
         union tgsi_exec_channel_aos tmp, dot;

         /* tmp = dp4(src0, src0): */
         FETCH( &r[0], 0, CHAN_X );
         micro_mul( &tmp, &r[0], &r[0] );

         FETCH( &r[1], 0, CHAN_Y );
         micro_mul( &dot, &r[1], &r[1] );
         micro_add( &tmp, &tmp, &dot );

         FETCH( &r[2], 0, CHAN_Z );
         micro_mul( &dot, &r[2], &r[2] );
         micro_add( &tmp, &tmp, &dot );

         FETCH( &r[3], 0, CHAN_W );
         micro_mul( &dot, &r[3], &r[3] );
         micro_add( &tmp, &tmp, &dot );

         /* tmp = 1 / sqrt(tmp) */
         micro_sqrt( &tmp, &tmp );
         micro_div( &tmp, &mach->Temps[TEMP_1_I].xyzw[TEMP_1_C], &tmp );

         FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
            /* chan = chan * tmp */
            micro_mul( &r[chan_index], &tmp, &r[chan_index] );
            STORE( &r[chan_index], 0, chan_index );
         }
      }
      break;

   case TGSI_OPCODE_DIV:
      assert( 0 );
      break;

   case TGSI_OPCODE_DP2:
      FETCH( &r[0], 0, CHAN_X );
      FETCH( &r[1], 1, CHAN_X );
      micro_mul( &r[0], &r[0], &r[1] );

      FETCH( &r[1], 0, CHAN_Y );
      FETCH( &r[2], 1, CHAN_Y );
      micro_mul( &r[1], &r[1], &r[2] );
      micro_add( &r[0], &r[0], &r[1] );

      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_IF:
      /* push CondMask */
      assert(mach->CondStackTop < TGSI_EXEC_MAX_COND_NESTING);
      mach->CondStack[mach->CondStackTop++] = mach->CondMask;
      FETCH( &r[0], 0, CHAN_X );
      /* update CondMask */
      if( ! r[0].u ) {
         mach->CondMask &= ~0x1;
      }
      UPDATE_EXEC_MASK(mach);
      /* Todo: If CondMask==0, jump to ELSE */
      break;

   case TGSI_OPCODE_ELSE:
      /* invert CondMask wrt previous mask */
      {
         uint prevMask;
         assert(mach->CondStackTop > 0);
         prevMask = mach->CondStack[mach->CondStackTop - 1];
         mach->CondMask = ~mach->CondMask & prevMask;
         UPDATE_EXEC_MASK(mach);
         /* Todo: If CondMask==0, jump to ENDIF */
      }
      break;

   case TGSI_OPCODE_ENDIF:
      /* pop CondMask */
      assert(mach->CondStackTop > 0);
      mach->CondMask = mach->CondStack[--mach->CondStackTop];
      UPDATE_EXEC_MASK(mach);
      break;

   case TGSI_OPCODE_END:
      /* halt execution */
      *pc = -1;
      break;

   case TGSI_OPCODE_REP:
      assert (0);
      break;

   case TGSI_OPCODE_ENDREP:
       assert (0);
       break;

   case TGSI_OPCODE_PUSHA:
      assert (0);
      break;

   case TGSI_OPCODE_POPA:
      assert (0);
      break;

   case TGSI_OPCODE_CEIL:
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH( &r[0], 0, chan_index );
         micro_ceil( &r[0], &r[0] );
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_I2F:
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH( &r[0], 0, chan_index );
         micro_i2f( &r[0], &r[0] );
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_NOT:
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH( &r[0], 0, chan_index );
         micro_not( &r[0], &r[0] );
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_TRUNC:
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH( &r[0], 0, chan_index );
         micro_trunc( &r[0], &r[0] );
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_SHL:
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH( &r[0], 0, chan_index );
         FETCH( &r[1], 1, chan_index );
         micro_shl( &r[0], &r[0], &r[1] );
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_SHR:
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH( &r[0], 0, chan_index );
         FETCH( &r[1], 1, chan_index );
         micro_ishr( &r[0], &r[0], &r[1] );
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_AND:
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH( &r[0], 0, chan_index );
         FETCH( &r[1], 1, chan_index );
         micro_and( &r[0], &r[0], &r[1] );
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_OR:
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH( &r[0], 0, chan_index );
         FETCH( &r[1], 1, chan_index );
         micro_or( &r[0], &r[0], &r[1] );
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_MOD:
      assert (0);
      break;

   case TGSI_OPCODE_XOR:
      FOR_EACH_ENABLED_CHANNEL( *inst, chan_index ) {
         FETCH( &r[0], 0, chan_index );
         FETCH( &r[1], 1, chan_index );
         micro_xor( &r[0], &r[0], &r[1] );
         STORE( &r[0], 0, chan_index );
      }
      break;

   case TGSI_OPCODE_SAD:
      assert (0);
      break;

   case TGSI_OPCODE_TXF:
      assert (0);
      break;

   case TGSI_OPCODE_TXQ:
      assert (0);
      break;

   case TGSI_OPCODE_EMIT:
      mach->Temps[TEMP_OUTPUT_I].xyzw[TEMP_OUTPUT_C].u += 16;
      mach->Primitives[mach->Temps[TEMP_PRIMITIVE_I].xyzw[TEMP_PRIMITIVE_C].u]++;
      break;

   case TGSI_OPCODE_ENDPRIM:
      mach->Temps[TEMP_PRIMITIVE_I].xyzw[TEMP_PRIMITIVE_C].u++;
      mach->Primitives[mach->Temps[TEMP_PRIMITIVE_I].xyzw[TEMP_PRIMITIVE_C].u] = 0;
      break;

   case TGSI_OPCODE_LOOP:
      /* fall-through (for now) */
   case TGSI_OPCODE_BGNLOOP2:
      /* push LoopMask and ContMasks */
      assert(mach->LoopStackTop < TGSI_EXEC_MAX_LOOP_NESTING);
      mach->LoopStack[mach->LoopStackTop++] = mach->LoopMask;
      assert(mach->ContStackTop < TGSI_EXEC_MAX_LOOP_NESTING);
      mach->ContStack[mach->ContStackTop++] = mach->ContMask;
      break;

   case TGSI_OPCODE_ENDLOOP:
      /* fall-through (for now at least) */
   case TGSI_OPCODE_ENDLOOP2:
      /* Restore ContMask, but don't pop */
      assert(mach->ContStackTop > 0);
      mach->ContMask = mach->ContStack[mach->ContStackTop - 1];
      UPDATE_EXEC_MASK(mach);
      if (mach->ExecMask) {
         /* repeat loop: jump to instruction just past BGNLOOP */
         *pc = inst->InstructionExtLabel.Label + 1;
      }
      else {
         /* exit loop: pop LoopMask */
         assert(mach->LoopStackTop > 0);
         mach->LoopMask = mach->LoopStack[--mach->LoopStackTop];
         /* pop ContMask */
         assert(mach->ContStackTop > 0);
         mach->ContMask = mach->ContStack[--mach->ContStackTop];
      }
      UPDATE_EXEC_MASK(mach);
      break;

   case TGSI_OPCODE_BRK:
      /* turn off loop channels for each enabled exec channel */
      mach->LoopMask &= ~mach->ExecMask;
      /* Todo: if mach->LoopMask == 0, jump to end of loop */
      UPDATE_EXEC_MASK(mach);
      break;

   case TGSI_OPCODE_CONT:
      /* turn off cont channels for each enabled exec channel */
      mach->ContMask &= ~mach->ExecMask;
      /* Todo: if mach->LoopMask == 0, jump to end of loop */
      UPDATE_EXEC_MASK(mach);
      break;

   case TGSI_OPCODE_BGNSUB:
      /* no-op */
      break;

   case TGSI_OPCODE_ENDSUB:
      /* no-op */
      break;

   case TGSI_OPCODE_NOISE1:
      assert( 0 );
      break;

   case TGSI_OPCODE_NOISE2:
      assert( 0 );
      break;

   case TGSI_OPCODE_NOISE3:
      assert( 0 );
      break;

   case TGSI_OPCODE_NOISE4:
      assert( 0 );
      break;

   case TGSI_OPCODE_NOP:
      break;

   default:
      assert( 0 );
   }
}


/**
 * Run TGSI interpreter.
 * \return bitmask of "alive" quad components
 */
uint
tgsi_exec_machine_aos_run( struct tgsi_exec_machine_aos *mach )
{
   uint i;
   int pc = 0;

   debug_assert(mach->Processor == TGSI_PROCESSOR_VERTEX ||
                mach->Processor == TGSI_PROCESSOR_GEOMETRY);

   mach->CondMask = 0xf;
   mach->LoopMask = 0xf;
   mach->ContMask = 0xf;
   mach->FuncMask = 0xf;
   mach->ExecMask = 0xf;

   mach->CondStackTop = 0; /* temporarily subvert this assertion */
   assert(mach->CondStackTop == 0);
   assert(mach->LoopStackTop == 0);
   assert(mach->ContStackTop == 0);
   assert(mach->CallStackTop == 0);

   mach->Temps[TEMP_KILMASK_I].xyzw[TEMP_KILMASK_C].u = 0;
   mach->Temps[TEMP_OUTPUT_I].xyzw[TEMP_OUTPUT_C].u = 0;

   if( mach->Processor == TGSI_PROCESSOR_GEOMETRY ) {
      mach->Temps[TEMP_PRIMITIVE_I].xyzw[TEMP_PRIMITIVE_C].u = 0;
      mach->Primitives[0] = 0;
   }

   mach->Temps[TEMP_CC_I].xyzw[TEMP_CC_C].u =
      (TGSI_EXEC_CC_EQ << TGSI_EXEC_CC_X_SHIFT) |
      (TGSI_EXEC_CC_EQ << TGSI_EXEC_CC_Y_SHIFT) |
      (TGSI_EXEC_CC_EQ << TGSI_EXEC_CC_Z_SHIFT) |
      (TGSI_EXEC_CC_EQ << TGSI_EXEC_CC_W_SHIFT);

   /* execute declarations (interpolants) */
   for (i = 0; i < mach->NumDeclarations; i++) {
      exec_declaration( mach, mach->Declarations+i );
   }

   /* execute instructions, until pc is set to -1 */
   while (pc != -1) {
      assert(pc < (int) mach->NumInstructions);
      exec_instruction( mach, mach->Instructions + pc, &pc );
   }

#if 0
   /* we scale from floats in [0,1] to Zbuffer ints in sp_quad_depth_test.c */
   if (mach->Processor == TGSI_PROCESSOR_FRAGMENT) {
      /*
       * Scale back depth component.
       */
      for (i = 0; i < 4; i++)
         mach->Outputs[0].xyzw[2].f[i] *= ctx->DrawBuffer->_DepthMaxF;
   }
#endif

   return ~mach->Temps[TEMP_KILMASK_I].xyzw[TEMP_KILMASK_C].u;
}
