
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 1



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "program_parse.y"

/*
 * Copyright © 2009 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main/mtypes.h"
#include "main/imports.h"
#include "program.h"
#include "prog_parameter.h"
#include "prog_parameter_layout.h"
#include "prog_statevars.h"
#include "prog_instruction.h"

#include "symbol_table.h"
#include "program_parser.h"

extern void *yy_scan_string(char *);
extern void yy_delete_buffer(void *);

static struct asm_symbol *declare_variable(struct asm_parser_state *state,
    char *name, enum asm_type t, struct YYLTYPE *locp);

static int add_state_reference(struct gl_program_parameter_list *param_list,
    const gl_state_index tokens[STATE_LENGTH]);

static int initialize_symbol_from_state(struct gl_program *prog,
    struct asm_symbol *param_var, const gl_state_index tokens[STATE_LENGTH]);

static int initialize_symbol_from_param(struct gl_program *prog,
    struct asm_symbol *param_var, const gl_state_index tokens[STATE_LENGTH]);

static int initialize_symbol_from_const(struct gl_program *prog,
    struct asm_symbol *param_var, const struct asm_vector *vec);

static int yyparse(struct asm_parser_state *state);

static char *make_error_string(const char *fmt, ...);

static void yyerror(struct YYLTYPE *locp, struct asm_parser_state *state,
    const char *s);

static int validate_inputs(struct YYLTYPE *locp,
    struct asm_parser_state *state);

static void init_dst_reg(struct prog_dst_register *r);

static void init_src_reg(struct asm_src_register *r);

static void asm_instruction_set_operands(struct asm_instruction *inst,
    const struct prog_dst_register *dst, const struct asm_src_register *src0,
    const struct asm_src_register *src1, const struct asm_src_register *src2);

static struct asm_instruction *asm_instruction_ctor(gl_inst_opcode op,
    const struct prog_dst_register *dst, const struct asm_src_register *src0,
    const struct asm_src_register *src1, const struct asm_src_register *src2);

static struct asm_instruction *asm_instruction_copy_ctor(
    const struct prog_instruction *base, const struct prog_dst_register *dst,
    const struct asm_src_register *src0, const struct asm_src_register *src1,
    const struct asm_src_register *src2);

#ifndef FALSE
#define FALSE 0
#define TRUE (!FALSE)
#endif

#define YYLLOC_DEFAULT(Current, Rhs, N)					\
   do {									\
      if (YYID(N)) {							\
	 (Current).first_line = YYRHSLOC(Rhs, 1).first_line;		\
	 (Current).first_column = YYRHSLOC(Rhs, 1).first_column;	\
	 (Current).position = YYRHSLOC(Rhs, 1).position;		\
	 (Current).last_line = YYRHSLOC(Rhs, N).last_line;		\
	 (Current).last_column = YYRHSLOC(Rhs, N).last_column;		\
      } else {								\
	 (Current).first_line = YYRHSLOC(Rhs, 0).last_line;		\
	 (Current).last_line = (Current).first_line;			\
	 (Current).first_column = YYRHSLOC(Rhs, 0).last_column;		\
	 (Current).last_column = (Current).first_column;		\
	 (Current).position = YYRHSLOC(Rhs, 0).position			\
	    + (Current).first_column;					\
      }									\
   } while(YYID(0))

#define YYLEX_PARAM state->scanner


/* Line 189 of yacc.c  */
#line 183 "program_parse.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ARBvp_10 = 258,
     ARBfp_10 = 259,
     ADDRESS = 260,
     ALIAS = 261,
     ATTRIB = 262,
     OPTION = 263,
     OUTPUT = 264,
     PARAM = 265,
     TEMP = 266,
     END = 267,
     BIN_OP = 268,
     BINSC_OP = 269,
     SAMPLE_OP = 270,
     SCALAR_OP = 271,
     TRI_OP = 272,
     VECTOR_OP = 273,
     ARL_OP = 274,
     ARA_OP = 275,
     KIL = 276,
     SWZ = 277,
     TXD_OP = 278,
     BRA_OP = 279,
     FLOW_OP = 280,
     INTEGER = 281,
     REAL = 282,
     AMBIENT = 283,
     ATTENUATION = 284,
     BACK = 285,
     CLIP = 286,
     COLOR = 287,
     DEPTH = 288,
     DIFFUSE = 289,
     DIRECTION = 290,
     EMISSION = 291,
     ENV = 292,
     EYE = 293,
     FOG = 294,
     FOGCOORD = 295,
     FRAGMENT = 296,
     FRONT = 297,
     HALF = 298,
     INVERSE = 299,
     INVTRANS = 300,
     LIGHT = 301,
     LIGHTMODEL = 302,
     LIGHTPROD = 303,
     LOCAL = 304,
     MATERIAL = 305,
     MAT_PROGRAM = 306,
     MATRIX = 307,
     MATRIXINDEX = 308,
     MODELVIEW = 309,
     MVP = 310,
     NORMAL = 311,
     OBJECT = 312,
     PALETTE = 313,
     PARAMS = 314,
     PLANE = 315,
     POINT_TOK = 316,
     POINTSIZE = 317,
     POSITION = 318,
     PRIMARY = 319,
     PROGRAM = 320,
     PROJECTION = 321,
     RANGE = 322,
     RESULT = 323,
     ROW = 324,
     SCENECOLOR = 325,
     SECONDARY = 326,
     SHININESS = 327,
     SIZE_TOK = 328,
     SPECULAR = 329,
     SPOT = 330,
     STATE = 331,
     TEXCOORD = 332,
     TEXENV = 333,
     TEXGEN = 334,
     TEXGEN_Q = 335,
     TEXGEN_R = 336,
     TEXGEN_S = 337,
     TEXGEN_T = 338,
     TEXTURE = 339,
     TRANSPOSE = 340,
     TEXTURE_UNIT = 341,
     TEX_1D = 342,
     TEX_2D = 343,
     TEX_3D = 344,
     TEX_CUBE = 345,
     TEX_RECT = 346,
     TEX_SHADOW1D = 347,
     TEX_SHADOW2D = 348,
     TEX_SHADOWRECT = 349,
     TEX_ARRAY1D = 350,
     TEX_ARRAY2D = 351,
     TEX_ARRAYSHADOW1D = 352,
     TEX_ARRAYSHADOW2D = 353,
     VERTEX = 354,
     VTXATTRIB = 355,
     WEIGHT = 356,
     IDENTIFIER = 357,
     USED_IDENTIFIER = 358,
     MASK4 = 359,
     MASK3 = 360,
     MASK2 = 361,
     MASK1 = 362,
     SWIZZLE = 363,
     DOT_DOT = 364,
     DOT = 365
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 116 "program_parse.y"

   struct asm_instruction *inst;
   struct asm_symbol *sym;
   struct asm_symbol temp_sym;
   struct asm_swizzle_mask swiz_mask;
   struct asm_src_register src_reg;
   struct prog_dst_register dst_reg;
   struct prog_instruction temp_inst;
   char *string;
   unsigned result;
   unsigned attrib;
   int integer;
   float real;
   gl_state_index state[STATE_LENGTH];
   int negate;
   struct asm_vector vector;
   gl_inst_opcode opcode;

   struct {
      unsigned swz;
      unsigned rgba_valid:1;
      unsigned xyzw_valid:1;
      unsigned negate:1;
   } ext_swizzle;



/* Line 214 of yacc.c  */
#line 357 "program_parse.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */

/* Line 264 of yacc.c  */
#line 262 "program_parse.y"

extern int yylex(YYSTYPE *yylval_param, YYLTYPE *yylloc_param,
    void *yyscanner);


/* Line 264 of yacc.c  */
#line 388 "program_parse.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   410

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  124
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  150
/* YYNRULES -- Number of rules.  */
#define YYNRULES  294
/* YYNRULES -- Number of states.  */
#define YYNSTATES  497

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   365

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     119,   120,     2,   117,   113,   118,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   112,   111,
       2,   121,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   115,     2,   116,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   122,   114,   123,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     8,    10,    12,    15,    16,    20,    23,
      24,    27,    30,    33,    35,    37,    39,    41,    43,    45,
      47,    49,    51,    53,    55,    57,    59,    61,    63,    65,
      70,    75,    80,    85,    92,    99,   108,   117,   120,   123,
     136,   139,   141,   143,   145,   147,   149,   151,   153,   155,
     157,   159,   161,   163,   170,   174,   177,   180,   185,   188,
     190,   194,   200,   204,   208,   210,   212,   220,   223,   225,
     227,   229,   231,   236,   238,   240,   242,   244,   246,   248,
     250,   254,   255,   258,   261,   263,   265,   267,   269,   271,
     273,   275,   277,   278,   280,   282,   284,   286,   287,   291,
     295,   296,   299,   302,   304,   306,   308,   310,   312,   314,
     316,   318,   320,   322,   324,   329,   332,   335,   337,   340,
     342,   345,   347,   350,   355,   360,   362,   363,   367,   369,
     371,   374,   376,   379,   381,   383,   387,   394,   395,   397,
     400,   405,   407,   411,   413,   415,   417,   419,   421,   423,
     425,   427,   429,   431,   434,   437,   440,   443,   446,   449,
     452,   455,   458,   461,   464,   467,   471,   473,   475,   477,
     483,   485,   487,   489,   492,   494,   496,   499,   501,   504,
     511,   513,   517,   519,   521,   523,   525,   527,   532,   534,
     536,   538,   540,   542,   544,   547,   549,   551,   557,   559,
     562,   564,   566,   572,   575,   576,   583,   587,   588,   590,
     592,   594,   596,   598,   601,   603,   605,   608,   613,   618,
     619,   623,   625,   627,   629,   632,   634,   636,   638,   640,
     646,   648,   652,   658,   664,   666,   670,   676,   678,   680,
     682,   684,   686,   688,   690,   692,   694,   698,   704,   712,
     722,   725,   728,   730,   732,   733,   734,   739,   741,   742,
     743,   747,   751,   753,   759,   762,   765,   768,   771,   775,
     778,   782,   783,   785,   787,   788,   790,   792,   793,   795,
     797,   798,   800,   802,   803,   807,   808,   812,   813,   817,
     819,   821,   823,   828,   830
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     125,     0,    -1,   126,   127,   129,    12,    -1,     3,    -1,
       4,    -1,   127,   128,    -1,    -1,     8,   273,   111,    -1,
     129,   130,    -1,    -1,   131,   111,    -1,   181,   111,    -1,
     179,   112,    -1,   132,    -1,   133,    -1,   134,    -1,   136,
      -1,   135,    -1,   137,    -1,   138,    -1,   139,    -1,   140,
      -1,   141,    -1,   147,    -1,   142,    -1,   143,    -1,   144,
      -1,   148,    -1,   149,    -1,    19,   154,   113,   150,    -1,
      20,   154,   113,   155,    -1,    18,   153,   113,   152,    -1,
      16,   153,   113,   150,    -1,    14,   153,   113,   150,   113,
     150,    -1,    13,   153,   113,   152,   113,   152,    -1,    17,
     153,   113,   152,   113,   152,   113,   152,    -1,    15,   153,
     113,   152,   113,   145,   113,   146,    -1,    21,   152,    -1,
      21,   175,    -1,    23,   153,   113,   152,   113,   152,   113,
     152,   113,   145,   113,   146,    -1,    86,   267,    -1,    87,
      -1,    88,    -1,    89,    -1,    90,    -1,    91,    -1,    92,
      -1,    93,    -1,    94,    -1,    95,    -1,    96,    -1,    97,
      -1,    98,    -1,    22,   153,   113,   160,   113,   157,    -1,
      24,   180,   174,    -1,    25,   174,    -1,   252,   151,    -1,
     252,   114,   151,   114,    -1,   160,   171,    -1,   249,    -1,
     252,   160,   172,    -1,   252,   114,   160,   172,   114,    -1,
     161,   173,   174,    -1,   169,   173,   174,    -1,   156,    -1,
     169,    -1,   158,   113,   158,   113,   158,   113,   158,    -1,
     252,   159,    -1,    26,    -1,   273,    -1,   103,    -1,   183,
      -1,   162,   115,   163,   116,    -1,   197,    -1,   260,    -1,
     103,    -1,   103,    -1,   164,    -1,   165,    -1,    26,    -1,
     169,   170,   166,    -1,    -1,   117,   167,    -1,   118,   168,
      -1,    26,    -1,    26,    -1,   103,    -1,   107,    -1,   107,
      -1,   107,    -1,   104,    -1,   108,    -1,    -1,   104,    -1,
     105,    -1,   106,    -1,   107,    -1,    -1,   119,   175,   120,
      -1,   119,   176,   120,    -1,    -1,   177,   172,    -1,   178,
     172,    -1,   102,    -1,   103,    -1,   180,    -1,   102,    -1,
     103,    -1,   182,    -1,   189,    -1,   253,    -1,   256,    -1,
     259,    -1,   272,    -1,     7,   102,   121,   183,    -1,    99,
     184,    -1,    41,   188,    -1,    63,    -1,   101,   186,    -1,
      56,    -1,    32,   265,    -1,    40,    -1,    77,   266,    -1,
      53,   115,   187,   116,    -1,   100,   115,   185,   116,    -1,
      26,    -1,    -1,   115,   187,   116,    -1,    26,    -1,    63,
      -1,    32,   265,    -1,    40,    -1,    77,   266,    -1,   190,
      -1,   191,    -1,    10,   102,   193,    -1,    10,   102,   115,
     192,   116,   194,    -1,    -1,    26,    -1,   121,   196,    -1,
     121,   122,   195,   123,    -1,   198,    -1,   195,   113,   198,
      -1,   200,    -1,   236,    -1,   246,    -1,   200,    -1,   236,
      -1,   247,    -1,   199,    -1,   237,    -1,   246,    -1,   200,
      -1,    76,   224,    -1,    76,   201,    -1,    76,   203,    -1,
      76,   206,    -1,    76,   208,    -1,    76,   214,    -1,    76,
     210,    -1,    76,   217,    -1,    76,   219,    -1,    76,   221,
      -1,    76,   223,    -1,    76,   235,    -1,    50,   264,   202,
      -1,   212,    -1,    36,    -1,    72,    -1,    46,   115,   213,
     116,   204,    -1,   212,    -1,    63,    -1,    29,    -1,    75,
     205,    -1,    43,    -1,    35,    -1,    47,   207,    -1,    28,
      -1,   264,    70,    -1,    48,   115,   213,   116,   264,   209,
      -1,   212,    -1,    78,   268,   211,    -1,    32,    -1,    28,
      -1,    34,    -1,    74,    -1,    26,    -1,    79,   266,   215,
     216,    -1,    38,    -1,    57,    -1,    82,    -1,    83,    -1,
      81,    -1,    80,    -1,    39,   218,    -1,    32,    -1,    59,
      -1,    31,   115,   220,   116,    60,    -1,    26,    -1,    61,
     222,    -1,    73,    -1,    29,    -1,   226,    69,   115,   229,
     116,    -1,   226,   225,    -1,    -1,    69,   115,   229,   109,
     229,   116,    -1,    52,   230,   227,    -1,    -1,   228,    -1,
      44,    -1,    85,    -1,    45,    -1,    26,    -1,    54,   231,
      -1,    66,    -1,    55,    -1,    84,   266,    -1,    58,   115,
     233,   116,    -1,    51,   115,   234,   116,    -1,    -1,   115,
     232,   116,    -1,    26,    -1,    26,    -1,    26,    -1,    33,
      67,    -1,   240,    -1,   243,    -1,   238,    -1,   241,    -1,
      65,    37,   115,   239,   116,    -1,   244,    -1,   244,   109,
     244,    -1,    65,    37,   115,   244,   116,    -1,    65,    49,
     115,   242,   116,    -1,   245,    -1,   245,   109,   245,    -1,
      65,    49,   115,   245,   116,    -1,    26,    -1,    26,    -1,
     248,    -1,   250,    -1,   249,    -1,   250,    -1,   251,    -1,
      27,    -1,    26,    -1,   122,   251,   123,    -1,   122,   251,
     113,   251,   123,    -1,   122,   251,   113,   251,   113,   251,
     123,    -1,   122,   251,   113,   251,   113,   251,   113,   251,
     123,    -1,   252,    27,    -1,   252,    26,    -1,   117,    -1,
     118,    -1,    -1,    -1,   255,    11,   254,   258,    -1,   273,
      -1,    -1,    -1,     5,   257,   258,    -1,   258,   113,   102,
      -1,   102,    -1,   255,     9,   102,   121,   260,    -1,    68,
      63,    -1,    68,    40,    -1,    68,   261,    -1,    68,    62,
      -1,    68,    77,   266,    -1,    68,    33,    -1,    32,   262,
     263,    -1,    -1,    42,    -1,    30,    -1,    -1,    64,    -1,
      71,    -1,    -1,    42,    -1,    30,    -1,    -1,    64,    -1,
      71,    -1,    -1,   115,   269,   116,    -1,    -1,   115,   270,
     116,    -1,    -1,   115,   271,   116,    -1,    26,    -1,    26,
      -1,    26,    -1,     6,   102,   121,   103,    -1,   102,    -1,
     103,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   269,   269,   272,   280,   292,   293,   296,   318,   319,
     322,   337,   338,   341,   346,   351,   354,   355,   356,   357,
     358,   359,   360,   361,   364,   365,   366,   369,   370,   373,
     379,   385,   391,   397,   404,   410,   417,   461,   466,   476,
     520,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   540,   552,   569,   579,   587,   604,   611,
     630,   641,   661,   686,   708,   711,   719,   752,   759,   774,
     824,   866,   877,   898,   908,   914,   945,   962,   962,   964,
     971,   983,   984,   985,   988,  1002,  1016,  1034,  1055,  1057,
    1058,  1059,  1060,  1063,  1063,  1063,  1063,  1064,  1067,  1078,
    1090,  1097,  1104,  1111,  1134,  1157,  1176,  1176,  1179,  1180,
    1181,  1182,  1183,  1184,  1187,  1205,  1209,  1215,  1219,  1223,
    1227,  1236,  1245,  1249,  1254,  1260,  1271,  1271,  1272,  1274,
    1278,  1282,  1286,  1292,  1292,  1294,  1310,  1333,  1336,  1347,
    1353,  1359,  1360,  1367,  1373,  1379,  1387,  1393,  1399,  1407,
    1413,  1419,  1427,  1428,  1431,  1432,  1433,  1434,  1435,  1436,
    1437,  1438,  1439,  1440,  1441,  1444,  1453,  1457,  1461,  1467,
    1476,  1480,  1484,  1493,  1497,  1503,  1509,  1516,  1521,  1529,
    1539,  1541,  1549,  1555,  1559,  1563,  1569,  1580,  1589,  1593,
    1598,  1602,  1606,  1610,  1616,  1623,  1627,  1633,  1641,  1652,
    1659,  1663,  1669,  1679,  1690,  1694,  1712,  1721,  1724,  1730,
    1734,  1738,  1744,  1755,  1760,  1765,  1770,  1775,  1780,  1788,
    1791,  1796,  1809,  1817,  1828,  1836,  1836,  1838,  1838,  1840,
    1850,  1855,  1862,  1872,  1881,  1886,  1893,  1903,  1913,  1925,
    1925,  1926,  1926,  1928,  1938,  1946,  1956,  1964,  1972,  1981,
    1992,  1996,  2002,  2003,  2004,  2007,  2007,  2010,  2045,  2049,
    2049,  2052,  2058,  2066,  2079,  2088,  2097,  2101,  2110,  2119,
    2130,  2137,  2142,  2151,  2163,  2166,  2175,  2186,  2187,  2188,
    2191,  2192,  2193,  2196,  2197,  2200,  2201,  2204,  2205,  2208,
    2219,  2230,  2241,  2262,  2263
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ARBvp_10", "ARBfp_10", "ADDRESS",
  "ALIAS", "ATTRIB", "OPTION", "OUTPUT", "PARAM", "TEMP", "END", "BIN_OP",
  "BINSC_OP", "SAMPLE_OP", "SCALAR_OP", "TRI_OP", "VECTOR_OP", "ARL_OP",
  "ARA_OP", "KIL", "SWZ", "TXD_OP", "BRA_OP", "FLOW_OP", "INTEGER", "REAL",
  "AMBIENT", "ATTENUATION", "BACK", "CLIP", "COLOR", "DEPTH", "DIFFUSE",
  "DIRECTION", "EMISSION", "ENV", "EYE", "FOG", "FOGCOORD", "FRAGMENT",
  "FRONT", "HALF", "INVERSE", "INVTRANS", "LIGHT", "LIGHTMODEL",
  "LIGHTPROD", "LOCAL", "MATERIAL", "MAT_PROGRAM", "MATRIX", "MATRIXINDEX",
  "MODELVIEW", "MVP", "NORMAL", "OBJECT", "PALETTE", "PARAMS", "PLANE",
  "POINT_TOK", "POINTSIZE", "POSITION", "PRIMARY", "PROGRAM", "PROJECTION",
  "RANGE", "RESULT", "ROW", "SCENECOLOR", "SECONDARY", "SHININESS",
  "SIZE_TOK", "SPECULAR", "SPOT", "STATE", "TEXCOORD", "TEXENV", "TEXGEN",
  "TEXGEN_Q", "TEXGEN_R", "TEXGEN_S", "TEXGEN_T", "TEXTURE", "TRANSPOSE",
  "TEXTURE_UNIT", "TEX_1D", "TEX_2D", "TEX_3D", "TEX_CUBE", "TEX_RECT",
  "TEX_SHADOW1D", "TEX_SHADOW2D", "TEX_SHADOWRECT", "TEX_ARRAY1D",
  "TEX_ARRAY2D", "TEX_ARRAYSHADOW1D", "TEX_ARRAYSHADOW2D", "VERTEX",
  "VTXATTRIB", "WEIGHT", "IDENTIFIER", "USED_IDENTIFIER", "MASK4", "MASK3",
  "MASK2", "MASK1", "SWIZZLE", "DOT_DOT", "DOT", "';'", "':'", "','",
  "'|'", "'['", "']'", "'+'", "'-'", "'('", "')'", "'='", "'{'", "'}'",
  "$accept", "program", "language", "optionSequence", "option",
  "statementSequence", "statement", "instruction", "ALU_instruction",
  "TexInstruction", "FlowInstruction", "ARL_instruction",
  "ARA_instruction", "VECTORop_instruction", "SCALARop_instruction",
  "BINSCop_instruction", "BINop_instruction", "TRIop_instruction",
  "SAMPLE_instruction", "KIL_instruction", "TXD_instruction",
  "texImageUnit", "texTarget", "SWZ_instruction", "BRA_instruction",
  "FLOWCC_instruction", "scalarSrcReg", "scalarUse", "swizzleSrcReg",
  "maskedDstReg", "instResultAddr", "instOperandAddrVNS", "addrUseVNS",
  "extendedSwizzle", "extSwizComp", "extSwizSel", "srcReg", "dstReg",
  "progParamArray", "progParamArrayMem", "progParamArrayAbs",
  "progParamArrayRel", "addrRegRelOffset", "addrRegPosOffset",
  "addrRegNegOffset", "addrReg", "addrComponent", "scalarSuffix",
  "swizzleSuffix", "optionalMask", "optionalCcMask", "ccTest", "ccTest2",
  "ccMaskRule", "ccMaskRule2", "labelStatement", "labelName",
  "namingStatement", "ATTRIB_statement", "attribBinding", "vtxAttribItem",
  "vtxAttribNum", "vtxOptWeightNum", "vtxWeightNum", "fragAttribItem",
  "PARAM_statement", "PARAM_singleStmt", "PARAM_multipleStmt",
  "optArraySize", "paramSingleInit", "paramMultipleInit",
  "paramMultInitList", "paramSingleItemDecl", "paramSingleItemUse",
  "paramMultipleItem", "stateMultipleItem", "stateSingleItem",
  "stateMaterialItem", "stateMatProperty", "stateLightItem",
  "stateLightProperty", "stateSpotProperty", "stateLightModelItem",
  "stateLModProperty", "stateLightProdItem", "stateLProdProperty",
  "stateTexEnvItem", "stateTexEnvProperty", "ambDiffSpecProperty",
  "stateLightNumber", "stateTexGenItem", "stateTexGenType",
  "stateTexGenCoord", "stateFogItem", "stateFogProperty",
  "stateClipPlaneItem", "stateClipPlaneNum", "statePointItem",
  "statePointProperty", "stateMatrixRow", "stateMatrixRows",
  "optMatrixRows", "stateMatrixItem", "stateOptMatModifier",
  "stateMatModifier", "stateMatrixRowNum", "stateMatrixName",
  "stateOptModMatNum", "stateModMatNum", "statePaletteMatNum",
  "stateProgramMatNum", "stateDepthItem", "programSingleItem",
  "programMultipleItem", "progEnvParams", "progEnvParamNums",
  "progEnvParam", "progLocalParams", "progLocalParamNums",
  "progLocalParam", "progEnvParamNum", "progLocalParamNum",
  "paramConstDecl", "paramConstUse", "paramConstScalarDecl",
  "paramConstScalarUse", "paramConstVector", "signedFloatConstant",
  "optionalSign", "TEMP_statement", "@1", "optVarSize",
  "ADDRESS_statement", "@2", "varNameList", "OUTPUT_statement",
  "resultBinding", "resultColBinding", "optResultFaceType",
  "optResultColorType", "optFaceType", "optColorType",
  "optTexCoordUnitNum", "optTexImageUnitNum", "optLegacyTexUnitNum",
  "texCoordUnitNum", "texImageUnitNum", "legacyTexUnitNum",
  "ALIAS_statement", "string", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,    59,    58,    44,   124,    91,    93,    43,    45,    40,
      41,    61,   123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   124,   125,   126,   126,   127,   127,   128,   129,   129,
     130,   130,   130,   131,   131,   131,   132,   132,   132,   132,
     132,   132,   132,   132,   133,   133,   133,   134,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   143,   144,
     145,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   147,   148,   149,   150,   150,   151,   151,
     152,   152,   153,   154,   155,   156,   157,   158,   159,   159,
     160,   160,   160,   160,   161,   161,   162,   163,   163,   164,
     165,   166,   166,   166,   167,   168,   169,   170,   171,   172,
     172,   172,   172,   173,   173,   173,   173,   173,   174,   174,
     174,   175,   176,   177,   178,   179,   180,   180,   181,   181,
     181,   181,   181,   181,   182,   183,   183,   184,   184,   184,
     184,   184,   184,   184,   184,   185,   186,   186,   187,   188,
     188,   188,   188,   189,   189,   190,   191,   192,   192,   193,
     194,   195,   195,   196,   196,   196,   197,   197,   197,   198,
     198,   198,   199,   199,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   201,   202,   202,   202,   203,
     204,   204,   204,   204,   204,   205,   206,   207,   207,   208,
     209,   210,   211,   212,   212,   212,   213,   214,   215,   215,
     216,   216,   216,   216,   217,   218,   218,   219,   220,   221,
     222,   222,   223,   224,   225,   225,   226,   227,   227,   228,
     228,   228,   229,   230,   230,   230,   230,   230,   230,   231,
     231,   232,   233,   234,   235,   236,   236,   237,   237,   238,
     239,   239,   240,   241,   242,   242,   243,   244,   245,   246,
     246,   247,   247,   248,   249,   249,   250,   250,   250,   250,
     251,   251,   252,   252,   252,   254,   253,   255,   255,   257,
     256,   258,   258,   259,   260,   260,   260,   260,   260,   260,
     261,   262,   262,   262,   263,   263,   263,   264,   264,   264,
     265,   265,   265,   266,   266,   267,   267,   268,   268,   269,
     270,   271,   272,   273,   273
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     1,     1,     2,     0,     3,     2,     0,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       4,     4,     4,     6,     6,     8,     8,     2,     2,    12,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     6,     3,     2,     2,     4,     2,     1,
       3,     5,     3,     3,     1,     1,     7,     2,     1,     1,
       1,     1,     4,     1,     1,     1,     1,     1,     1,     1,
       3,     0,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     1,     1,     1,     0,     3,     3,
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     2,     2,     1,     2,     1,
       2,     1,     2,     4,     4,     1,     0,     3,     1,     1,
       2,     1,     2,     1,     1,     3,     6,     0,     1,     2,
       4,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     3,     1,     1,     1,     5,
       1,     1,     1,     2,     1,     1,     2,     1,     2,     6,
       1,     3,     1,     1,     1,     1,     1,     4,     1,     1,
       1,     1,     1,     1,     2,     1,     1,     5,     1,     2,
       1,     1,     5,     2,     0,     6,     3,     0,     1,     1,
       1,     1,     1,     2,     1,     1,     2,     4,     4,     0,
       3,     1,     1,     1,     2,     1,     1,     1,     1,     5,
       1,     3,     5,     5,     1,     3,     5,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     5,     7,     9,
       2,     2,     1,     1,     0,     0,     4,     1,     0,     0,
       3,     3,     1,     5,     2,     2,     2,     2,     3,     2,
       3,     0,     1,     1,     0,     1,     1,     0,     1,     1,
       0,     1,     1,     0,     3,     0,     3,     0,     3,     1,
       1,     1,     4,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     3,     4,     0,     6,     1,     9,     0,     5,   258,
     293,   294,     0,   259,     0,     0,     0,     2,     0,     0,
       0,     0,     0,     0,     0,     0,   254,     0,     0,     0,
     100,   293,   294,     8,     0,    13,    14,    15,    17,    16,
      18,    19,    20,    21,    22,    24,    25,    26,    23,    27,
      28,     0,   105,     0,   108,   109,   133,   134,   110,     0,
     111,   112,   113,   257,     7,     0,     0,     0,     0,     0,
      75,     0,    97,    74,     0,     0,     0,     0,     0,    86,
       0,    97,     0,   103,   252,   253,    37,    38,    92,     0,
       0,     0,   106,   107,   100,     0,    55,    10,    12,    11,
       0,   255,   262,   260,     0,     0,   137,   254,   135,   271,
     269,   265,   267,   264,   283,   266,   254,    93,    94,    95,
      96,   100,   254,   254,   254,   254,   254,   254,   100,     0,
      90,    89,    91,   101,   245,   244,     0,     0,     0,     0,
      70,     0,   254,    92,     0,    71,    73,   146,   147,   225,
     226,   148,   241,   242,     0,   254,    54,   104,     0,     0,
      92,     0,     0,     0,   292,   114,   138,     0,   139,   143,
     144,   145,   239,   240,   243,     0,   273,   272,   274,     0,
     268,     0,    62,     0,     0,     0,    32,     0,    31,    29,
      63,    30,    64,    65,   280,   131,   129,   283,   116,     0,
       0,     0,     0,     0,     0,   277,     0,   277,     0,     0,
     287,   283,   154,   155,   156,   157,   159,   158,   160,   161,
     162,   163,     0,   164,   280,   121,     0,   119,   117,   283,
       0,   126,   115,    92,     0,    60,     0,     0,     0,    98,
      99,   102,     0,   256,   261,     0,   251,   250,   275,   276,
     270,   289,     0,   254,   254,     0,    56,     0,    59,     0,
     254,   281,   282,   130,   132,     0,     0,     0,   224,   195,
     196,   194,     0,   177,   279,   278,   176,     0,     0,     0,
       0,   219,   215,     0,   214,   283,   207,   201,   200,   199,
       0,     0,     0,     0,   120,     0,   122,     0,     0,   118,
       0,   254,   246,    79,     0,    77,    78,     0,   254,   254,
     263,     0,   136,   284,    34,    33,     0,    88,    58,   285,
       0,     0,   237,     0,   238,     0,   198,     0,   186,     0,
     178,     0,   183,   184,   167,   168,   185,   165,   166,     0,
       0,   213,     0,   216,   209,   211,   210,   206,   208,   291,
       0,   182,   181,   188,   189,     0,     0,   128,     0,   125,
       0,     0,    61,     0,    72,    87,    81,    53,     0,     0,
       0,   254,    57,     0,    40,     0,   254,   232,   236,     0,
       0,   277,   223,     0,   221,     0,   222,     0,   288,   193,
     192,   190,   191,   187,   212,     0,   123,   124,   127,   254,
     247,     0,     0,    80,   254,    68,    67,    69,   254,     0,
       0,     0,   141,   149,   152,   150,   227,   228,   151,   290,
       0,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    36,    35,   197,   172,   174,   171,     0,
     169,   170,     0,   218,   220,   217,   202,     0,    84,    82,
      85,    83,     0,     0,     0,     0,   153,   204,   254,   140,
     286,   175,   173,   179,   180,   254,   248,   254,     0,     0,
       0,     0,   203,   142,     0,     0,     0,     0,   230,     0,
     234,     0,   249,   254,     0,   229,     0,   233,     0,     0,
      66,    39,   231,   235,     0,     0,   205
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     6,     8,     9,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,   320,   433,    48,    49,    50,   183,   256,    86,    71,
      80,   191,   192,   367,   368,   406,   257,    72,   144,   304,
     305,   306,   403,   449,   451,    81,   366,   318,   133,   121,
      96,    87,   159,    88,   160,    51,    52,    53,    54,   145,
     232,   360,   299,   358,   198,    55,    56,    57,   167,   108,
     312,   411,   168,   146,   412,   413,   147,   212,   337,   213,
     440,   462,   214,   276,   215,   463,   216,   352,   338,   329,
     217,   355,   393,   218,   271,   219,   327,   220,   289,   221,
     456,   472,   222,   347,   348,   395,   286,   341,   385,   387,
     383,   223,   148,   415,   416,   477,   149,   417,   479,   150,
     323,   325,   418,   151,   172,   152,   153,   174,    89,    58,
     162,    59,    60,    65,   103,    61,    73,   115,   178,   250,
     277,   263,   180,   374,   291,   252,   420,   350,    62,    12
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -404
static const yytype_int16 yypact[] =
{
      29,  -404,  -404,    54,  -404,  -404,    67,   148,  -404,    24,
    -404,  -404,   -32,  -404,    26,    40,    46,  -404,   -41,   -41,
     -41,   -41,   -41,   -41,    53,    53,    43,   -41,   -41,   156,
      47,    50,    65,  -404,    75,  -404,  -404,  -404,  -404,  -404,
    -404,  -404,  -404,  -404,  -404,  -404,  -404,  -404,  -404,  -404,
    -404,    79,  -404,   103,  -404,  -404,  -404,  -404,  -404,   171,
    -404,  -404,  -404,  -404,  -404,   114,   116,   123,    22,   135,
    -404,   106,   150,  -404,   147,   152,   164,   167,   168,  -404,
     169,   150,   170,  -404,  -404,  -404,  -404,  -404,   145,   -15,
     172,   173,  -404,  -404,    47,   160,  -404,  -404,  -404,  -404,
     182,  -404,  -404,   174,   185,   -19,   222,    -2,  -404,    48,
    -404,  -404,  -404,  -404,   175,  -404,   155,  -404,  -404,  -404,
    -404,    47,   155,   155,   155,   155,   155,   155,    47,    53,
    -404,  -404,  -404,  -404,  -404,  -404,    28,   144,   126,    69,
     176,    32,   155,   145,   179,  -404,  -404,  -404,  -404,  -404,
    -404,  -404,  -404,  -404,    32,   155,  -404,  -404,   180,   181,
     145,   177,   114,   187,  -404,  -404,  -404,   183,  -404,  -404,
    -404,  -404,  -404,  -404,  -404,   249,  -404,  -404,   146,   267,
    -404,   189,  -404,   190,    30,   191,  -404,   192,  -404,  -404,
    -404,  -404,  -404,  -404,   151,  -404,  -404,   175,  -404,   193,
     194,   195,   228,    23,   196,   178,   197,   141,   134,    -1,
     198,   175,  -404,  -404,  -404,  -404,  -404,  -404,  -404,  -404,
    -404,  -404,   227,  -404,   151,  -404,   199,  -404,  -404,   175,
     200,   201,  -404,   145,    86,  -404,   -10,   204,   205,  -404,
    -404,  -404,   239,   174,  -404,   202,  -404,  -404,  -404,  -404,
    -404,  -404,   203,   155,   155,    32,  -404,   213,   214,   236,
     155,  -404,  -404,  -404,  -404,   298,   299,   300,  -404,  -404,
    -404,  -404,   302,  -404,  -404,  -404,  -404,   259,   302,    66,
     215,   216,  -404,   217,  -404,   175,    68,  -404,  -404,  -404,
     307,   303,    73,   221,  -404,   308,  -404,   311,   308,  -404,
     224,   155,  -404,  -404,   223,  -404,  -404,   233,   155,   155,
    -404,   220,  -404,  -404,  -404,  -404,   229,  -404,  -404,   226,
     231,   232,  -404,   230,  -404,   234,  -404,   240,  -404,   241,
    -404,   243,  -404,  -404,  -404,  -404,  -404,  -404,  -404,   321,
     322,  -404,   323,  -404,  -404,  -404,  -404,  -404,  -404,  -404,
     246,  -404,  -404,  -404,  -404,   188,   326,  -404,   247,  -404,
     248,   250,  -404,    90,  -404,  -404,   161,  -404,   242,   -16,
     252,     1,  -404,   341,  -404,   136,   155,  -404,  -404,   309,
     121,   141,  -404,   254,  -404,   255,  -404,   256,  -404,  -404,
    -404,  -404,  -404,  -404,  -404,   257,  -404,  -404,  -404,   155,
    -404,   342,   348,  -404,   155,  -404,  -404,  -404,   155,   153,
     126,    98,  -404,  -404,  -404,  -404,  -404,  -404,  -404,  -404,
     261,  -404,  -404,  -404,  -404,  -404,  -404,  -404,  -404,  -404,
    -404,  -404,  -404,  -404,  -404,  -404,  -404,  -404,  -404,   340,
    -404,  -404,    36,  -404,  -404,  -404,  -404,   122,  -404,  -404,
    -404,  -404,   265,   266,   268,   269,  -404,   312,     1,  -404,
    -404,  -404,  -404,  -404,  -404,   155,  -404,   155,   236,   298,
     299,   270,  -404,  -404,   263,   274,   275,   264,   273,   276,
     280,   326,  -404,   155,   136,  -404,   298,  -404,   299,   127,
    -404,  -404,  -404,  -404,   326,   277,  -404
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -404,  -404,  -404,  -404,  -404,  -404,  -404,  -404,  -404,  -404,
    -404,  -404,  -404,  -404,  -404,  -404,  -404,  -404,  -404,  -404,
    -404,   -78,   -93,  -404,  -404,  -404,  -118,   139,  -102,   219,
     370,  -404,  -404,  -404,  -391,  -404,   -37,  -404,  -404,  -404,
    -404,  -404,  -404,  -404,  -404,  -122,  -404,  -404,  -135,   315,
     -25,   304,  -404,  -404,  -404,  -404,   368,  -404,  -404,   293,
    -404,  -404,  -404,   102,  -404,  -404,  -404,  -404,  -404,  -404,
    -404,  -404,  -404,  -404,   -57,  -404,  -105,  -404,  -404,  -404,
    -404,  -404,  -404,  -404,  -404,  -404,  -404,  -404,  -361,   124,
    -404,  -404,  -404,  -404,  -404,  -404,  -404,  -404,  -404,  -404,
    -404,  -404,    -7,  -404,  -404,  -360,  -404,  -404,  -404,  -404,
    -404,  -404,   297,  -404,  -404,  -404,  -404,  -404,  -404,  -404,
    -345,  -403,   301,  -404,  -404,  -166,  -104,  -138,  -107,  -404,
    -404,  -404,  -404,  -404,   244,  -404,   163,  -404,  -404,  -404,
    -202,   186,  -146,  -404,  -404,  -404,  -404,  -404,  -404,    -8
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -242
static const yytype_int16 yytable[] =
{
     175,    63,   169,   173,   234,   279,   186,   193,   235,   189,
     405,   134,   135,   452,   181,   184,   303,   184,   258,   441,
     184,   185,   136,   187,   188,   241,   136,    69,   287,    13,
      14,    15,     1,     2,    16,   175,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
     137,   264,   143,   238,     5,   269,   134,   135,   134,   135,
     194,   138,    70,   137,   332,   292,   409,   480,   195,   156,
     333,   136,   288,   136,   138,     7,   475,   410,   176,    64,
     139,   464,   270,   296,   139,   493,    10,    11,   140,   258,
     177,   196,   490,    79,   332,   137,   182,   137,   300,   141,
     333,   224,   334,   190,   233,   197,   138,   142,   138,   225,
     336,   353,   344,   345,   307,    84,    85,   237,    84,    85,
     142,   489,   226,   142,   478,   227,    31,    32,    66,   139,
     354,   139,   228,   140,   495,   140,   315,   106,   335,   343,
     336,   492,    67,   107,   255,    83,   229,   184,    68,   332,
     436,   314,   142,   346,   142,   333,    79,   201,   321,   202,
      84,    85,  -106,   363,   437,   203,    95,   109,   110,   230,
     231,   274,   204,   205,   206,   111,   207,  -107,   208,   442,
     100,   199,   101,   275,   438,   280,    97,   209,   281,   282,
     454,    98,   283,   200,   175,   336,   439,   112,   113,   301,
     284,   369,   455,   399,   210,   211,   273,   370,   274,   302,
     248,   458,   114,   400,    99,   261,   102,   249,   285,   116,
     275,   459,   262,   421,   422,   423,   424,   425,   426,   427,
     428,   429,   430,   431,   432,   465,   494,   104,    74,    75,
      76,    77,    78,   446,   105,   466,    90,    91,   166,   130,
      10,    11,   131,   132,   117,   118,   119,   120,    92,    93,
     122,   447,    83,   157,   175,   123,   414,   173,   389,   390,
     391,   392,    84,    85,   434,   246,   247,   124,   401,   402,
     125,   126,   127,   129,   161,   154,   155,   163,   164,   244,
     179,   -76,   175,   251,   236,   268,   293,   369,   242,   245,
     239,   240,   253,   254,   259,   260,   453,    69,   265,   266,
     267,   272,   278,   290,   295,   297,   298,   308,   309,   313,
     317,  -241,   319,   311,   322,   324,   326,   474,   328,   330,
     339,   340,   342,   349,   357,   351,   356,   359,   362,   364,
     365,   373,   371,   372,   375,   376,   377,   382,   384,   386,
     378,   175,   394,   414,   173,   404,   379,   380,   175,   381,
     369,   407,   388,   396,   397,   408,   398,   419,   448,   435,
     443,   444,   445,   446,   450,   461,   369,   460,   467,   468,
     485,   471,   486,   469,   470,   481,   482,   483,   484,   488,
     476,   491,   487,   496,   316,    82,   128,    94,   165,   158,
     361,   473,   331,   457,   170,   310,   243,     0,   171,     0,
     294
};

static const yytype_int16 yycheck[] =
{
     107,     9,   107,   107,   142,   207,   124,   129,   143,   127,
      26,    26,    27,   404,   116,   122,    26,   124,   184,   380,
     127,   123,    41,   125,   126,   160,    41,    68,    29,     5,
       6,     7,     3,     4,    10,   142,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      65,   197,    89,   155,     0,    32,    26,    27,    26,    27,
      32,    76,   103,    65,    28,   211,    65,   470,    40,    94,
      34,    41,    73,    41,    76,     8,   467,    76,    30,   111,
      99,   442,    59,   229,    99,   488,   102,   103,   103,   255,
      42,    63,   483,   103,    28,    65,   121,    65,   233,   114,
      34,    32,    36,   128,   141,    77,    76,   122,    76,    40,
      74,    38,    44,    45,   236,   117,   118,   154,   117,   118,
     122,   481,    53,   122,   469,    56,   102,   103,   102,    99,
      57,    99,    63,   103,   494,   103,   254,   115,    72,   285,
      74,   486,   102,   121,   114,   102,    77,   254,   102,    28,
      29,   253,   122,    85,   122,    34,   103,    31,   260,    33,
     117,   118,   112,   301,    43,    39,   119,    32,    33,   100,
     101,    30,    46,    47,    48,    40,    50,   112,    52,   381,
       9,    37,    11,    42,    63,    51,   111,    61,    54,    55,
      37,   112,    58,    49,   301,    74,    75,    62,    63,   113,
      66,   308,    49,   113,    78,    79,    28,   309,    30,   123,
      64,   113,    77,   123,   111,    64,   102,    71,    84,   113,
      42,   123,    71,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,   113,   109,   121,    19,    20,
      21,    22,    23,   116,   121,   123,    27,    28,    26,   104,
     102,   103,   107,   108,   104,   105,   106,   107,   102,   103,
     113,   399,   102,   103,   371,   113,   371,   371,    80,    81,
      82,    83,   117,   118,   376,    26,    27,   113,   117,   118,
     113,   113,   113,   113,   102,   113,   113,   113,   103,   102,
     115,   115,   399,    26,   115,    67,    69,   404,   121,   116,
     120,   120,   113,   113,   113,   113,   408,    68,   115,   115,
     115,   115,   115,   115,   115,   115,   115,   113,   113,   116,
     107,   107,    86,   121,    26,    26,    26,   465,    26,    70,
     115,   115,   115,    26,    26,    32,   115,    26,   114,   116,
     107,   115,   122,   114,   113,   113,   116,    26,    26,    26,
     116,   458,    26,   458,   458,   113,   116,   116,   465,   116,
     467,   369,   116,   116,   116,   113,   116,    26,    26,    60,
     116,   116,   116,   116,    26,    35,   483,   116,   113,   113,
     116,    69,   109,   115,   115,   115,   123,   113,   113,   109,
     468,   484,   116,   116,   255,    25,    81,    29,   105,    95,
     298,   458,   278,   410,   107,   242,   162,    -1,   107,    -1,
     224
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     3,     4,   125,   126,     0,   127,     8,   128,   129,
     102,   103,   273,     5,     6,     7,    10,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,   102,   103,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   147,   148,
     149,   179,   180,   181,   182,   189,   190,   191,   253,   255,
     256,   259,   272,   273,   111,   257,   102,   102,   102,    68,
     103,   153,   161,   260,   153,   153,   153,   153,   153,   103,
     154,   169,   154,   102,   117,   118,   152,   175,   177,   252,
     153,   153,   102,   103,   180,   119,   174,   111,   112,   111,
       9,    11,   102,   258,   121,   121,   115,   121,   193,    32,
      33,    40,    62,    63,    77,   261,   113,   104,   105,   106,
     107,   173,   113,   113,   113,   113,   113,   113,   173,   113,
     104,   107,   108,   172,    26,    27,    41,    65,    76,    99,
     103,   114,   122,   160,   162,   183,   197,   200,   236,   240,
     243,   247,   249,   250,   113,   113,   174,   103,   175,   176,
     178,   102,   254,   113,   103,   183,    26,   192,   196,   200,
     236,   246,   248,   250,   251,   252,    30,    42,   262,   115,
     266,   152,   174,   150,   252,   152,   150,   152,   152,   150,
     174,   155,   156,   169,    32,    40,    63,    77,   188,    37,
      49,    31,    33,    39,    46,    47,    48,    50,    52,    61,
      78,    79,   201,   203,   206,   208,   210,   214,   217,   219,
     221,   223,   226,   235,    32,    40,    53,    56,    63,    77,
     100,   101,   184,   160,   251,   172,   115,   160,   152,   120,
     120,   172,   121,   258,   102,   116,    26,    27,    64,    71,
     263,    26,   269,   113,   113,   114,   151,   160,   249,   113,
     113,    64,    71,   265,   266,   115,   115,   115,    67,    32,
      59,   218,   115,    28,    30,    42,   207,   264,   115,   264,
      51,    54,    55,    58,    66,    84,   230,    29,    73,   222,
     115,   268,   266,    69,   265,   115,   266,   115,   115,   186,
     172,   113,   123,    26,   163,   164,   165,   169,   113,   113,
     260,   121,   194,   116,   152,   150,   151,   107,   171,    86,
     145,   152,    26,   244,    26,   245,    26,   220,    26,   213,
      70,   213,    28,    34,    36,    72,    74,   202,   212,   115,
     115,   231,   115,   266,    44,    45,    85,   227,   228,    26,
     271,    32,   211,    38,    57,   215,   115,    26,   187,    26,
     185,   187,   114,   251,   116,   107,   170,   157,   158,   252,
     152,   122,   114,   115,   267,   113,   113,   116,   116,   116,
     116,   116,    26,   234,    26,   232,    26,   233,   116,    80,
      81,    82,    83,   216,    26,   229,   116,   116,   116,   113,
     123,   117,   118,   166,   113,    26,   159,   273,   113,    65,
      76,   195,   198,   199,   200,   237,   238,   241,   246,    26,
     270,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,   146,   152,    60,    29,    43,    63,    75,
     204,   212,   264,   116,   116,   116,   116,   251,    26,   167,
      26,   168,   158,   152,    37,    49,   224,   226,   113,   123,
     116,    35,   205,   209,   212,   113,   123,   113,   113,   115,
     115,    69,   225,   198,   251,   158,   145,   239,   244,   242,
     245,   115,   123,   113,   113,   116,   109,   116,   109,   229,
     158,   146,   244,   245,   109,   229,   116
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (&yylloc, state, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, &yylloc, scanner)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location, state); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, struct asm_parser_state *state)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, state)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    struct asm_parser_state *state;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
  YYUSE (state);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, struct asm_parser_state *state)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, state)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    struct asm_parser_state *state;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, state);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, struct asm_parser_state *state)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, state)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    struct asm_parser_state *state;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       , state);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule, state); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, struct asm_parser_state *state)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, state)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    struct asm_parser_state *state;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (state);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (struct asm_parser_state *state);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */





/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (struct asm_parser_state *state)
#else
int
yyparse (state)
    struct asm_parser_state *state;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[2];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:

/* Line 1455 of yacc.c  */
#line 273 "program_parse.y"
    {
	   if (state->prog->Target != GL_VERTEX_PROGRAM_ARB) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid fragment program header");

	   }
	   state->mode = ARB_vertex;
	;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 281 "program_parse.y"
    {
	   if (state->prog->Target != GL_FRAGMENT_PROGRAM_ARB) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid vertex program header");
	   }
	   state->mode = ARB_fragment;

	   state->option.TexRect =
	      (state->ctx->Extensions.NV_texture_rectangle != GL_FALSE);
	;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 297 "program_parse.y"
    {
	   int valid = 0;

	   if (state->mode == ARB_vertex) {
	      valid = _mesa_ARBvp_parse_option(state, (yyvsp[(2) - (3)].string));
	   } else if (state->mode == ARB_fragment) {
	      valid = _mesa_ARBfp_parse_option(state, (yyvsp[(2) - (3)].string));
	   }


	   if (!valid) {
	      const char *const err_str = (state->mode == ARB_vertex)
		 ? "invalid ARB vertex program option"
		 : "invalid ARB fragment program option";

	      yyerror(& (yylsp[(2) - (3)]), state, err_str);
	      YYERROR;
	   }
	;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 323 "program_parse.y"
    {
	   if ((yyvsp[(1) - (2)].inst) != NULL) {
	      if (state->inst_tail == NULL) {
		 state->inst_head = (yyvsp[(1) - (2)].inst);
	      } else {
		 state->inst_tail->next = (yyvsp[(1) - (2)].inst);
	      }

	      state->inst_tail = (yyvsp[(1) - (2)].inst);
	      (yyvsp[(1) - (2)].inst)->next = NULL;

	      state->prog->NumInstructions++;
	   }
	;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 342 "program_parse.y"
    {
	   (yyval.inst) = (yyvsp[(1) - (1)].inst);
	   state->prog->NumAluInstructions++;
	;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 347 "program_parse.y"
    {
	   (yyval.inst) = (yyvsp[(1) - (1)].inst);
	   state->prog->NumTexInstructions++;
	;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 374 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (4)].temp_inst), & (yyvsp[(2) - (4)].dst_reg), & (yyvsp[(4) - (4)].src_reg), NULL, NULL);
	;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 380 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_ctor(OPCODE_ARA, & (yyvsp[(2) - (4)].dst_reg), & (yyvsp[(4) - (4)].src_reg), NULL, NULL);
	;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 386 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (4)].temp_inst), & (yyvsp[(2) - (4)].dst_reg), & (yyvsp[(4) - (4)].src_reg), NULL, NULL);
	;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 392 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (4)].temp_inst), & (yyvsp[(2) - (4)].dst_reg), & (yyvsp[(4) - (4)].src_reg), NULL, NULL);
	;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 398 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (6)].temp_inst), & (yyvsp[(2) - (6)].dst_reg), & (yyvsp[(4) - (6)].src_reg), & (yyvsp[(6) - (6)].src_reg), NULL);
	;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 405 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (6)].temp_inst), & (yyvsp[(2) - (6)].dst_reg), & (yyvsp[(4) - (6)].src_reg), & (yyvsp[(6) - (6)].src_reg), NULL);
	;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 412 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (8)].temp_inst), & (yyvsp[(2) - (8)].dst_reg), & (yyvsp[(4) - (8)].src_reg), & (yyvsp[(6) - (8)].src_reg), & (yyvsp[(8) - (8)].src_reg));
	;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 418 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (8)].temp_inst), & (yyvsp[(2) - (8)].dst_reg), & (yyvsp[(4) - (8)].src_reg), NULL, NULL);
	   if ((yyval.inst) != NULL) {
	      const GLbitfield tex_mask = (1U << (yyvsp[(6) - (8)].integer));
	      GLbitfield shadow_tex = 0;
	      GLbitfield target_mask = 0;


	      (yyval.inst)->Base.TexSrcUnit = (yyvsp[(6) - (8)].integer);

	      if ((yyvsp[(8) - (8)].integer) < 0) {
		 shadow_tex = tex_mask;

		 (yyval.inst)->Base.TexSrcTarget = -(yyvsp[(8) - (8)].integer);
		 (yyval.inst)->Base.TexShadow = 1;
	      } else {
		 (yyval.inst)->Base.TexSrcTarget = (yyvsp[(8) - (8)].integer);
	      }

	      target_mask = (1U << (yyval.inst)->Base.TexSrcTarget);

	      /* If this texture unit was previously accessed and that access
	       * had a different texture target, generate an error.
	       *
	       * If this texture unit was previously accessed and that access
	       * had a different shadow mode, generate an error.
	       */
	      if ((state->prog->TexturesUsed[(yyvsp[(6) - (8)].integer)] != 0)
		  && ((state->prog->TexturesUsed[(yyvsp[(6) - (8)].integer)] != target_mask)
		      || ((state->prog->ShadowSamplers & tex_mask)
			  != shadow_tex))) {
		 yyerror(& (yylsp[(8) - (8)]), state,
			 "multiple targets used on one texture image unit");
		 YYERROR;
	      }


	      state->prog->TexturesUsed[(yyvsp[(6) - (8)].integer)] |= target_mask;
	      state->prog->ShadowSamplers |= shadow_tex;
	   }
	;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 462 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_ctor(OPCODE_KIL, NULL, & (yyvsp[(2) - (2)].src_reg), NULL, NULL);
	   state->fragment.UsesKill = 1;
	;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 467 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_ctor(OPCODE_KIL_NV, NULL, NULL, NULL, NULL);
	   (yyval.inst)->Base.DstReg.CondMask = (yyvsp[(2) - (2)].dst_reg).CondMask;
	   (yyval.inst)->Base.DstReg.CondSwizzle = (yyvsp[(2) - (2)].dst_reg).CondSwizzle;
	   (yyval.inst)->Base.DstReg.CondSrc = (yyvsp[(2) - (2)].dst_reg).CondSrc;
	   state->fragment.UsesKill = 1;
	;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 477 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (12)].temp_inst), & (yyvsp[(2) - (12)].dst_reg), & (yyvsp[(4) - (12)].src_reg), & (yyvsp[(6) - (12)].src_reg), & (yyvsp[(8) - (12)].src_reg));
	   if ((yyval.inst) != NULL) {
	      const GLbitfield tex_mask = (1U << (yyvsp[(10) - (12)].integer));
	      GLbitfield shadow_tex = 0;
	      GLbitfield target_mask = 0;


	      (yyval.inst)->Base.TexSrcUnit = (yyvsp[(10) - (12)].integer);

	      if ((yyvsp[(12) - (12)].integer) < 0) {
		 shadow_tex = tex_mask;

		 (yyval.inst)->Base.TexSrcTarget = -(yyvsp[(12) - (12)].integer);
		 (yyval.inst)->Base.TexShadow = 1;
	      } else {
		 (yyval.inst)->Base.TexSrcTarget = (yyvsp[(12) - (12)].integer);
	      }

	      target_mask = (1U << (yyval.inst)->Base.TexSrcTarget);

	      /* If this texture unit was previously accessed and that access
	       * had a different texture target, generate an error.
	       *
	       * If this texture unit was previously accessed and that access
	       * had a different shadow mode, generate an error.
	       */
	      if ((state->prog->TexturesUsed[(yyvsp[(10) - (12)].integer)] != 0)
		  && ((state->prog->TexturesUsed[(yyvsp[(10) - (12)].integer)] != target_mask)
		      || ((state->prog->ShadowSamplers & tex_mask)
			  != shadow_tex))) {
		 yyerror(& (yylsp[(12) - (12)]), state,
			 "multiple targets used on one texture image unit");
		 YYERROR;
	      }


	      state->prog->TexturesUsed[(yyvsp[(10) - (12)].integer)] |= target_mask;
	      state->prog->ShadowSamplers |= shadow_tex;
	   }
	;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 521 "program_parse.y"
    {
	   (yyval.integer) = (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 526 "program_parse.y"
    { (yyval.integer) = TEXTURE_1D_INDEX; ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 527 "program_parse.y"
    { (yyval.integer) = TEXTURE_2D_INDEX; ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 528 "program_parse.y"
    { (yyval.integer) = TEXTURE_3D_INDEX; ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 529 "program_parse.y"
    { (yyval.integer) = TEXTURE_CUBE_INDEX; ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 530 "program_parse.y"
    { (yyval.integer) = TEXTURE_RECT_INDEX; ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 531 "program_parse.y"
    { (yyval.integer) = -TEXTURE_1D_INDEX; ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 532 "program_parse.y"
    { (yyval.integer) = -TEXTURE_2D_INDEX; ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 533 "program_parse.y"
    { (yyval.integer) = -TEXTURE_RECT_INDEX; ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 534 "program_parse.y"
    { (yyval.integer) = TEXTURE_1D_ARRAY_INDEX; ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 535 "program_parse.y"
    { (yyval.integer) = TEXTURE_2D_ARRAY_INDEX; ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 536 "program_parse.y"
    { (yyval.integer) = -TEXTURE_1D_ARRAY_INDEX; ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 537 "program_parse.y"
    { (yyval.integer) = -TEXTURE_2D_ARRAY_INDEX; ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 541 "program_parse.y"
    {
	   /* FIXME: Is this correct?  Should the extenedSwizzle be applied
	    * FIXME: to the existing swizzle?
	    */
	   (yyvsp[(4) - (6)].src_reg).Base.Swizzle = (yyvsp[(6) - (6)].swiz_mask).swizzle;
	   (yyvsp[(4) - (6)].src_reg).Base.Negate = (yyvsp[(6) - (6)].swiz_mask).mask;

	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (6)].temp_inst), & (yyvsp[(2) - (6)].dst_reg), & (yyvsp[(4) - (6)].src_reg), NULL, NULL);
	;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 553 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (3)].temp_inst), NULL, NULL, NULL, NULL);

	   /* Since we don't know whether or not the branch label is valid,
	    * store the position of the label.  This allows use to generate a
	    * sensible error message later.
	    */
	   (yyval.inst)->Base.Data = (yyvsp[(2) - (3)].string);
	   (yyval.inst)->position = (yylsp[(2) - (3)]);

	   (yyval.inst)->Base.DstReg.CondMask = (yyvsp[(3) - (3)].dst_reg).CondMask;
	   (yyval.inst)->Base.DstReg.CondSwizzle = (yyvsp[(3) - (3)].dst_reg).CondSwizzle;
	   (yyval.inst)->Base.DstReg.CondSrc = (yyvsp[(3) - (3)].dst_reg).CondSrc;
	;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 570 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (2)].temp_inst), NULL, NULL, NULL, NULL);

	   (yyval.inst)->Base.DstReg.CondMask = (yyvsp[(2) - (2)].dst_reg).CondMask;
	   (yyval.inst)->Base.DstReg.CondSwizzle = (yyvsp[(2) - (2)].dst_reg).CondSwizzle;
	   (yyval.inst)->Base.DstReg.CondSrc = (yyvsp[(2) - (2)].dst_reg).CondSrc;
	;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 580 "program_parse.y"
    {
	   (yyval.src_reg) = (yyvsp[(2) - (2)].src_reg);

	   if ((yyvsp[(1) - (2)].negate)) {
	      (yyval.src_reg).Base.Negate = ~(yyval.src_reg).Base.Negate;
	   }
	;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 588 "program_parse.y"
    {
	   (yyval.src_reg) = (yyvsp[(3) - (4)].src_reg);

	   if (!state->option.NV_fragment && !state->option.NV_vertex2) {
	      yyerror(& (yylsp[(2) - (4)]), state, "unexpected character '|'");
	      YYERROR;
	   }

	   if ((yyvsp[(1) - (4)].negate)) {
	      (yyval.src_reg).Base.Negate = ~(yyval.src_reg).Base.Negate;
	   }

	   (yyval.src_reg).Base.Abs = 1;
	;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 605 "program_parse.y"
    {
	   (yyval.src_reg) = (yyvsp[(1) - (2)].src_reg);

	   (yyval.src_reg).Base.Swizzle = _mesa_combine_swizzles((yyval.src_reg).Base.Swizzle,
						    (yyvsp[(2) - (2)].swiz_mask).swizzle);
	;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 612 "program_parse.y"
    {
	   struct asm_symbol temp_sym;

	   if (!state->option.NV_fragment && !state->option.NV_vertex2) {
	      yyerror(& (yylsp[(1) - (1)]), state, "expected scalar suffix");
	      YYERROR;
	   }

	   memset(& temp_sym, 0, sizeof(temp_sym));
	   temp_sym.param_binding_begin = ~0;
	   initialize_symbol_from_const(state->prog, & temp_sym, & (yyvsp[(1) - (1)].vector));

	   init_src_reg(& (yyval.src_reg));
	   (yyval.src_reg).Base.File = PROGRAM_CONSTANT;
	   (yyval.src_reg).Base.Index = temp_sym.param_binding_begin;
	;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 631 "program_parse.y"
    {
	   (yyval.src_reg) = (yyvsp[(2) - (3)].src_reg);

	   if ((yyvsp[(1) - (3)].negate)) {
	      (yyval.src_reg).Base.Negate = ~(yyval.src_reg).Base.Negate;
	   }

	   (yyval.src_reg).Base.Swizzle = _mesa_combine_swizzles((yyval.src_reg).Base.Swizzle,
						    (yyvsp[(3) - (3)].swiz_mask).swizzle);
	;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 642 "program_parse.y"
    {
	   (yyval.src_reg) = (yyvsp[(3) - (5)].src_reg);

	   if (!state->option.NV_fragment && !state->option.NV_vertex2) {
	      yyerror(& (yylsp[(2) - (5)]), state, "unexpected character '|'");
	      YYERROR;
	   }

	   if ((yyvsp[(1) - (5)].negate)) {
	      (yyval.src_reg).Base.Negate = ~(yyval.src_reg).Base.Negate;
	   }

	   (yyval.src_reg).Base.Abs = 1;
	   (yyval.src_reg).Base.Swizzle = _mesa_combine_swizzles((yyval.src_reg).Base.Swizzle,
						    (yyvsp[(4) - (5)].swiz_mask).swizzle);
	;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 662 "program_parse.y"
    {
	   (yyval.dst_reg) = (yyvsp[(1) - (3)].dst_reg);
	   (yyval.dst_reg).WriteMask = (yyvsp[(2) - (3)].swiz_mask).mask;
	   (yyval.dst_reg).CondMask = (yyvsp[(3) - (3)].dst_reg).CondMask;
	   (yyval.dst_reg).CondSwizzle = (yyvsp[(3) - (3)].dst_reg).CondSwizzle;
	   (yyval.dst_reg).CondSrc = (yyvsp[(3) - (3)].dst_reg).CondSrc;

	   if ((yyval.dst_reg).File == PROGRAM_OUTPUT) {
	      /* Technically speaking, this should check that it is in
	       * vertex program mode.  However, PositionInvariant can never be
	       * set in fragment program mode, so it is somewhat irrelevant.
	       */
	      if (state->option.PositionInvariant
	       && ((yyval.dst_reg).Index == VERT_RESULT_HPOS)) {
		 yyerror(& (yylsp[(1) - (3)]), state, "position-invariant programs cannot "
			 "write position");
		 YYERROR;
	      }

	      state->prog->OutputsWritten |= (1U << (yyval.dst_reg).Index);
	   }
	;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 687 "program_parse.y"
    {
	   if (!state->option.NV_vertex2) {
	      if ((yyvsp[(2) - (3)].swiz_mask).mask != WRITEMASK_X) {
		 yyerror(& (yylsp[(2) - (3)]), state,
			 "address register write mask must be \".x\"");
		 YYERROR;
	      }
	   }

	   init_dst_reg(& (yyval.dst_reg));
	   (yyval.dst_reg).File = PROGRAM_ADDRESS;
	   (yyval.dst_reg).Index = 0;
	   (yyval.dst_reg).WriteMask = (yyvsp[(2) - (3)].swiz_mask).mask;
	   (yyval.dst_reg).CondMask = (yyvsp[(3) - (3)].dst_reg).CondMask;
	   (yyval.dst_reg).CondSwizzle = (yyvsp[(3) - (3)].dst_reg).CondSwizzle;
	   (yyval.dst_reg).CondSrc = (yyvsp[(3) - (3)].dst_reg).CondSrc;
	;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 712 "program_parse.y"
    {
	   init_src_reg(& (yyval.src_reg));
	   (yyval.src_reg).Base.File = PROGRAM_ADDRESS;
	   (yyval.src_reg).Symbol = (yyvsp[(1) - (1)].sym);
	;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 720 "program_parse.y"
    {
	   const unsigned xyzw_valid =
	      ((yyvsp[(1) - (7)].ext_swizzle).xyzw_valid << 0)
	      | ((yyvsp[(3) - (7)].ext_swizzle).xyzw_valid << 1)
	      | ((yyvsp[(5) - (7)].ext_swizzle).xyzw_valid << 2)
	      | ((yyvsp[(7) - (7)].ext_swizzle).xyzw_valid << 3);
	   const unsigned rgba_valid =
	      ((yyvsp[(1) - (7)].ext_swizzle).rgba_valid << 0)
	      | ((yyvsp[(3) - (7)].ext_swizzle).rgba_valid << 1)
	      | ((yyvsp[(5) - (7)].ext_swizzle).rgba_valid << 2)
	      | ((yyvsp[(7) - (7)].ext_swizzle).rgba_valid << 3);

	   /* All of the swizzle components have to be valid in either RGBA
	    * or XYZW.  Note that 0 and 1 are valid in both, so both masks
	    * can have some bits set.
	    *
	    * We somewhat deviate from the spec here.  It would be really hard
	    * to figure out which component is the error, and there probably
	    * isn't a lot of benefit.
	    */
	   if ((rgba_valid != 0x0f) && (xyzw_valid != 0x0f)) {
	      yyerror(& (yylsp[(1) - (7)]), state, "cannot combine RGBA and XYZW swizzle "
		      "components");
	      YYERROR;
	   }

	   (yyval.swiz_mask).swizzle = MAKE_SWIZZLE4((yyvsp[(1) - (7)].ext_swizzle).swz, (yyvsp[(3) - (7)].ext_swizzle).swz, (yyvsp[(5) - (7)].ext_swizzle).swz, (yyvsp[(7) - (7)].ext_swizzle).swz);
	   (yyval.swiz_mask).mask = ((yyvsp[(1) - (7)].ext_swizzle).negate) | ((yyvsp[(3) - (7)].ext_swizzle).negate << 1) | ((yyvsp[(5) - (7)].ext_swizzle).negate << 2)
	      | ((yyvsp[(7) - (7)].ext_swizzle).negate << 3);
	;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 753 "program_parse.y"
    {
	   (yyval.ext_swizzle) = (yyvsp[(2) - (2)].ext_swizzle);
	   (yyval.ext_swizzle).negate = ((yyvsp[(1) - (2)].negate)) ? 1 : 0;
	;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 760 "program_parse.y"
    {
	   if (((yyvsp[(1) - (1)].integer) != 0) && ((yyvsp[(1) - (1)].integer) != 1)) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid extended swizzle selector");
	      YYERROR;
	   }

	   (yyval.ext_swizzle).swz = ((yyvsp[(1) - (1)].integer) == 0) ? SWIZZLE_ZERO : SWIZZLE_ONE;

	   /* 0 and 1 are valid for both RGBA swizzle names and XYZW
	    * swizzle names.
	    */
	   (yyval.ext_swizzle).xyzw_valid = 1;
	   (yyval.ext_swizzle).rgba_valid = 1;
	;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 775 "program_parse.y"
    {
	   if (strlen((yyvsp[(1) - (1)].string)) > 1) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid extended swizzle selector");
	      YYERROR;
	   }

	   switch ((yyvsp[(1) - (1)].string)[0]) {
	   case 'x':
	      (yyval.ext_swizzle).swz = SWIZZLE_X;
	      (yyval.ext_swizzle).xyzw_valid = 1;
	      break;
	   case 'y':
	      (yyval.ext_swizzle).swz = SWIZZLE_Y;
	      (yyval.ext_swizzle).xyzw_valid = 1;
	      break;
	   case 'z':
	      (yyval.ext_swizzle).swz = SWIZZLE_Z;
	      (yyval.ext_swizzle).xyzw_valid = 1;
	      break;
	   case 'w':
	      (yyval.ext_swizzle).swz = SWIZZLE_W;
	      (yyval.ext_swizzle).xyzw_valid = 1;
	      break;

	   case 'r':
	      (yyval.ext_swizzle).swz = SWIZZLE_X;
	      (yyval.ext_swizzle).rgba_valid = 1;
	      break;
	   case 'g':
	      (yyval.ext_swizzle).swz = SWIZZLE_Y;
	      (yyval.ext_swizzle).rgba_valid = 1;
	      break;
	   case 'b':
	      (yyval.ext_swizzle).swz = SWIZZLE_Z;
	      (yyval.ext_swizzle).rgba_valid = 1;
	      break;
	   case 'a':
	      (yyval.ext_swizzle).swz = SWIZZLE_W;
	      (yyval.ext_swizzle).rgba_valid = 1;
	      break;

	   default:
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid extended swizzle selector");
	      YYERROR;
	      break;
	   }
	;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 825 "program_parse.y"
    {
	   struct asm_symbol *const s = (struct asm_symbol *)
	      _mesa_symbol_table_find_symbol(state->st, 0, (yyvsp[(1) - (1)].string));

	   if (s == NULL) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid operand variable");
	      YYERROR;
	   } else if ((s->type != at_param) && (s->type != at_temp)
		      && (s->type != at_attrib)) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid operand variable");
	      YYERROR;
	   } else if ((s->type == at_param) && s->param_is_array) {
	      yyerror(& (yylsp[(1) - (1)]), state, "non-array access to array PARAM");
	      YYERROR;
	   }

	   init_src_reg(& (yyval.src_reg));
	   switch (s->type) {
	   case at_temp:
	      (yyval.src_reg).Base.File = PROGRAM_TEMPORARY;
	      (yyval.src_reg).Base.Index = s->temp_binding;
	      break;
	   case at_param:
	      (yyval.src_reg).Base.File = s->param_binding_type;
	      (yyval.src_reg).Base.Index = s->param_binding_begin;
	      break;
	   case at_attrib:
	      (yyval.src_reg).Base.File = PROGRAM_INPUT;
	      (yyval.src_reg).Base.Index = s->attrib_binding;
	      state->prog->InputsRead |= (1U << (yyval.src_reg).Base.Index);

	      if (!validate_inputs(& (yylsp[(1) - (1)]), state)) {
		 YYERROR;
	      }
	      break;

	   default:
	      YYERROR;
	      break;
	   }
	;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 867 "program_parse.y"
    {
	   init_src_reg(& (yyval.src_reg));
	   (yyval.src_reg).Base.File = PROGRAM_INPUT;
	   (yyval.src_reg).Base.Index = (yyvsp[(1) - (1)].attrib);
	   state->prog->InputsRead |= (1U << (yyval.src_reg).Base.Index);

	   if (!validate_inputs(& (yylsp[(1) - (1)]), state)) {
	      YYERROR;
	   }
	;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 878 "program_parse.y"
    {
	   if (! (yyvsp[(3) - (4)].src_reg).Base.RelAddr
	       && ((unsigned) (yyvsp[(3) - (4)].src_reg).Base.Index >= (yyvsp[(1) - (4)].sym)->param_binding_length)) {
	      yyerror(& (yylsp[(3) - (4)]), state, "out of bounds array access");
	      YYERROR;
	   }

	   init_src_reg(& (yyval.src_reg));
	   (yyval.src_reg).Base.File = (yyvsp[(1) - (4)].sym)->param_binding_type;

	   if ((yyvsp[(3) - (4)].src_reg).Base.RelAddr) {
	      (yyvsp[(1) - (4)].sym)->param_accessed_indirectly = 1;

	      (yyval.src_reg).Base.RelAddr = 1;
	      (yyval.src_reg).Base.Index = (yyvsp[(3) - (4)].src_reg).Base.Index;
	      (yyval.src_reg).Symbol = (yyvsp[(1) - (4)].sym);
	   } else {
	      (yyval.src_reg).Base.Index = (yyvsp[(1) - (4)].sym)->param_binding_begin + (yyvsp[(3) - (4)].src_reg).Base.Index;
	   }
	;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 899 "program_parse.y"
    {
	   init_src_reg(& (yyval.src_reg));
	   (yyval.src_reg).Base.File = ((yyvsp[(1) - (1)].temp_sym).name != NULL) 
	      ? (yyvsp[(1) - (1)].temp_sym).param_binding_type
	      : PROGRAM_CONSTANT;
	   (yyval.src_reg).Base.Index = (yyvsp[(1) - (1)].temp_sym).param_binding_begin;
	;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 909 "program_parse.y"
    {
	   init_dst_reg(& (yyval.dst_reg));
	   (yyval.dst_reg).File = PROGRAM_OUTPUT;
	   (yyval.dst_reg).Index = (yyvsp[(1) - (1)].result);
	;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 915 "program_parse.y"
    {
	   struct asm_symbol *const s = (struct asm_symbol *)
	      _mesa_symbol_table_find_symbol(state->st, 0, (yyvsp[(1) - (1)].string));

	   if (s == NULL) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid operand variable");
	      YYERROR;
	   } else if ((s->type != at_output) && (s->type != at_temp)) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid operand variable");
	      YYERROR;
	   }

	   init_dst_reg(& (yyval.dst_reg));
	   switch (s->type) {
	   case at_temp:
	      (yyval.dst_reg).File = PROGRAM_TEMPORARY;
	      (yyval.dst_reg).Index = s->temp_binding;
	      break;
	   case at_output:
	      (yyval.dst_reg).File = PROGRAM_OUTPUT;
	      (yyval.dst_reg).Index = s->output_binding;
	      break;
	   default:
	      (yyval.dst_reg).File = s->param_binding_type;
	      (yyval.dst_reg).Index = s->param_binding_begin;
	      break;
	   }
	;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 946 "program_parse.y"
    {
	   struct asm_symbol *const s = (struct asm_symbol *)
	      _mesa_symbol_table_find_symbol(state->st, 0, (yyvsp[(1) - (1)].string));

	   if (s == NULL) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid operand variable");
	      YYERROR;
	   } else if ((s->type != at_param) || !s->param_is_array) {
	      yyerror(& (yylsp[(1) - (1)]), state, "array access to non-PARAM variable");
	      YYERROR;
	   } else {
	      (yyval.sym) = s;
	   }
	;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 965 "program_parse.y"
    {
	   init_src_reg(& (yyval.src_reg));
	   (yyval.src_reg).Base.Index = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 972 "program_parse.y"
    {
	   /* FINISHME: Add support for multiple address registers.
	    */
	   init_src_reg(& (yyval.src_reg));
	   (yyval.src_reg).Base.RelAddr = 1;
	   (yyval.src_reg).Base.Index = (yyvsp[(3) - (3)].integer);
	   (yyval.src_reg).Base.AddrReg = 0;
	   (yyval.src_reg).Base.AddrComponent = (yyvsp[(2) - (3)].integer);
	;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 983 "program_parse.y"
    { (yyval.integer) = 0; ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 984 "program_parse.y"
    { (yyval.integer) = (yyvsp[(2) - (2)].integer); ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 985 "program_parse.y"
    { (yyval.integer) = -(yyvsp[(2) - (2)].integer); ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 989 "program_parse.y"
    {
	   if (((yyvsp[(1) - (1)].integer) < 0) || ((yyvsp[(1) - (1)].integer) > 63)) {
              char s[100];
              _mesa_snprintf(s, sizeof(s),
                             "relative address offset too large (%d)", (yyvsp[(1) - (1)].integer));
	      yyerror(& (yylsp[(1) - (1)]), state, s);
	      YYERROR;
	   } else {
	      (yyval.integer) = (yyvsp[(1) - (1)].integer);
	   }
	;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 1003 "program_parse.y"
    {
	   if (((yyvsp[(1) - (1)].integer) < 0) || ((yyvsp[(1) - (1)].integer) > 64)) {
              char s[100];
              _mesa_snprintf(s, sizeof(s),
                             "relative address offset too large (%d)", (yyvsp[(1) - (1)].integer));
	      yyerror(& (yylsp[(1) - (1)]), state, s);
	      YYERROR;
	   } else {
	      (yyval.integer) = (yyvsp[(1) - (1)].integer);
	   }
	;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 1017 "program_parse.y"
    {
	   struct asm_symbol *const s = (struct asm_symbol *)
	      _mesa_symbol_table_find_symbol(state->st, 0, (yyvsp[(1) - (1)].string));

	   if (s == NULL) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid array member");
	      YYERROR;
	   } else if (s->type != at_address) {
	      yyerror(& (yylsp[(1) - (1)]), state,
		      "invalid variable for indexed array access");
	      YYERROR;
	   } else {
	      (yyval.sym) = s;
	   }
	;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 1035 "program_parse.y"
    {
	   if (!state->option.NV_vertex2 && ((yyvsp[(1) - (1)].swiz_mask).mask != WRITEMASK_X)) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid address component selector");
	      YYERROR;
	   } else {
	      switch ((yyvsp[(1) - (1)].swiz_mask).mask) {
	      case WRITEMASK_X: (yyval.integer) = 0; break;
	      case WRITEMASK_Y: (yyval.integer) = 1; break;
	      case WRITEMASK_Z: (yyval.integer) = 2; break;
	      case WRITEMASK_W: (yyval.integer) = 3; break;

	      /* It should be impossible to get here because the RHS of the
	       * production is MASK1.
	       */
	      default: YYERROR; break;
	      }
	   }
	;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 1060 "program_parse.y"
    { (yyval.swiz_mask).swizzle = SWIZZLE_NOOP; (yyval.swiz_mask).mask = WRITEMASK_XYZW; ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 1064 "program_parse.y"
    { (yyval.swiz_mask).swizzle = SWIZZLE_NOOP; (yyval.swiz_mask).mask = WRITEMASK_XYZW; ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 1068 "program_parse.y"
    {
	   if (!state->option.NV_vertex2
	       && !state->option.NV_fragment) {
	      yyerror(& (yylsp[(1) - (3)]), state,
		      "conditional write mask invalid in this program mode");
	      YYERROR;
	   }

	   (yyval.dst_reg) = (yyvsp[(2) - (3)].dst_reg);
	;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 1079 "program_parse.y"
    {
	   if (!state->option.NV_vertex2
	       && !state->option.NV_fragment) {
	      yyerror(& (yylsp[(1) - (3)]), state,
		      "conditional write mask invalid in this program mode");
	      YYERROR;
	   }

	   (yyval.dst_reg) = (yyvsp[(2) - (3)].dst_reg);
	;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 1090 "program_parse.y"
    {
	   (yyval.dst_reg).CondMask = COND_TR;
	   (yyval.dst_reg).CondSwizzle = SWIZZLE_NOOP;
	   (yyval.dst_reg).CondSrc = 0;
	;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 1098 "program_parse.y"
    {
	   (yyval.dst_reg) = (yyvsp[(1) - (2)].dst_reg);
	   (yyval.dst_reg).CondSwizzle = (yyvsp[(2) - (2)].swiz_mask).swizzle;
	;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 1105 "program_parse.y"
    {
	   (yyval.dst_reg) = (yyvsp[(1) - (2)].dst_reg);
	   (yyval.dst_reg).CondSwizzle = (yyvsp[(2) - (2)].swiz_mask).swizzle;
	;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 1112 "program_parse.y"
    {
	   const int cond = _mesa_parse_cc((yyvsp[(1) - (1)].string));
	   if ((cond == 0) || ((yyvsp[(1) - (1)].string)[2] != '\0')) {
	      char *const err_str =
		 make_error_string("invalid condition code \"%s\"", (yyvsp[(1) - (1)].string));

	      yyerror(& (yylsp[(1) - (1)]), state, (err_str != NULL)
		      ? err_str : "invalid condition code");

	      if (err_str != NULL) {
		 _mesa_free(err_str);
	      }

	      YYERROR;
	   }

	   (yyval.dst_reg).CondMask = cond;
	   (yyval.dst_reg).CondSwizzle = SWIZZLE_NOOP;
	   (yyval.dst_reg).CondSrc = 0;
	;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 1135 "program_parse.y"
    {
	   const int cond = _mesa_parse_cc((yyvsp[(1) - (1)].string));
	   if ((cond == 0) || ((yyvsp[(1) - (1)].string)[2] != '\0')) {
	      char *const err_str =
		 make_error_string("invalid condition code \"%s\"", (yyvsp[(1) - (1)].string));

	      yyerror(& (yylsp[(1) - (1)]), state, (err_str != NULL)
		      ? err_str : "invalid condition code");

	      if (err_str != NULL) {
		 _mesa_free(err_str);
	      }

	      YYERROR;
	   }

	   (yyval.dst_reg).CondMask = cond;
	   (yyval.dst_reg).CondSwizzle = SWIZZLE_NOOP;
	   (yyval.dst_reg).CondSrc = 0;
	;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 1158 "program_parse.y"
    {
	   const void *const label =
	      _mesa_symbol_table_find_symbol(state->st, 1, (yyvsp[(1) - (1)].string));

	   if (label != NULL) {
	      yyerror(& (yylsp[(1) - (1)]), state, "duplicate label");
	      YYERROR;
	   } else {
	      /* We have to bias the offset by one so that we don't add a
	       * symbol with zero value.  That will fail the symbol existence
	       * test above.
	       */
	      const intptr_t offset = state->prog->NumInstructions + 1;
	      _mesa_symbol_table_add_symbol(state->st, 1, (yyvsp[(1) - (1)].string), (void *) offset);
	   }
	;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 1188 "program_parse.y"
    {
	   struct asm_symbol *const s =
	      declare_variable(state, (yyvsp[(2) - (4)].string), at_attrib, & (yylsp[(2) - (4)]));

	   if (s == NULL) {
	      YYERROR;
	   } else {
	      s->attrib_binding = (yyvsp[(4) - (4)].attrib);
	      state->InputsBound |= (1U << s->attrib_binding);

	      if (!validate_inputs(& (yylsp[(4) - (4)]), state)) {
		 YYERROR;
	      }
	   }
	;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 1206 "program_parse.y"
    {
	   (yyval.attrib) = (yyvsp[(2) - (2)].attrib);
	;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 1210 "program_parse.y"
    {
	   (yyval.attrib) = (yyvsp[(2) - (2)].attrib);
	;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 1216 "program_parse.y"
    {
	   (yyval.attrib) = VERT_ATTRIB_POS;
	;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 1220 "program_parse.y"
    {
	   (yyval.attrib) = VERT_ATTRIB_WEIGHT;
	;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 1224 "program_parse.y"
    {
	   (yyval.attrib) = VERT_ATTRIB_NORMAL;
	;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 1228 "program_parse.y"
    {
	   if (!state->ctx->Extensions.EXT_secondary_color) {
	      yyerror(& (yylsp[(2) - (2)]), state, "GL_EXT_secondary_color not supported");
	      YYERROR;
	   }

	   (yyval.attrib) = VERT_ATTRIB_COLOR0 + (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 1237 "program_parse.y"
    {
	   if (!state->ctx->Extensions.EXT_fog_coord) {
	      yyerror(& (yylsp[(1) - (1)]), state, "GL_EXT_fog_coord not supported");
	      YYERROR;
	   }

	   (yyval.attrib) = VERT_ATTRIB_FOG;
	;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 1246 "program_parse.y"
    {
	   (yyval.attrib) = VERT_ATTRIB_TEX0 + (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 1250 "program_parse.y"
    {
	   yyerror(& (yylsp[(1) - (4)]), state, "GL_ARB_matrix_palette not supported");
	   YYERROR;
	;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 1255 "program_parse.y"
    {
	   (yyval.attrib) = VERT_ATTRIB_GENERIC0 + (yyvsp[(3) - (4)].integer);
	;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 1261 "program_parse.y"
    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->limits->MaxAttribs) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid vertex attribute reference");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 1275 "program_parse.y"
    {
	   (yyval.attrib) = FRAG_ATTRIB_WPOS;
	;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 1279 "program_parse.y"
    {
	   (yyval.attrib) = FRAG_ATTRIB_COL0 + (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 1283 "program_parse.y"
    {
	   (yyval.attrib) = FRAG_ATTRIB_FOGC;
	;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 1287 "program_parse.y"
    {
	   (yyval.attrib) = FRAG_ATTRIB_TEX0 + (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 1295 "program_parse.y"
    {
	   struct asm_symbol *const s =
	      declare_variable(state, (yyvsp[(2) - (3)].string), at_param, & (yylsp[(2) - (3)]));

	   if (s == NULL) {
	      YYERROR;
	   } else {
	      s->param_binding_type = (yyvsp[(3) - (3)].temp_sym).param_binding_type;
	      s->param_binding_begin = (yyvsp[(3) - (3)].temp_sym).param_binding_begin;
	      s->param_binding_length = (yyvsp[(3) - (3)].temp_sym).param_binding_length;
	      s->param_is_array = 0;
	   }
	;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 1311 "program_parse.y"
    {
	   if (((yyvsp[(4) - (6)].integer) != 0) && ((unsigned) (yyvsp[(4) - (6)].integer) != (yyvsp[(6) - (6)].temp_sym).param_binding_length)) {
	      yyerror(& (yylsp[(4) - (6)]), state, 
		      "parameter array size and number of bindings must match");
	      YYERROR;
	   } else {
	      struct asm_symbol *const s =
		 declare_variable(state, (yyvsp[(2) - (6)].string), (yyvsp[(6) - (6)].temp_sym).type, & (yylsp[(2) - (6)]));

	      if (s == NULL) {
		 YYERROR;
	      } else {
		 s->param_binding_type = (yyvsp[(6) - (6)].temp_sym).param_binding_type;
		 s->param_binding_begin = (yyvsp[(6) - (6)].temp_sym).param_binding_begin;
		 s->param_binding_length = (yyvsp[(6) - (6)].temp_sym).param_binding_length;
		 s->param_is_array = 1;
	      }
	   }
	;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 1333 "program_parse.y"
    {
	   (yyval.integer) = 0;
	;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 1337 "program_parse.y"
    {
	   if (((yyvsp[(1) - (1)].integer) < 1) || ((unsigned) (yyvsp[(1) - (1)].integer) >= state->limits->MaxParameters)) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid parameter array size");
	      YYERROR;
	   } else {
	      (yyval.integer) = (yyvsp[(1) - (1)].integer);
	   }
	;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 1348 "program_parse.y"
    {
	   (yyval.temp_sym) = (yyvsp[(2) - (2)].temp_sym);
	;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 1354 "program_parse.y"
    {
	   (yyval.temp_sym) = (yyvsp[(3) - (4)].temp_sym);
	;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 1361 "program_parse.y"
    {
	   (yyvsp[(1) - (3)].temp_sym).param_binding_length += (yyvsp[(3) - (3)].temp_sym).param_binding_length;
	   (yyval.temp_sym) = (yyvsp[(1) - (3)].temp_sym);
	;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 1368 "program_parse.y"
    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_state(state->prog, & (yyval.temp_sym), (yyvsp[(1) - (1)].state));
	;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 1374 "program_parse.y"
    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_param(state->prog, & (yyval.temp_sym), (yyvsp[(1) - (1)].state));
	;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 1380 "program_parse.y"
    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_const(state->prog, & (yyval.temp_sym), & (yyvsp[(1) - (1)].vector));
	;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 1388 "program_parse.y"
    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_state(state->prog, & (yyval.temp_sym), (yyvsp[(1) - (1)].state));
	;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 1394 "program_parse.y"
    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_param(state->prog, & (yyval.temp_sym), (yyvsp[(1) - (1)].state));
	;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 1400 "program_parse.y"
    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_const(state->prog, & (yyval.temp_sym), & (yyvsp[(1) - (1)].vector));
	;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 1408 "program_parse.y"
    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_state(state->prog, & (yyval.temp_sym), (yyvsp[(1) - (1)].state));
	;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 1414 "program_parse.y"
    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_param(state->prog, & (yyval.temp_sym), (yyvsp[(1) - (1)].state));
	;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 1420 "program_parse.y"
    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_const(state->prog, & (yyval.temp_sym), & (yyvsp[(1) - (1)].vector));
	;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 1427 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(1) - (1)].state), sizeof((yyval.state))); ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 1428 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 1431 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 1432 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 1433 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 1434 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 1435 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1436 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 1437 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 1438 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 1439 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 1440 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1441 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1445 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_MATERIAL;
	   (yyval.state)[1] = (yyvsp[(2) - (3)].integer);
	   (yyval.state)[2] = (yyvsp[(3) - (3)].integer);
	;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1454 "program_parse.y"
    {
	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1458 "program_parse.y"
    {
	   (yyval.integer) = STATE_EMISSION;
	;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1462 "program_parse.y"
    {
	   (yyval.integer) = STATE_SHININESS;
	;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1468 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_LIGHT;
	   (yyval.state)[1] = (yyvsp[(3) - (5)].integer);
	   (yyval.state)[2] = (yyvsp[(5) - (5)].integer);
	;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1477 "program_parse.y"
    {
	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1481 "program_parse.y"
    {
	   (yyval.integer) = STATE_POSITION;
	;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1485 "program_parse.y"
    {
	   if (!state->ctx->Extensions.EXT_point_parameters) {
	      yyerror(& (yylsp[(1) - (1)]), state, "GL_ARB_point_parameters not supported");
	      YYERROR;
	   }

	   (yyval.integer) = STATE_ATTENUATION;
	;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1494 "program_parse.y"
    {
	   (yyval.integer) = (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1498 "program_parse.y"
    {
	   (yyval.integer) = STATE_HALF_VECTOR;
	;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1504 "program_parse.y"
    {
	   (yyval.integer) = STATE_SPOT_DIRECTION;
	;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1510 "program_parse.y"
    {
	   (yyval.state)[0] = (yyvsp[(2) - (2)].state)[0];
	   (yyval.state)[1] = (yyvsp[(2) - (2)].state)[1];
	;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1517 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_LIGHTMODEL_AMBIENT;
	;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1522 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_LIGHTMODEL_SCENECOLOR;
	   (yyval.state)[1] = (yyvsp[(1) - (2)].integer);
	;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1530 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_LIGHTPROD;
	   (yyval.state)[1] = (yyvsp[(3) - (6)].integer);
	   (yyval.state)[2] = (yyvsp[(5) - (6)].integer);
	   (yyval.state)[3] = (yyvsp[(6) - (6)].integer);
	;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1542 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = (yyvsp[(3) - (3)].integer);
	   (yyval.state)[1] = (yyvsp[(2) - (3)].integer);
	;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1550 "program_parse.y"
    {
	   (yyval.integer) = STATE_TEXENV_COLOR;
	;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1556 "program_parse.y"
    {
	   (yyval.integer) = STATE_AMBIENT;
	;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1560 "program_parse.y"
    {
	   (yyval.integer) = STATE_DIFFUSE;
	;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1564 "program_parse.y"
    {
	   (yyval.integer) = STATE_SPECULAR;
	;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1570 "program_parse.y"
    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->MaxLights) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid light selector");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1581 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_TEXGEN;
	   (yyval.state)[1] = (yyvsp[(2) - (4)].integer);
	   (yyval.state)[2] = (yyvsp[(3) - (4)].integer) + (yyvsp[(4) - (4)].integer);
	;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1590 "program_parse.y"
    {
	   (yyval.integer) = STATE_TEXGEN_EYE_S;
	;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1594 "program_parse.y"
    {
	   (yyval.integer) = STATE_TEXGEN_OBJECT_S;
	;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1599 "program_parse.y"
    {
	   (yyval.integer) = STATE_TEXGEN_EYE_S - STATE_TEXGEN_EYE_S;
	;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1603 "program_parse.y"
    {
	   (yyval.integer) = STATE_TEXGEN_EYE_T - STATE_TEXGEN_EYE_S;
	;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1607 "program_parse.y"
    {
	   (yyval.integer) = STATE_TEXGEN_EYE_R - STATE_TEXGEN_EYE_S;
	;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1611 "program_parse.y"
    {
	   (yyval.integer) = STATE_TEXGEN_EYE_Q - STATE_TEXGEN_EYE_S;
	;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1617 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1624 "program_parse.y"
    {
	   (yyval.integer) = STATE_FOG_COLOR;
	;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1628 "program_parse.y"
    {
	   (yyval.integer) = STATE_FOG_PARAMS;
	;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1634 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_CLIPPLANE;
	   (yyval.state)[1] = (yyvsp[(3) - (5)].integer);
	;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1642 "program_parse.y"
    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->MaxClipPlanes) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid clip plane selector");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1653 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1660 "program_parse.y"
    {
	   (yyval.integer) = STATE_POINT_SIZE;
	;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1664 "program_parse.y"
    {
	   (yyval.integer) = STATE_POINT_ATTENUATION;
	;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1670 "program_parse.y"
    {
	   (yyval.state)[0] = (yyvsp[(1) - (5)].state)[0];
	   (yyval.state)[1] = (yyvsp[(1) - (5)].state)[1];
	   (yyval.state)[2] = (yyvsp[(4) - (5)].integer);
	   (yyval.state)[3] = (yyvsp[(4) - (5)].integer);
	   (yyval.state)[4] = (yyvsp[(1) - (5)].state)[2];
	;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1680 "program_parse.y"
    {
	   (yyval.state)[0] = (yyvsp[(1) - (2)].state)[0];
	   (yyval.state)[1] = (yyvsp[(1) - (2)].state)[1];
	   (yyval.state)[2] = (yyvsp[(2) - (2)].state)[2];
	   (yyval.state)[3] = (yyvsp[(2) - (2)].state)[3];
	   (yyval.state)[4] = (yyvsp[(1) - (2)].state)[2];
	;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1690 "program_parse.y"
    {
	   (yyval.state)[2] = 0;
	   (yyval.state)[3] = 3;
	;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1695 "program_parse.y"
    {
	   /* It seems logical that the matrix row range specifier would have
	    * to specify a range or more than one row (i.e., $5 > $3).
	    * However, the ARB_vertex_program spec says "a program will fail
	    * to load if <a> is greater than <b>."  This means that $3 == $5
	    * is valid.
	    */
	   if ((yyvsp[(3) - (6)].integer) > (yyvsp[(5) - (6)].integer)) {
	      yyerror(& (yylsp[(3) - (6)]), state, "invalid matrix row range");
	      YYERROR;
	   }

	   (yyval.state)[2] = (yyvsp[(3) - (6)].integer);
	   (yyval.state)[3] = (yyvsp[(5) - (6)].integer);
	;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1713 "program_parse.y"
    {
	   (yyval.state)[0] = (yyvsp[(2) - (3)].state)[0];
	   (yyval.state)[1] = (yyvsp[(2) - (3)].state)[1];
	   (yyval.state)[2] = (yyvsp[(3) - (3)].integer);
	;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1721 "program_parse.y"
    {
	   (yyval.integer) = 0;
	;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1725 "program_parse.y"
    {
	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1731 "program_parse.y"
    {
	   (yyval.integer) = STATE_MATRIX_INVERSE;
	;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1735 "program_parse.y"
    {
	   (yyval.integer) = STATE_MATRIX_TRANSPOSE;
	;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1739 "program_parse.y"
    {
	   (yyval.integer) = STATE_MATRIX_INVTRANS;
	;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1745 "program_parse.y"
    {
	   if ((yyvsp[(1) - (1)].integer) > 3) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid matrix row reference");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1756 "program_parse.y"
    {
	   (yyval.state)[0] = STATE_MODELVIEW_MATRIX;
	   (yyval.state)[1] = (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1761 "program_parse.y"
    {
	   (yyval.state)[0] = STATE_PROJECTION_MATRIX;
	   (yyval.state)[1] = 0;
	;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1766 "program_parse.y"
    {
	   (yyval.state)[0] = STATE_MVP_MATRIX;
	   (yyval.state)[1] = 0;
	;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1771 "program_parse.y"
    {
	   (yyval.state)[0] = STATE_TEXTURE_MATRIX;
	   (yyval.state)[1] = (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1776 "program_parse.y"
    {
	   yyerror(& (yylsp[(1) - (4)]), state, "GL_ARB_matrix_palette not supported");
	   YYERROR;
	;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1781 "program_parse.y"
    {
	   (yyval.state)[0] = STATE_PROGRAM_MATRIX;
	   (yyval.state)[1] = (yyvsp[(3) - (4)].integer);
	;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1788 "program_parse.y"
    {
	   (yyval.integer) = 0;
	;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1792 "program_parse.y"
    {
	   (yyval.integer) = (yyvsp[(2) - (3)].integer);
	;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1797 "program_parse.y"
    {
	   /* Since GL_ARB_vertex_blend isn't supported, only modelview matrix
	    * zero is valid.
	    */
	   if ((yyvsp[(1) - (1)].integer) != 0) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid modelview matrix index");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1810 "program_parse.y"
    {
	   /* Since GL_ARB_matrix_palette isn't supported, just let any value
	    * through here.  The error will be generated later.
	    */
	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1818 "program_parse.y"
    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->MaxProgramMatrices) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid program matrix selector");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1829 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_DEPTH_RANGE;
	;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1841 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = state->state_param_enum;
	   (yyval.state)[1] = STATE_ENV;
	   (yyval.state)[2] = (yyvsp[(4) - (5)].state)[0];
	   (yyval.state)[3] = (yyvsp[(4) - (5)].state)[1];
	;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1851 "program_parse.y"
    {
	   (yyval.state)[0] = (yyvsp[(1) - (1)].integer);
	   (yyval.state)[1] = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1856 "program_parse.y"
    {
	   (yyval.state)[0] = (yyvsp[(1) - (3)].integer);
	   (yyval.state)[1] = (yyvsp[(3) - (3)].integer);
	;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1863 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = state->state_param_enum;
	   (yyval.state)[1] = STATE_ENV;
	   (yyval.state)[2] = (yyvsp[(4) - (5)].integer);
	   (yyval.state)[3] = (yyvsp[(4) - (5)].integer);
	;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1873 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = state->state_param_enum;
	   (yyval.state)[1] = STATE_LOCAL;
	   (yyval.state)[2] = (yyvsp[(4) - (5)].state)[0];
	   (yyval.state)[3] = (yyvsp[(4) - (5)].state)[1];
	;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1882 "program_parse.y"
    {
	   (yyval.state)[0] = (yyvsp[(1) - (1)].integer);
	   (yyval.state)[1] = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1887 "program_parse.y"
    {
	   (yyval.state)[0] = (yyvsp[(1) - (3)].integer);
	   (yyval.state)[1] = (yyvsp[(3) - (3)].integer);
	;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1894 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = state->state_param_enum;
	   (yyval.state)[1] = STATE_LOCAL;
	   (yyval.state)[2] = (yyvsp[(4) - (5)].integer);
	   (yyval.state)[3] = (yyvsp[(4) - (5)].integer);
	;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1904 "program_parse.y"
    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->limits->MaxEnvParams) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid environment parameter reference");
	      YYERROR;
	   }
	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1914 "program_parse.y"
    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->limits->MaxLocalParams) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid local parameter reference");
	      YYERROR;
	   }
	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1929 "program_parse.y"
    {
	   (yyval.vector).count = 4;
	   (yyval.vector).data[0] = (yyvsp[(1) - (1)].real);
	   (yyval.vector).data[1] = (yyvsp[(1) - (1)].real);
	   (yyval.vector).data[2] = (yyvsp[(1) - (1)].real);
	   (yyval.vector).data[3] = (yyvsp[(1) - (1)].real);
	;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1939 "program_parse.y"
    {
	   (yyval.vector).count = 1;
	   (yyval.vector).data[0] = (yyvsp[(1) - (1)].real);
	   (yyval.vector).data[1] = (yyvsp[(1) - (1)].real);
	   (yyval.vector).data[2] = (yyvsp[(1) - (1)].real);
	   (yyval.vector).data[3] = (yyvsp[(1) - (1)].real);
	;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1947 "program_parse.y"
    {
	   (yyval.vector).count = 1;
	   (yyval.vector).data[0] = (float) (yyvsp[(1) - (1)].integer);
	   (yyval.vector).data[1] = (float) (yyvsp[(1) - (1)].integer);
	   (yyval.vector).data[2] = (float) (yyvsp[(1) - (1)].integer);
	   (yyval.vector).data[3] = (float) (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1957 "program_parse.y"
    {
	   (yyval.vector).count = 4;
	   (yyval.vector).data[0] = (yyvsp[(2) - (3)].real);
	   (yyval.vector).data[1] = 0.0f;
	   (yyval.vector).data[2] = 0.0f;
	   (yyval.vector).data[3] = 1.0f;
	;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1965 "program_parse.y"
    {
	   (yyval.vector).count = 4;
	   (yyval.vector).data[0] = (yyvsp[(2) - (5)].real);
	   (yyval.vector).data[1] = (yyvsp[(4) - (5)].real);
	   (yyval.vector).data[2] = 0.0f;
	   (yyval.vector).data[3] = 1.0f;
	;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1974 "program_parse.y"
    {
	   (yyval.vector).count = 4;
	   (yyval.vector).data[0] = (yyvsp[(2) - (7)].real);
	   (yyval.vector).data[1] = (yyvsp[(4) - (7)].real);
	   (yyval.vector).data[2] = (yyvsp[(6) - (7)].real);
	   (yyval.vector).data[3] = 1.0f;
	;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1983 "program_parse.y"
    {
	   (yyval.vector).count = 4;
	   (yyval.vector).data[0] = (yyvsp[(2) - (9)].real);
	   (yyval.vector).data[1] = (yyvsp[(4) - (9)].real);
	   (yyval.vector).data[2] = (yyvsp[(6) - (9)].real);
	   (yyval.vector).data[3] = (yyvsp[(8) - (9)].real);
	;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1993 "program_parse.y"
    {
	   (yyval.real) = ((yyvsp[(1) - (2)].negate)) ? -(yyvsp[(2) - (2)].real) : (yyvsp[(2) - (2)].real);
	;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1997 "program_parse.y"
    {
	   (yyval.real) = (float)(((yyvsp[(1) - (2)].negate)) ? -(yyvsp[(2) - (2)].integer) : (yyvsp[(2) - (2)].integer));
	;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 2002 "program_parse.y"
    { (yyval.negate) = FALSE; ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 2003 "program_parse.y"
    { (yyval.negate) = TRUE;  ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 2004 "program_parse.y"
    { (yyval.negate) = FALSE; ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 2007 "program_parse.y"
    { (yyval.integer) = (yyvsp[(2) - (2)].integer); ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 2011 "program_parse.y"
    {
	   /* NV_fragment_program_option defines the size qualifiers in a
	    * fairly broken way.  "SHORT" or "LONG" can optionally be used
	    * before TEMP or OUTPUT.  However, neither is a reserved word!
	    * This means that we have to parse it as an identifier, then check
	    * to make sure it's one of the valid values.  *sigh*
	    *
	    * In addition, the grammar in the extension spec does *not* allow
	    * the size specifier to be optional, but all known implementations
	    * do.
	    */
	   if (!state->option.NV_fragment) {
	      yyerror(& (yylsp[(1) - (1)]), state, "unexpected IDENTIFIER");
	      YYERROR;
	   }

	   if (strcmp("SHORT", (yyvsp[(1) - (1)].string)) == 0) {
	   } else if (strcmp("LONG", (yyvsp[(1) - (1)].string)) == 0) {
	   } else {
	      char *const err_str =
		 make_error_string("invalid storage size specifier \"%s\"",
				   (yyvsp[(1) - (1)].string));

	      yyerror(& (yylsp[(1) - (1)]), state, (err_str != NULL)
		      ? err_str : "invalid storage size specifier");

	      if (err_str != NULL) {
		 _mesa_free(err_str);
	      }

	      YYERROR;
	   }
	;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 2045 "program_parse.y"
    {
	;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 2049 "program_parse.y"
    { (yyval.integer) = (yyvsp[(1) - (1)].integer); ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 2053 "program_parse.y"
    {
	   if (!declare_variable(state, (yyvsp[(3) - (3)].string), (yyvsp[(0) - (3)].integer), & (yylsp[(3) - (3)]))) {
	      YYERROR;
	   }
	;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 2059 "program_parse.y"
    {
	   if (!declare_variable(state, (yyvsp[(1) - (1)].string), (yyvsp[(0) - (1)].integer), & (yylsp[(1) - (1)]))) {
	      YYERROR;
	   }
	;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 2067 "program_parse.y"
    {
	   struct asm_symbol *const s =
	      declare_variable(state, (yyvsp[(3) - (5)].string), at_output, & (yylsp[(3) - (5)]));

	   if (s == NULL) {
	      YYERROR;
	   } else {
	      s->output_binding = (yyvsp[(5) - (5)].result);
	   }
	;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 2080 "program_parse.y"
    {
	   if (state->mode == ARB_vertex) {
	      (yyval.result) = VERT_RESULT_HPOS;
	   } else {
	      yyerror(& (yylsp[(2) - (2)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 2089 "program_parse.y"
    {
	   if (state->mode == ARB_vertex) {
	      (yyval.result) = VERT_RESULT_FOGC;
	   } else {
	      yyerror(& (yylsp[(2) - (2)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 2098 "program_parse.y"
    {
	   (yyval.result) = (yyvsp[(2) - (2)].result);
	;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 2102 "program_parse.y"
    {
	   if (state->mode == ARB_vertex) {
	      (yyval.result) = VERT_RESULT_PSIZ;
	   } else {
	      yyerror(& (yylsp[(2) - (2)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 2111 "program_parse.y"
    {
	   if (state->mode == ARB_vertex) {
	      (yyval.result) = VERT_RESULT_TEX0 + (yyvsp[(3) - (3)].integer);
	   } else {
	      yyerror(& (yylsp[(2) - (3)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 2120 "program_parse.y"
    {
	   if (state->mode == ARB_fragment) {
	      (yyval.result) = FRAG_RESULT_DEPTH;
	   } else {
	      yyerror(& (yylsp[(2) - (2)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 2131 "program_parse.y"
    {
	   (yyval.result) = (yyvsp[(2) - (3)].integer) + (yyvsp[(3) - (3)].integer);
	;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 2137 "program_parse.y"
    {
	   (yyval.integer) = (state->mode == ARB_vertex)
	      ? VERT_RESULT_COL0
	      : FRAG_RESULT_COLOR;
	;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 2143 "program_parse.y"
    {
	   if (state->mode == ARB_vertex) {
	      (yyval.integer) = VERT_RESULT_COL0;
	   } else {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 2152 "program_parse.y"
    {
	   if (state->mode == ARB_vertex) {
	      (yyval.integer) = VERT_RESULT_BFC0;
	   } else {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 2163 "program_parse.y"
    {
	   (yyval.integer) = 0; 
	;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 2167 "program_parse.y"
    {
	   if (state->mode == ARB_vertex) {
	      (yyval.integer) = 0;
	   } else {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 2176 "program_parse.y"
    {
	   if (state->mode == ARB_vertex) {
	      (yyval.integer) = 1;
	   } else {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 2186 "program_parse.y"
    { (yyval.integer) = 0; ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 2187 "program_parse.y"
    { (yyval.integer) = 0; ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 2188 "program_parse.y"
    { (yyval.integer) = 1; ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 2191 "program_parse.y"
    { (yyval.integer) = 0; ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 2192 "program_parse.y"
    { (yyval.integer) = 0; ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 2193 "program_parse.y"
    { (yyval.integer) = 1; ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 2196 "program_parse.y"
    { (yyval.integer) = 0; ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 2197 "program_parse.y"
    { (yyval.integer) = (yyvsp[(2) - (3)].integer); ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 2200 "program_parse.y"
    { (yyval.integer) = 0; ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 2201 "program_parse.y"
    { (yyval.integer) = (yyvsp[(2) - (3)].integer); ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 2204 "program_parse.y"
    { (yyval.integer) = 0; ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 2205 "program_parse.y"
    { (yyval.integer) = (yyvsp[(2) - (3)].integer); ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 2209 "program_parse.y"
    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->MaxTextureCoordUnits) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid texture coordinate unit selector");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 2220 "program_parse.y"
    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->MaxTextureImageUnits) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid texture image unit selector");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 2231 "program_parse.y"
    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->MaxTextureUnits) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid texture unit selector");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 2242 "program_parse.y"
    {
	   struct asm_symbol *exist = (struct asm_symbol *)
	      _mesa_symbol_table_find_symbol(state->st, 0, (yyvsp[(2) - (4)].string));
	   struct asm_symbol *target = (struct asm_symbol *)
	      _mesa_symbol_table_find_symbol(state->st, 0, (yyvsp[(4) - (4)].string));


	   if (exist != NULL) {
	      yyerror(& (yylsp[(2) - (4)]), state, "redeclared identifier");
	      YYERROR;
	   } else if (target == NULL) {
	      yyerror(& (yylsp[(4) - (4)]), state,
		      "undefined variable binding in ALIAS statement");
	      YYERROR;
	   } else {
	      _mesa_symbol_table_add_symbol(state->st, 0, (yyvsp[(2) - (4)].string), target);
	   }
	;}
    break;



/* Line 1455 of yacc.c  */
#line 5029 "program_parse.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, state, YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (&yylloc, state, yymsg);
	  }
	else
	  {
	    yyerror (&yylloc, state, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, &yylloc, state);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[0] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp, state);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, state, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc, state);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp, state);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 2266 "program_parse.y"


void
asm_instruction_set_operands(struct asm_instruction *inst,
			     const struct prog_dst_register *dst,
			     const struct asm_src_register *src0,
			     const struct asm_src_register *src1,
			     const struct asm_src_register *src2)
{
   /* In the extensions supported by this assembler, the following instructions
    * do not have destination registers:
    *
    *     KIL (GL_ARB_fragment_program and GL_NV_fragment_program_option)
    *     BRA (GL_NV_vertex_program2_option)
    *     CAL (GL_NV_vertex_program2_option)
    *     RET (GL_NV_vertex_program2_option)
    */
   if (dst == NULL) {
      init_dst_reg(& inst->Base.DstReg);
   } else {
      inst->Base.DstReg = *dst;
   }

   /* In the extensions supported by this assembler, the following instructions
    * do not have any source registers:
    *
    *     KIL (GL_NV_fragment_program_option)
    *     BRA (GL_NV_vertex_program2_option)
    *     CAL (GL_NV_vertex_program2_option)
    *     RET (GL_NV_vertex_program2_option)
    */
   if (src0 != NULL) {
      inst->Base.SrcReg[0] = src0->Base;
      inst->SrcReg[0] = *src0;
   } else {
      init_src_reg(& inst->SrcReg[0]);
   }

   if (src1 != NULL) {
      inst->Base.SrcReg[1] = src1->Base;
      inst->SrcReg[1] = *src1;
   } else {
      init_src_reg(& inst->SrcReg[1]);
   }

   if (src2 != NULL) {
      inst->Base.SrcReg[2] = src2->Base;
      inst->SrcReg[2] = *src2;
   } else {
      init_src_reg(& inst->SrcReg[2]);
   }
}


struct asm_instruction *
asm_instruction_ctor(gl_inst_opcode op,
		     const struct prog_dst_register *dst,
		     const struct asm_src_register *src0,
		     const struct asm_src_register *src1,
		     const struct asm_src_register *src2)
{
   struct asm_instruction *inst = CALLOC_STRUCT(asm_instruction);

   if (inst) {
      _mesa_init_instructions(& inst->Base, 1);
      inst->Base.Opcode = op;

      asm_instruction_set_operands(inst, dst, src0, src1, src2);
   }

   return inst;
}


struct asm_instruction *
asm_instruction_copy_ctor(const struct prog_instruction *base,
			  const struct prog_dst_register *dst,
			  const struct asm_src_register *src0,
			  const struct asm_src_register *src1,
			  const struct asm_src_register *src2)
{
   struct asm_instruction *inst = CALLOC_STRUCT(asm_instruction);

   if (inst) {
      _mesa_init_instructions(& inst->Base, 1);
      inst->Base.Opcode = base->Opcode;
      inst->Base.CondUpdate = base->CondUpdate;
      inst->Base.CondDst = base->CondDst;
      inst->Base.SaturateMode = base->SaturateMode;
      inst->Base.Precision = base->Precision;

      asm_instruction_set_operands(inst, dst, src0, src1, src2);
   }

   return inst;
}


void
init_dst_reg(struct prog_dst_register *r)
{
   memset(r, 0, sizeof(*r));
   r->File = PROGRAM_UNDEFINED;
   r->WriteMask = WRITEMASK_XYZW;
   r->CondMask = COND_TR;
   r->CondSwizzle = SWIZZLE_NOOP;
}


void
init_src_reg(struct asm_src_register *r)
{
   memset(r, 0, sizeof(*r));
   r->Base.File = PROGRAM_UNDEFINED;
   r->Base.Swizzle = SWIZZLE_NOOP;
   r->Symbol = NULL;
}


/**
 * Validate the set of inputs used by a program
 *
 * Validates that legal sets of inputs are used by the program.  In this case
 * "used" included both reading the input or binding the input to a name using
 * the \c ATTRIB command.
 *
 * \return
 * \c TRUE if the combination of inputs used is valid, \c FALSE otherwise.
 */
int
validate_inputs(struct YYLTYPE *locp, struct asm_parser_state *state)
{
   const int inputs = state->prog->InputsRead | state->InputsBound;

   if (((inputs & 0x0ffff) & (inputs >> 16)) != 0) {
      yyerror(locp, state, "illegal use of generic attribute and name attribute");
      return 0;
   }

   return 1;
}


struct asm_symbol *
declare_variable(struct asm_parser_state *state, char *name, enum asm_type t,
		 struct YYLTYPE *locp)
{
   struct asm_symbol *s = NULL;
   struct asm_symbol *exist = (struct asm_symbol *)
      _mesa_symbol_table_find_symbol(state->st, 0, name);


   if (exist != NULL) {
      yyerror(locp, state, "redeclared identifier");
   } else {
      s = calloc(1, sizeof(struct asm_symbol));
      s->name = name;
      s->type = t;

      switch (t) {
      case at_temp:
	 if (state->prog->NumTemporaries >= state->limits->MaxTemps) {
	    yyerror(locp, state, "too many temporaries declared");
	    free(s);
	    return NULL;
	 }

	 s->temp_binding = state->prog->NumTemporaries;
	 state->prog->NumTemporaries++;
	 break;

      case at_address:
	 if (state->prog->NumAddressRegs >= state->limits->MaxAddressRegs) {
	    yyerror(locp, state, "too many address registers declared");
	    free(s);
	    return NULL;
	 }

	 /* FINISHME: Add support for multiple address registers.
	  */
	 state->prog->NumAddressRegs++;
	 break;

      default:
	 break;
      }

      _mesa_symbol_table_add_symbol(state->st, 0, s->name, s);
      s->next = state->sym;
      state->sym = s;
   }

   return s;
}


int add_state_reference(struct gl_program_parameter_list *param_list,
			const gl_state_index tokens[STATE_LENGTH])
{
   const GLuint size = 4; /* XXX fix */
   char *name;
   GLint index;

   name = _mesa_program_state_string(tokens);
   index = _mesa_add_parameter(param_list, PROGRAM_STATE_VAR, name,
                               size, GL_NONE, NULL, tokens, 0x0);
   param_list->StateFlags |= _mesa_program_state_flags(tokens);

   /* free name string here since we duplicated it in add_parameter() */
   _mesa_free(name);

   return index;
}


int
initialize_symbol_from_state(struct gl_program *prog,
			     struct asm_symbol *param_var, 
			     const gl_state_index tokens[STATE_LENGTH])
{
   int idx = -1;
   gl_state_index state_tokens[STATE_LENGTH];


   memcpy(state_tokens, tokens, sizeof(state_tokens));

   param_var->type = at_param;
   param_var->param_binding_type = PROGRAM_STATE_VAR;

   /* If we are adding a STATE_MATRIX that has multiple rows, we need to
    * unroll it and call add_state_reference() for each row
    */
   if ((state_tokens[0] == STATE_MODELVIEW_MATRIX ||
	state_tokens[0] == STATE_PROJECTION_MATRIX ||
	state_tokens[0] == STATE_MVP_MATRIX ||
	state_tokens[0] == STATE_TEXTURE_MATRIX ||
	state_tokens[0] == STATE_PROGRAM_MATRIX)
       && (state_tokens[2] != state_tokens[3])) {
      int row;
      const int first_row = state_tokens[2];
      const int last_row = state_tokens[3];

      for (row = first_row; row <= last_row; row++) {
	 state_tokens[2] = state_tokens[3] = row;

	 idx = add_state_reference(prog->Parameters, state_tokens);
	 if (param_var->param_binding_begin == ~0U)
	    param_var->param_binding_begin = idx;
	 param_var->param_binding_length++;
      }
   }
   else {
      idx = add_state_reference(prog->Parameters, state_tokens);
      if (param_var->param_binding_begin == ~0U)
	 param_var->param_binding_begin = idx;
      param_var->param_binding_length++;
   }

   return idx;
}


int
initialize_symbol_from_param(struct gl_program *prog,
			     struct asm_symbol *param_var, 
			     const gl_state_index tokens[STATE_LENGTH])
{
   int idx = -1;
   gl_state_index state_tokens[STATE_LENGTH];


   memcpy(state_tokens, tokens, sizeof(state_tokens));

   assert((state_tokens[0] == STATE_VERTEX_PROGRAM)
	  || (state_tokens[0] == STATE_FRAGMENT_PROGRAM));
   assert((state_tokens[1] == STATE_ENV)
	  || (state_tokens[1] == STATE_LOCAL));

   param_var->type = at_param;
   param_var->param_binding_type = (state_tokens[1] == STATE_ENV)
     ? PROGRAM_ENV_PARAM : PROGRAM_LOCAL_PARAM;

   /* If we are adding a STATE_ENV or STATE_LOCAL that has multiple elements,
    * we need to unroll it and call add_state_reference() for each row
    */
   if (state_tokens[2] != state_tokens[3]) {
      int row;
      const int first_row = state_tokens[2];
      const int last_row = state_tokens[3];

      for (row = first_row; row <= last_row; row++) {
	 state_tokens[2] = state_tokens[3] = row;

	 idx = add_state_reference(prog->Parameters, state_tokens);
	 if (param_var->param_binding_begin == ~0U)
	    param_var->param_binding_begin = idx;
	 param_var->param_binding_length++;
      }
   }
   else {
      idx = add_state_reference(prog->Parameters, state_tokens);
      if (param_var->param_binding_begin == ~0U)
	 param_var->param_binding_begin = idx;
      param_var->param_binding_length++;
   }

   return idx;
}


int
initialize_symbol_from_const(struct gl_program *prog,
			     struct asm_symbol *param_var, 
			     const struct asm_vector *vec)
{
   const int idx = _mesa_add_parameter(prog->Parameters, PROGRAM_CONSTANT,
				       NULL, vec->count, GL_NONE, vec->data,
				       NULL, 0x0);

   param_var->type = at_param;
   param_var->param_binding_type = PROGRAM_CONSTANT;

   if (param_var->param_binding_begin == ~0U)
      param_var->param_binding_begin = idx;
   param_var->param_binding_length++;

   return idx;
}


char *
make_error_string(const char *fmt, ...)
{
   int length;
   char *str;
   va_list args;

   va_start(args, fmt);

   /* Call vsnprintf once to determine how large the final string is.  Call it
    * again to do the actual formatting.  from the vsnprintf manual page:
    *
    *    Upon successful return, these functions return the number of
    *    characters printed  (not including the trailing '\0' used to end
    *    output to strings).
    */
   length = 1 + vsnprintf(NULL, 0, fmt, args);

   str = _mesa_malloc(length);
   if (str) {
      vsnprintf(str, length, fmt, args);
   }

   va_end(args);

   return str;
}


void
yyerror(YYLTYPE *locp, struct asm_parser_state *state, const char *s)
{
   char *err_str;


   err_str = make_error_string("glProgramStringARB(%s)\n", s);
   if (err_str) {
      _mesa_error(state->ctx, GL_INVALID_OPERATION, err_str);
      _mesa_free(err_str);
   }

   err_str = make_error_string("line %u, char %u: error: %s\n",
			       locp->first_line, locp->first_column, s);
   _mesa_set_program_error(state->ctx, locp->position, err_str);

   if (err_str) {
      _mesa_free(err_str);
   }
}


GLboolean
_mesa_parse_arb_program(GLcontext *ctx, GLenum target, const GLubyte *str,
			GLsizei len, struct asm_parser_state *state)
{
   struct asm_instruction *inst;
   unsigned i;
   GLubyte *strz;
   GLboolean result = GL_FALSE;
   void *temp;
   struct asm_symbol *sym;
   unsigned main_offset;

   state->ctx = ctx;
   state->prog->Target = target;
   state->prog->Parameters = _mesa_new_parameter_list();

   /* Make a copy of the program string and force it to be NUL-terminated.
    */
   strz = (GLubyte *) _mesa_malloc(len + 1);
   if (strz == NULL) {
      _mesa_error(ctx, GL_OUT_OF_MEMORY, "glProgramStringARB");
      return GL_FALSE;
   }
   _mesa_memcpy (strz, str, len);
   strz[len] = '\0';

   state->prog->String = strz;

   state->st = _mesa_symbol_table_ctor();

   state->limits = (target == GL_VERTEX_PROGRAM_ARB)
      ? & ctx->Const.VertexProgram
      : & ctx->Const.FragmentProgram;

   state->MaxTextureImageUnits = ctx->Const.MaxTextureImageUnits;
   state->MaxTextureCoordUnits = ctx->Const.MaxTextureCoordUnits;
   state->MaxTextureUnits = ctx->Const.MaxTextureUnits;
   state->MaxClipPlanes = ctx->Const.MaxClipPlanes;
   state->MaxLights = ctx->Const.MaxLights;
   state->MaxProgramMatrices = ctx->Const.MaxProgramMatrices;

   state->state_param_enum = (target == GL_VERTEX_PROGRAM_ARB)
      ? STATE_VERTEX_PROGRAM : STATE_FRAGMENT_PROGRAM;

   _mesa_set_program_error(ctx, -1, NULL);

   _mesa_program_lexer_ctor(& state->scanner, state, (const char *) str, len);
   yyparse(state);
   _mesa_program_lexer_dtor(state->scanner);


   if (ctx->Program.ErrorPos != -1) {
      goto error;
   }

   if (! _mesa_layout_parameters(state)) {
      struct YYLTYPE loc;

      loc.first_line = 0;
      loc.first_column = 0;
      loc.position = len;

      yyerror(& loc, state, "invalid PARAM usage");
      goto error;
   }


   /* Search the instruction stream for BRA and CAL opcodes.  Attempt to
    * resolve the branch targets.
    */
   for (inst = state->inst_head; inst != NULL; inst = inst->next) {
      if ((inst->Base.Opcode == OPCODE_CAL)
	  || (inst->Base.Opcode == OPCODE_BRA)) {
	 const char *const label = inst->Base.Data;
	 unsigned offset;

	 offset = (unsigned) (intptr_t)
	    _mesa_symbol_table_find_symbol(state->st, 1, label);
	 if (offset == 0) {
	    yyerror(& inst->position, state, "invalid branch target");
	    goto error;
	 } else {
	    inst->Base.BranchTarget = offset - 1;
	 }
      }
   }

   /* If either there is no "main" label (offset of zero) or the "main" label
    * is at the first instruction of the shader, do nothing.  Otherwise
    * prepend an unconditional branch to the main label.  This also means that
    * all branch targets need to be incremented by one instruction.
    */
   main_offset = (unsigned) (intptr_t)
      _mesa_symbol_table_find_symbol(state->st, 1, "main");
   if (main_offset > 1) {
      struct asm_instruction *bra_to_main =
	 asm_instruction_ctor(OPCODE_BRA, NULL, NULL, NULL, NULL);

      /* The -1 handles the biasing on the stored offset, and the +1 handles
       * the offset caused by inserting a new instruction.
       */
      bra_to_main->Base.BranchTarget = main_offset - 1 + 1;

      /* Insert the instruction in the list.
       */
      bra_to_main->next = state->inst_head;
      state->inst_head = bra_to_main;
      state->prog->NumInstructions++;

      /* Bump the offsets of all other branch instructions.
       */
      for (inst = bra_to_main->next; inst != NULL; inst = inst->next) {
	 if ((inst->Base.Opcode == OPCODE_CAL)
	     || (inst->Base.Opcode == OPCODE_BRA)) {
	    inst->Base.BranchTarget++;
	 }
      }
   }


   /* Add one instruction to store the "END" instruction.
    */
   state->prog->Instructions =
      _mesa_alloc_instructions(state->prog->NumInstructions + 1);
   inst = state->inst_head;
   for (i = 0; i < state->prog->NumInstructions; i++) {
      struct asm_instruction *const temp = inst->next;

      state->prog->Instructions[i] = inst->Base;
      inst = temp;
   }

   /* Finally, tag on an OPCODE_END instruction */
   {
      const GLuint numInst = state->prog->NumInstructions;
      _mesa_init_instructions(state->prog->Instructions + numInst, 1);
      state->prog->Instructions[numInst].Opcode = OPCODE_END;
   }
   state->prog->NumInstructions++;

   state->prog->NumParameters = state->prog->Parameters->NumParameters;
   state->prog->NumAttributes = _mesa_bitcount(state->prog->InputsRead);

   /*
    * Initialize native counts to logical counts.  The device driver may
    * change them if program is translated into a hardware program.
    */
   state->prog->NumNativeInstructions = state->prog->NumInstructions;
   state->prog->NumNativeTemporaries = state->prog->NumTemporaries;
   state->prog->NumNativeParameters = state->prog->NumParameters;
   state->prog->NumNativeAttributes = state->prog->NumAttributes;
   state->prog->NumNativeAddressRegs = state->prog->NumAddressRegs;

   result = GL_TRUE;

error:
   for (inst = state->inst_head; inst != NULL; inst = temp) {
      temp = inst->next;
      _mesa_free(inst);
   }

   state->inst_head = NULL;
   state->inst_tail = NULL;

   for (sym = state->sym; sym != NULL; sym = temp) {
      temp = sym->next;

      _mesa_free((void *) sym->name);
      _mesa_free(sym);
   }
   state->sym = NULL;

   _mesa_symbol_table_dtor(state->st);
   state->st = NULL;

   return result;
}

