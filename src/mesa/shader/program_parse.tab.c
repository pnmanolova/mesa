
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

#define SWIZZLE_XXXX MAKE_SWIZZLE4(SWIZZLE_X, SWIZZLE_X, SWIZZLE_X, SWIZZLE_X)
#define SWIZZLE_YYYY MAKE_SWIZZLE4(SWIZZLE_Y, SWIZZLE_Y, SWIZZLE_Y, SWIZZLE_Y)
#define SWIZZLE_ZZZZ MAKE_SWIZZLE4(SWIZZLE_Z, SWIZZLE_Z, SWIZZLE_Z, SWIZZLE_Z)
#define SWIZZLE_WWWW MAKE_SWIZZLE4(SWIZZLE_W, SWIZZLE_W, SWIZZLE_W, SWIZZLE_W)


/* Line 189 of yacc.c  */
#line 188 "program_parse.tab.c"

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
     KIL = 274,
     SWZ = 275,
     TXD_OP = 276,
     BRA_OP = 277,
     FLOW_OP = 278,
     ARL_OP = 279,
     ARL_NV_OP = 280,
     ARA_OP = 281,
     PUSHA_OP = 282,
     POPA_OP = 283,
     INTEGER = 284,
     REAL = 285,
     AMBIENT = 286,
     ATTENUATION = 287,
     BACK = 288,
     CLIP = 289,
     COLOR = 290,
     DEPTH = 291,
     DIFFUSE = 292,
     DIRECTION = 293,
     EMISSION = 294,
     ENV = 295,
     EYE = 296,
     FOG = 297,
     FOGCOORD = 298,
     FRAGMENT = 299,
     FRONT = 300,
     HALF = 301,
     INVERSE = 302,
     INVTRANS = 303,
     LIGHT = 304,
     LIGHTMODEL = 305,
     LIGHTPROD = 306,
     LOCAL = 307,
     MATERIAL = 308,
     MAT_PROGRAM = 309,
     MATRIX = 310,
     MATRIXINDEX = 311,
     MODELVIEW = 312,
     MVP = 313,
     NORMAL = 314,
     OBJECT = 315,
     PALETTE = 316,
     PARAMS = 317,
     PLANE = 318,
     POINT_TOK = 319,
     POINTSIZE = 320,
     POSITION = 321,
     PRIMARY = 322,
     PROGRAM = 323,
     PROJECTION = 324,
     RANGE = 325,
     RESULT = 326,
     ROW = 327,
     SCENECOLOR = 328,
     SECONDARY = 329,
     SHININESS = 330,
     SIZE_TOK = 331,
     SPECULAR = 332,
     SPOT = 333,
     STATE = 334,
     TEXCOORD = 335,
     TEXENV = 336,
     TEXGEN = 337,
     TEXGEN_Q = 338,
     TEXGEN_R = 339,
     TEXGEN_S = 340,
     TEXGEN_T = 341,
     TEXTURE = 342,
     TRANSPOSE = 343,
     TEXTURE_UNIT = 344,
     TEX_1D = 345,
     TEX_2D = 346,
     TEX_3D = 347,
     TEX_CUBE = 348,
     TEX_RECT = 349,
     TEX_SHADOW1D = 350,
     TEX_SHADOW2D = 351,
     TEX_SHADOWRECT = 352,
     TEX_ARRAY1D = 353,
     TEX_ARRAY2D = 354,
     TEX_ARRAYSHADOW1D = 355,
     TEX_ARRAYSHADOW2D = 356,
     VERTEX = 357,
     VTXATTRIB = 358,
     WEIGHT = 359,
     IDENTIFIER = 360,
     USED_IDENTIFIER = 361,
     MASK4 = 362,
     MASK3 = 363,
     MASK2 = 364,
     MASK1 = 365,
     SWIZZLE = 366,
     DOT_DOT = 367,
     DOT = 368
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 121 "program_parse.y"

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
#line 365 "program_parse.tab.c"
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
#line 269 "program_parse.y"

extern int yylex(YYSTYPE *yylval_param, YYLTYPE *yylloc_param,
    void *yyscanner);


/* Line 264 of yacc.c  */
#line 396 "program_parse.tab.c"

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
#define YYLAST   493

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  127
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  151
/* YYNRULES -- Number of rules.  */
#define YYNRULES  301
/* YYNRULES -- Number of states.  */
#define YYNSTATES  513

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   368

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     122,   123,     2,   120,   116,   121,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   115,   114,
       2,   124,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   118,     2,   119,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   125,   117,   126,     2,     2,     2,     2,
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
     105,   106,   107,   108,   109,   110,   111,   112,   113
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     8,    10,    12,    15,    16,    20,    23,
      24,    27,    30,    33,    35,    37,    39,    41,    43,    45,
      47,    49,    51,    53,    55,    57,    59,    61,    63,    65,
      67,    72,    77,    82,    85,    88,    93,    98,   105,   112,
     121,   130,   133,   136,   149,   152,   155,   157,   159,   161,
     163,   165,   167,   169,   171,   173,   175,   177,   179,   181,
     188,   192,   195,   198,   203,   206,   208,   212,   218,   222,
     226,   228,   230,   238,   241,   243,   245,   247,   249,   254,
     256,   258,   260,   262,   264,   266,   268,   272,   273,   276,
     279,   281,   283,   285,   287,   289,   291,   293,   295,   296,
     298,   300,   302,   304,   305,   309,   313,   314,   317,   320,
     322,   324,   326,   328,   330,   332,   334,   336,   338,   340,
     342,   347,   350,   353,   355,   358,   360,   363,   365,   368,
     373,   378,   380,   381,   385,   387,   389,   392,   394,   397,
     399,   401,   405,   412,   413,   415,   418,   423,   425,   429,
     431,   433,   435,   437,   439,   441,   443,   445,   447,   449,
     452,   455,   458,   461,   464,   467,   470,   473,   476,   479,
     482,   485,   489,   491,   493,   495,   501,   503,   505,   507,
     510,   512,   514,   517,   519,   522,   529,   531,   535,   537,
     539,   541,   543,   545,   550,   552,   554,   556,   558,   560,
     562,   565,   567,   569,   575,   577,   580,   582,   584,   590,
     593,   594,   601,   605,   606,   608,   610,   612,   614,   616,
     619,   621,   623,   626,   631,   636,   637,   641,   643,   645,
     647,   650,   652,   654,   656,   658,   664,   666,   670,   676,
     682,   684,   688,   694,   696,   698,   700,   702,   704,   706,
     708,   710,   712,   716,   722,   730,   740,   743,   746,   748,
     750,   751,   752,   757,   759,   760,   761,   765,   769,   771,
     777,   780,   783,   786,   789,   793,   796,   802,   806,   807,
     809,   811,   812,   814,   816,   817,   819,   821,   822,   824,
     826,   827,   831,   832,   836,   837,   841,   843,   845,   847,
     852,   854
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     128,     0,    -1,   129,   130,   132,    12,    -1,     3,    -1,
       4,    -1,   130,   131,    -1,    -1,     8,   277,   114,    -1,
     132,   133,    -1,    -1,   134,   114,    -1,   185,   114,    -1,
     183,   115,    -1,   135,    -1,   136,    -1,   137,    -1,   139,
      -1,   138,    -1,   140,    -1,   141,    -1,   142,    -1,   143,
      -1,   144,    -1,   145,    -1,   151,    -1,   146,    -1,   147,
      -1,   148,    -1,   152,    -1,   153,    -1,    24,   158,   116,
     154,    -1,    25,   158,   116,   156,    -1,    26,   158,   116,
     159,    -1,    27,   159,    -1,    28,   158,    -1,    18,   157,
     116,   156,    -1,    16,   157,   116,   154,    -1,    14,   157,
     116,   154,   116,   154,    -1,    13,   157,   116,   156,   116,
     156,    -1,    17,   157,   116,   156,   116,   156,   116,   156,
      -1,    15,   157,   116,   156,   116,   149,   116,   150,    -1,
      19,   156,    -1,    19,   179,    -1,    21,   157,   116,   156,
     116,   156,   116,   156,   116,   149,   116,   150,    -1,    89,
     271,    -1,   277,   271,    -1,    90,    -1,    91,    -1,    92,
      -1,    93,    -1,    94,    -1,    95,    -1,    96,    -1,    97,
      -1,    98,    -1,    99,    -1,   100,    -1,   101,    -1,   277,
      -1,    20,   157,   116,   164,   116,   161,    -1,    22,   184,
     178,    -1,    23,   178,    -1,   256,   155,    -1,   256,   117,
     155,   117,    -1,   164,   175,    -1,   253,    -1,   256,   164,
     176,    -1,   256,   117,   164,   176,   117,    -1,   165,   177,
     178,    -1,   173,   177,   178,    -1,   160,    -1,   173,    -1,
     162,   116,   162,   116,   162,   116,   162,    -1,   256,   163,
      -1,    29,    -1,   277,    -1,   106,    -1,   187,    -1,   166,
     118,   167,   119,    -1,   201,    -1,   264,    -1,   106,    -1,
     106,    -1,   168,    -1,   169,    -1,    29,    -1,   173,   174,
     170,    -1,    -1,   120,   171,    -1,   121,   172,    -1,    29,
      -1,    29,    -1,   106,    -1,   110,    -1,   110,    -1,   110,
      -1,   107,    -1,   111,    -1,    -1,   107,    -1,   108,    -1,
     109,    -1,   110,    -1,    -1,   122,   179,   123,    -1,   122,
     180,   123,    -1,    -1,   181,   176,    -1,   182,   176,    -1,
     105,    -1,   106,    -1,   184,    -1,   105,    -1,   106,    -1,
     186,    -1,   193,    -1,   257,    -1,   260,    -1,   263,    -1,
     276,    -1,     7,   105,   124,   187,    -1,   102,   188,    -1,
      44,   192,    -1,    66,    -1,   104,   190,    -1,    59,    -1,
      35,   269,    -1,    43,    -1,    80,   270,    -1,    56,   118,
     191,   119,    -1,   103,   118,   189,   119,    -1,    29,    -1,
      -1,   118,   191,   119,    -1,    29,    -1,    66,    -1,    35,
     269,    -1,    43,    -1,    80,   270,    -1,   194,    -1,   195,
      -1,    10,   105,   197,    -1,    10,   105,   118,   196,   119,
     198,    -1,    -1,    29,    -1,   124,   200,    -1,   124,   125,
     199,   126,    -1,   202,    -1,   199,   116,   202,    -1,   204,
      -1,   240,    -1,   250,    -1,   204,    -1,   240,    -1,   251,
      -1,   203,    -1,   241,    -1,   250,    -1,   204,    -1,    79,
     228,    -1,    79,   205,    -1,    79,   207,    -1,    79,   210,
      -1,    79,   212,    -1,    79,   218,    -1,    79,   214,    -1,
      79,   221,    -1,    79,   223,    -1,    79,   225,    -1,    79,
     227,    -1,    79,   239,    -1,    53,   268,   206,    -1,   216,
      -1,    39,    -1,    75,    -1,    49,   118,   217,   119,   208,
      -1,   216,    -1,    66,    -1,    32,    -1,    78,   209,    -1,
      46,    -1,    38,    -1,    50,   211,    -1,    31,    -1,   268,
      73,    -1,    51,   118,   217,   119,   268,   213,    -1,   216,
      -1,    81,   272,   215,    -1,    35,    -1,    31,    -1,    37,
      -1,    77,    -1,    29,    -1,    82,   270,   219,   220,    -1,
      41,    -1,    60,    -1,    85,    -1,    86,    -1,    84,    -1,
      83,    -1,    42,   222,    -1,    35,    -1,    62,    -1,    34,
     118,   224,   119,    63,    -1,    29,    -1,    64,   226,    -1,
      76,    -1,    32,    -1,   230,    72,   118,   233,   119,    -1,
     230,   229,    -1,    -1,    72,   118,   233,   112,   233,   119,
      -1,    55,   234,   231,    -1,    -1,   232,    -1,    47,    -1,
      88,    -1,    48,    -1,    29,    -1,    57,   235,    -1,    69,
      -1,    58,    -1,    87,   270,    -1,    61,   118,   237,   119,
      -1,    54,   118,   238,   119,    -1,    -1,   118,   236,   119,
      -1,    29,    -1,    29,    -1,    29,    -1,    36,    70,    -1,
     244,    -1,   247,    -1,   242,    -1,   245,    -1,    68,    40,
     118,   243,   119,    -1,   248,    -1,   248,   112,   248,    -1,
      68,    40,   118,   248,   119,    -1,    68,    52,   118,   246,
     119,    -1,   249,    -1,   249,   112,   249,    -1,    68,    52,
     118,   249,   119,    -1,    29,    -1,    29,    -1,   252,    -1,
     254,    -1,   253,    -1,   254,    -1,   255,    -1,    30,    -1,
      29,    -1,   125,   255,   126,    -1,   125,   255,   116,   255,
     126,    -1,   125,   255,   116,   255,   116,   255,   126,    -1,
     125,   255,   116,   255,   116,   255,   116,   255,   126,    -1,
     256,    30,    -1,   256,    29,    -1,   120,    -1,   121,    -1,
      -1,    -1,   259,    11,   258,   262,    -1,   277,    -1,    -1,
      -1,     5,   261,   262,    -1,   262,   116,   105,    -1,   105,
      -1,   259,     9,   105,   124,   264,    -1,    71,    66,    -1,
      71,    43,    -1,    71,   265,    -1,    71,    65,    -1,    71,
      80,   270,    -1,    71,    36,    -1,    71,    34,   118,   224,
     119,    -1,    35,   266,   267,    -1,    -1,    45,    -1,    33,
      -1,    -1,    67,    -1,    74,    -1,    -1,    45,    -1,    33,
      -1,    -1,    67,    -1,    74,    -1,    -1,   118,   273,   119,
      -1,    -1,   118,   274,   119,    -1,    -1,   118,   275,   119,
      -1,    29,    -1,    29,    -1,    29,    -1,     6,   105,   124,
     106,    -1,   105,    -1,   106,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   276,   276,   279,   287,   299,   300,   303,   325,   326,
     329,   344,   345,   348,   353,   358,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   372,   373,   374,   377,   378,
     387,   391,   397,   403,   407,   419,   425,   431,   438,   444,
     451,   495,   500,   510,   554,   558,   575,   576,   577,   578,
     579,   580,   581,   582,   583,   584,   585,   586,   587,   627,
     639,   656,   666,   674,   691,   698,   717,   728,   748,   773,
     795,   798,   807,   840,   847,   862,   912,   954,   965,   988,
     998,  1004,  1035,  1052,  1052,  1054,  1061,  1071,  1072,  1073,
    1076,  1090,  1104,  1122,  1143,  1145,  1146,  1147,  1148,  1151,
    1151,  1151,  1151,  1152,  1155,  1166,  1178,  1185,  1192,  1199,
    1220,  1241,  1260,  1260,  1263,  1264,  1265,  1266,  1267,  1268,
    1271,  1289,  1293,  1299,  1303,  1307,  1311,  1320,  1329,  1333,
    1338,  1344,  1355,  1355,  1356,  1358,  1362,  1366,  1370,  1376,
    1376,  1378,  1394,  1417,  1420,  1431,  1437,  1443,  1444,  1451,
    1457,  1463,  1471,  1477,  1483,  1491,  1497,  1503,  1511,  1512,
    1515,  1516,  1517,  1518,  1519,  1520,  1521,  1522,  1523,  1524,
    1525,  1528,  1537,  1541,  1545,  1551,  1560,  1564,  1568,  1577,
    1581,  1587,  1593,  1600,  1605,  1613,  1623,  1625,  1633,  1639,
    1643,  1647,  1653,  1664,  1673,  1677,  1682,  1686,  1690,  1694,
    1700,  1707,  1711,  1717,  1725,  1736,  1743,  1747,  1753,  1763,
    1774,  1778,  1796,  1805,  1808,  1814,  1818,  1822,  1828,  1839,
    1844,  1849,  1854,  1859,  1864,  1872,  1875,  1880,  1893,  1901,
    1912,  1920,  1920,  1922,  1922,  1924,  1934,  1939,  1946,  1956,
    1965,  1970,  1977,  1987,  1997,  2009,  2009,  2010,  2010,  2012,
    2022,  2030,  2040,  2048,  2056,  2065,  2076,  2080,  2086,  2087,
    2088,  2091,  2091,  2094,  2129,  2133,  2133,  2136,  2142,  2150,
    2163,  2172,  2181,  2185,  2194,  2203,  2212,  2219,  2226,  2231,
    2240,  2252,  2255,  2264,  2275,  2276,  2277,  2280,  2281,  2282,
    2285,  2286,  2289,  2290,  2293,  2294,  2297,  2308,  2319,  2330,
    2351,  2352
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ARBvp_10", "ARBfp_10", "ADDRESS",
  "ALIAS", "ATTRIB", "OPTION", "OUTPUT", "PARAM", "TEMP", "END", "BIN_OP",
  "BINSC_OP", "SAMPLE_OP", "SCALAR_OP", "TRI_OP", "VECTOR_OP", "KIL",
  "SWZ", "TXD_OP", "BRA_OP", "FLOW_OP", "ARL_OP", "ARL_NV_OP", "ARA_OP",
  "PUSHA_OP", "POPA_OP", "INTEGER", "REAL", "AMBIENT", "ATTENUATION",
  "BACK", "CLIP", "COLOR", "DEPTH", "DIFFUSE", "DIRECTION", "EMISSION",
  "ENV", "EYE", "FOG", "FOGCOORD", "FRAGMENT", "FRONT", "HALF", "INVERSE",
  "INVTRANS", "LIGHT", "LIGHTMODEL", "LIGHTPROD", "LOCAL", "MATERIAL",
  "MAT_PROGRAM", "MATRIX", "MATRIXINDEX", "MODELVIEW", "MVP", "NORMAL",
  "OBJECT", "PALETTE", "PARAMS", "PLANE", "POINT_TOK", "POINTSIZE",
  "POSITION", "PRIMARY", "PROGRAM", "PROJECTION", "RANGE", "RESULT", "ROW",
  "SCENECOLOR", "SECONDARY", "SHININESS", "SIZE_TOK", "SPECULAR", "SPOT",
  "STATE", "TEXCOORD", "TEXENV", "TEXGEN", "TEXGEN_Q", "TEXGEN_R",
  "TEXGEN_S", "TEXGEN_T", "TEXTURE", "TRANSPOSE", "TEXTURE_UNIT", "TEX_1D",
  "TEX_2D", "TEX_3D", "TEX_CUBE", "TEX_RECT", "TEX_SHADOW1D",
  "TEX_SHADOW2D", "TEX_SHADOWRECT", "TEX_ARRAY1D", "TEX_ARRAY2D",
  "TEX_ARRAYSHADOW1D", "TEX_ARRAYSHADOW2D", "VERTEX", "VTXATTRIB",
  "WEIGHT", "IDENTIFIER", "USED_IDENTIFIER", "MASK4", "MASK3", "MASK2",
  "MASK1", "SWIZZLE", "DOT_DOT", "DOT", "';'", "':'", "','", "'|'", "'['",
  "']'", "'+'", "'-'", "'('", "')'", "'='", "'{'", "'}'", "$accept",
  "program", "language", "optionSequence", "option", "statementSequence",
  "statement", "instruction", "ALU_instruction", "TexInstruction",
  "FlowInstruction", "ARL_instruction", "ARA_instruction",
  "ASTACK_instruction", "VECTORop_instruction", "SCALARop_instruction",
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
     365,   366,   367,   368,    59,    58,    44,   124,    91,    93,
      43,    45,    40,    41,    61,   123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   127,   128,   129,   129,   130,   130,   131,   132,   132,
     133,   133,   133,   134,   134,   134,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   136,   136,   136,   137,   137,
     138,   138,   139,   140,   140,   141,   142,   143,   144,   145,
     146,   147,   147,   148,   149,   149,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   151,
     152,   153,   154,   154,   155,   155,   156,   156,   157,   158,
     159,   160,   161,   162,   163,   163,   164,   164,   164,   164,
     165,   165,   166,   167,   167,   168,   169,   170,   170,   170,
     171,   172,   173,   174,   175,   176,   176,   176,   176,   177,
     177,   177,   177,   177,   178,   178,   178,   179,   180,   181,
     182,   183,   184,   184,   185,   185,   185,   185,   185,   185,
     186,   187,   187,   188,   188,   188,   188,   188,   188,   188,
     188,   189,   190,   190,   191,   192,   192,   192,   192,   193,
     193,   194,   195,   196,   196,   197,   198,   199,   199,   200,
     200,   200,   201,   201,   201,   202,   202,   202,   203,   203,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   205,   206,   206,   206,   207,   208,   208,   208,   208,
     208,   209,   210,   211,   211,   212,   213,   214,   215,   216,
     216,   216,   217,   218,   219,   219,   220,   220,   220,   220,
     221,   222,   222,   223,   224,   225,   226,   226,   227,   228,
     229,   229,   230,   231,   231,   232,   232,   232,   233,   234,
     234,   234,   234,   234,   234,   235,   235,   236,   237,   238,
     239,   240,   240,   241,   241,   242,   243,   243,   244,   245,
     246,   246,   247,   248,   249,   250,   250,   251,   251,   252,
     253,   253,   254,   254,   254,   254,   255,   255,   256,   256,
     256,   258,   257,   259,   259,   261,   260,   262,   262,   263,
     264,   264,   264,   264,   264,   264,   264,   265,   266,   266,
     266,   267,   267,   267,   268,   268,   268,   269,   269,   269,
     270,   270,   271,   271,   272,   272,   273,   274,   275,   276,
     277,   277
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     1,     1,     2,     0,     3,     2,     0,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     4,     2,     2,     4,     4,     6,     6,     8,
       8,     2,     2,    12,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     6,
       3,     2,     2,     4,     2,     1,     3,     5,     3,     3,
       1,     1,     7,     2,     1,     1,     1,     1,     4,     1,
       1,     1,     1,     1,     1,     1,     3,     0,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     1,
       1,     1,     1,     0,     3,     3,     0,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     2,     2,     1,     2,     1,     2,     1,     2,     4,
       4,     1,     0,     3,     1,     1,     2,     1,     2,     1,
       1,     3,     6,     0,     1,     2,     4,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     3,     1,     1,     1,     5,     1,     1,     1,     2,
       1,     1,     2,     1,     2,     6,     1,     3,     1,     1,
       1,     1,     1,     4,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     5,     1,     2,     1,     1,     5,     2,
       0,     6,     3,     0,     1,     1,     1,     1,     1,     2,
       1,     1,     2,     4,     4,     0,     3,     1,     1,     1,
       2,     1,     1,     1,     1,     5,     1,     3,     5,     5,
       1,     3,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     5,     7,     9,     2,     2,     1,     1,
       0,     0,     4,     1,     0,     0,     3,     3,     1,     5,
       2,     2,     2,     2,     3,     2,     5,     3,     0,     1,
       1,     0,     1,     1,     0,     1,     1,     0,     1,     1,
       0,     3,     0,     3,     0,     3,     1,     1,     1,     4,
       1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     3,     4,     0,     6,     1,     9,     0,     5,   264,
     300,   301,     0,   265,     0,     0,     0,     2,     0,     0,
       0,     0,     0,     0,   260,     0,     0,     0,   106,     0,
       0,     0,     0,     0,   300,   301,     8,     0,    13,    14,
      15,    17,    16,    18,    19,    20,    21,    22,    23,    25,
      26,    27,    24,    28,    29,     0,   111,     0,   114,   115,
     139,   140,   116,     0,   117,   118,   119,   263,     7,     0,
       0,     0,     0,     0,    81,     0,   103,    80,     0,     0,
       0,     0,     0,   109,   258,   259,    41,    42,    98,     0,
       0,     0,   112,   113,   106,     0,    61,    92,     0,   103,
       0,     0,    33,    70,    71,    34,    10,    12,    11,     0,
     261,   268,   266,     0,     0,   143,   260,   141,     0,   278,
     275,   271,   273,   270,   290,   272,   260,    99,   100,   101,
     102,   106,   260,   260,   260,   260,   260,    96,    95,    97,
     107,   251,   250,     0,     0,     0,     0,    76,     0,   260,
      98,     0,    77,    79,   152,   153,   231,   232,   154,   247,
     248,     0,   260,    60,   110,     0,     0,    98,   260,   106,
     260,     0,     0,     0,     0,   299,   120,   144,     0,   145,
     149,   150,   151,   245,   246,   249,     0,     0,   280,   279,
     281,     0,   274,     0,    68,     0,     0,     0,    36,     0,
      35,   287,   137,   135,   290,   122,     0,     0,     0,     0,
       0,     0,   284,     0,   284,     0,     0,   294,   290,   160,
     161,   162,   163,   165,   164,   166,   167,   168,   169,     0,
     170,   287,   127,     0,   125,   123,   290,     0,   132,   121,
      98,     0,    66,     0,     0,     0,   104,   105,   108,    30,
      69,    31,    32,     0,   262,   267,     0,   257,   256,   204,
       0,   282,   283,   277,   296,     0,   260,   260,     0,    62,
       0,    65,     0,   260,   288,   289,   136,   138,     0,     0,
       0,   230,   201,   202,   200,     0,   183,   286,   285,   182,
       0,     0,     0,     0,   225,   221,     0,   220,   290,   213,
     207,   206,   205,     0,     0,     0,     0,   126,     0,   128,
       0,     0,   124,     0,   260,   252,    85,     0,    83,    84,
       0,   260,   260,   269,     0,   142,   276,   291,    38,    37,
       0,    94,    64,   292,     0,   292,     0,   243,     0,   244,
       0,     0,   192,     0,   184,     0,   189,   190,   173,   174,
     191,   171,   172,     0,     0,   219,     0,   222,   215,   217,
     216,   212,   214,   298,     0,   188,   187,   194,   195,     0,
       0,   134,     0,   131,     0,     0,    67,     0,    78,    93,
      87,    59,     0,     0,     0,   260,    63,     0,    44,     0,
      45,   260,   238,   242,     0,     0,   284,   229,     0,   227,
       0,   228,     0,   295,   199,   198,   196,   197,   193,   218,
       0,   129,   130,   133,   260,   253,     0,     0,    86,   260,
      74,    73,    75,   260,     0,     0,     0,   147,   155,   158,
     156,   233,   234,   157,   297,     0,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    40,    58,
      39,   203,   178,   180,   177,     0,   175,   176,     0,   224,
     226,   223,   208,     0,    90,    88,    91,    89,     0,     0,
       0,     0,   159,   210,   260,   146,   293,   181,   179,   185,
     186,   260,   254,   260,     0,     0,     0,     0,   209,   148,
       0,     0,     0,     0,   236,     0,   240,     0,   255,   260,
       0,   235,     0,   239,     0,     0,    72,    43,   237,   241,
       0,     0,   211
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     6,     8,     9,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,   334,   448,    52,    53,    54,   195,   269,    86,
      75,    98,   102,   103,   381,   382,   421,   270,    76,   151,
     317,   318,   319,   418,   465,   467,    99,   380,   332,   140,
     131,    96,    87,   166,    88,   167,    55,    56,    57,    58,
     152,   239,   374,   312,   372,   205,    59,    60,    61,   178,
     117,   325,   426,   179,   153,   427,   428,   154,   219,   351,
     220,   456,   478,   221,   289,   222,   479,   223,   366,   352,
     343,   224,   369,   408,   225,   284,   226,   260,   227,   302,
     228,   472,   488,   229,   361,   362,   410,   299,   355,   400,
     402,   398,   230,   155,   430,   431,   493,   156,   432,   495,
     157,   338,   340,   433,   158,   183,   159,   160,   185,    89,
      62,   173,    63,    64,    69,   112,    65,    77,   125,   190,
     263,   290,   276,   192,   388,   304,   265,   435,   364,    66,
     335
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -468
static const yytype_int16 yypact[] =
{
     144,  -468,  -468,    32,  -468,  -468,    28,   125,  -468,    51,
    -468,  -468,   -74,  -468,     7,    16,    80,  -468,   -51,   -51,
     -51,   -51,   -51,   -51,    13,   -51,   -51,   132,   117,    94,
      94,    94,    94,    94,    87,    90,  -468,   133,  -468,  -468,
    -468,  -468,  -468,  -468,  -468,  -468,  -468,  -468,  -468,  -468,
    -468,  -468,  -468,  -468,  -468,   150,  -468,   172,  -468,  -468,
    -468,  -468,  -468,    97,  -468,  -468,  -468,  -468,  -468,   182,
     165,   166,   -14,   138,  -468,   175,   126,  -468,   176,   177,
     178,   179,   180,  -468,  -468,  -468,  -468,  -468,   118,    18,
     181,   184,  -468,  -468,   117,   158,  -468,  -468,   185,   126,
     186,   187,  -468,  -468,  -468,  -468,  -468,  -468,  -468,   183,
    -468,  -468,   189,   192,    -9,   277,    59,  -468,   190,     4,
    -468,  -468,  -468,  -468,   193,  -468,   162,  -468,  -468,  -468,
    -468,   117,   162,   162,   162,   162,   162,  -468,  -468,  -468,
    -468,  -468,  -468,   147,   109,   159,    66,   195,    61,   162,
     118,   196,  -468,  -468,  -468,  -468,  -468,  -468,  -468,  -468,
    -468,    61,   162,  -468,  -468,   194,   197,   118,   162,   117,
     162,    94,   198,   182,   202,  -468,  -468,  -468,   199,  -468,
    -468,  -468,  -468,  -468,  -468,  -468,   239,   280,  -468,  -468,
     120,   281,  -468,   200,  -468,   203,    58,   205,  -468,   207,
    -468,   148,  -468,  -468,   193,  -468,   206,   208,   209,   245,
      -2,   210,    83,   211,    78,   163,     6,   212,   193,  -468,
    -468,  -468,  -468,  -468,  -468,  -468,  -468,  -468,  -468,   253,
    -468,   148,  -468,   213,  -468,  -468,   193,   214,   215,  -468,
     118,   -87,  -468,   -17,   218,   219,  -468,  -468,  -468,  -468,
    -468,  -468,  -468,   265,   189,  -468,   216,  -468,  -468,  -468,
     220,  -468,  -468,  -468,  -468,   222,   162,   162,    61,  -468,
     227,   232,    53,   162,  -468,  -468,  -468,  -468,   314,   315,
     280,  -468,  -468,  -468,  -468,   316,  -468,  -468,  -468,  -468,
     273,   316,    76,   229,   230,  -468,   231,  -468,   193,   -34,
    -468,  -468,  -468,   321,   317,   -10,   233,  -468,   324,  -468,
     325,   324,  -468,   238,   162,  -468,  -468,   237,  -468,  -468,
     247,   162,   162,  -468,   235,  -468,  -468,  -468,  -468,  -468,
     241,  -468,  -468,   246,   249,   246,   251,  -468,   244,  -468,
     250,   252,  -468,   254,  -468,   255,  -468,  -468,  -468,  -468,
    -468,  -468,  -468,   341,   343,  -468,   346,  -468,  -468,  -468,
    -468,  -468,  -468,  -468,   258,  -468,  -468,  -468,  -468,   191,
     349,  -468,   260,  -468,   261,   262,  -468,    52,  -468,  -468,
     164,  -468,   267,   -11,   269,    71,  -468,   357,  -468,   161,
    -468,   162,  -468,  -468,   326,    99,    78,  -468,   268,  -468,
     271,  -468,   272,  -468,  -468,  -468,  -468,  -468,  -468,  -468,
     274,  -468,  -468,  -468,   162,  -468,   359,   363,  -468,   162,
    -468,  -468,  -468,   162,   114,   159,    72,  -468,  -468,  -468,
    -468,  -468,  -468,  -468,  -468,   275,  -468,  -468,  -468,  -468,
    -468,  -468,  -468,  -468,  -468,  -468,  -468,  -468,  -468,  -468,
    -468,  -468,  -468,  -468,  -468,   358,  -468,  -468,    15,  -468,
    -468,  -468,  -468,    81,  -468,  -468,  -468,  -468,   279,   282,
     283,   284,  -468,   327,    71,  -468,  -468,  -468,  -468,  -468,
    -468,   162,  -468,   162,    53,   314,   315,   285,  -468,  -468,
     278,   289,   290,   288,   296,   291,   297,   349,  -468,   162,
     161,  -468,   314,  -468,   315,   107,  -468,  -468,  -468,  -468,
     349,   292,  -468
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -468,  -468,  -468,  -468,  -468,  -468,  -468,  -468,  -468,  -468,
    -468,  -468,  -468,  -468,  -468,  -468,  -468,  -468,  -468,  -468,
    -468,  -468,   -84,  -103,  -468,  -468,  -468,  -126,   145,  -111,
     223,   248,   243,  -468,  -468,  -403,  -468,   -63,  -468,  -468,
    -468,  -468,  -468,  -468,  -468,  -468,   -27,  -468,  -468,  -140,
     313,   -50,   320,  -468,  -468,  -468,  -468,   389,  -468,  -468,
     303,  -468,  -468,  -468,   108,  -468,  -468,  -468,  -468,  -468,
    -468,  -468,  -468,  -468,  -468,   -56,  -468,  -113,  -468,  -468,
    -468,  -468,  -468,  -468,  -468,  -468,  -468,  -468,  -468,  -374,
     129,  -468,  -468,  -468,  -468,  -468,  -468,   141,  -468,  -468,
    -468,  -468,  -468,    -3,  -468,  -468,  -467,  -468,  -468,  -468,
    -468,  -468,  -468,   307,  -468,  -468,  -468,  -468,  -468,  -468,
    -468,  -457,  -387,   308,  -468,  -468,  -185,  -112,  -143,  -115,
    -468,  -468,  -468,  -468,  -468,   256,  -468,   173,  -468,  -468,
    -468,  -207,   201,  -195,    92,  -468,  -468,  -468,  -468,  -468,
      -7
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -248
static const yytype_int16 yytable[] =
{
      12,   186,    67,   180,   184,   104,   241,   292,   198,   277,
     242,   271,   316,   358,   359,   193,   468,   196,   420,   196,
      73,   457,   197,   305,   199,   200,   150,   248,   494,   314,
     505,   367,     5,   282,   186,   143,     7,   188,   300,   315,
      68,   309,   249,   511,   163,   508,   346,   141,   142,   189,
     368,   245,   347,   196,   360,    74,    13,    14,    15,   251,
     283,    16,   143,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
     491,   194,   301,   271,   480,   240,   144,   141,   142,    97,
     141,   142,   350,   146,    10,    11,   506,   145,   244,   496,
     313,   231,   143,   357,   115,   143,   109,   346,   110,   232,
     116,   287,    70,   347,   286,   348,   287,   509,    83,   250,
     146,    71,   233,   288,   147,   234,   144,   144,   288,   144,
     346,   452,   235,    84,    85,   148,   347,   145,   145,   424,
     145,   329,   333,   149,   104,   453,   236,     1,     2,   206,
     425,   349,   196,   350,   470,   328,    34,    35,    10,    11,
     146,   207,   336,   146,   147,   454,   471,   147,   414,   237,
     238,   377,   118,   119,   120,   268,   350,   455,   415,    84,
      85,   121,   201,   149,   149,    72,   149,   261,   474,   458,
     202,    84,    85,   208,   262,   209,   149,   481,   475,   186,
      97,   210,  -112,   122,   123,  -113,   383,   482,   211,   212,
     213,   384,   214,   203,   215,   274,   320,   293,   124,   510,
     294,   295,   275,   216,   296,   137,   462,   204,   138,   139,
      10,    11,   297,   127,   128,   129,   130,    92,    93,    95,
     217,   218,    78,    79,    80,    81,    82,   106,    90,    91,
     298,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,    83,   164,   107,    10,    11,   257,   258,
     186,   463,   429,   184,   404,   405,   406,   407,   100,   101,
     450,   105,    84,    85,   416,   417,   108,   111,   172,   113,
     114,   126,   132,   133,   134,   135,   136,   161,   175,   186,
     162,   168,   170,   171,   383,   174,   177,   255,   187,   259,
     264,   191,   469,   -82,   243,   281,   266,   246,   256,   267,
     247,   272,   253,   273,   278,   306,   279,   280,   285,   291,
     303,   308,   310,   311,   321,   322,    73,   331,   490,   326,
     324,   327,  -247,   337,   339,   342,   344,   353,   354,   356,
     363,   370,   365,   371,   373,   376,   378,   379,   386,   186,
     385,   429,   184,   392,   387,   389,   186,   391,   383,   393,
     397,   394,   399,   395,   396,   401,   422,   403,   409,   411,
     412,   413,   449,   419,   383,   423,   434,   459,   464,   451,
     460,   461,   466,   462,   476,   483,   477,   507,   484,   487,
     492,   485,   486,   497,   498,   499,   500,   501,   502,   504,
     503,   512,   169,   330,   252,   165,    94,   176,   489,   375,
     345,   341,   473,   181,   182,     0,   323,   390,     0,   254,
       0,     0,   307,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   449
};

static const yytype_int16 yycheck[] =
{
       7,   116,     9,   116,   116,    32,   149,   214,   134,   204,
     150,   196,    29,    47,    48,   126,   419,   132,    29,   134,
      71,   395,   133,   218,   135,   136,    89,   167,   485,   116,
     497,    41,     0,    35,   149,    44,     8,    33,    32,   126,
     114,   236,   168,   510,    94,   502,    31,    29,    30,    45,
      60,   162,    37,   168,    88,   106,     5,     6,     7,   170,
      62,    10,    44,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
     483,   131,    76,   268,   458,   148,    68,    29,    30,   106,
      29,    30,    77,   102,   105,   106,   499,    79,   161,   486,
     240,    35,    44,   298,   118,    44,     9,    31,    11,    43,
     124,    33,   105,    37,    31,    39,    33,   504,   105,   169,
     102,   105,    56,    45,   106,    59,    68,    68,    45,    68,
      31,    32,    66,   120,   121,   117,    37,    79,    79,    68,
      79,   267,    89,   125,   171,    46,    80,     3,     4,    40,
      79,    75,   267,    77,    40,   266,   105,   106,   105,   106,
     102,    52,   273,   102,   106,    66,    52,   106,   116,   103,
     104,   314,    34,    35,    36,   117,    77,    78,   126,   120,
     121,    43,    35,   125,   125,   105,   125,    67,   116,   396,
      43,   120,   121,    34,    74,    36,   125,   116,   126,   314,
     106,    42,   115,    65,    66,   115,   321,   126,    49,    50,
      51,   322,    53,    66,    55,    67,   243,    54,    80,   112,
      57,    58,    74,    64,    61,   107,   119,    80,   110,   111,
     105,   106,    69,   107,   108,   109,   110,   105,   106,   122,
      81,    82,    19,    20,    21,    22,    23,   114,    25,    26,
      87,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   105,   106,   115,   105,   106,    29,    30,
     385,   414,   385,   385,    83,    84,    85,    86,    30,    31,
     391,    33,   120,   121,   120,   121,   114,   105,   105,   124,
     124,   116,   116,   116,   116,   116,   116,   116,   106,   414,
     116,   116,   116,   116,   419,   116,    29,   105,   118,    29,
      29,   118,   423,   118,   118,    70,   116,   123,   119,   116,
     123,   116,   124,   116,   118,    72,   118,   118,   118,   118,
     118,   118,   118,   118,   116,   116,    71,   110,   481,   119,
     124,   119,   110,    29,    29,    29,    73,   118,   118,   118,
      29,   118,    35,    29,    29,   117,   119,   110,   117,   474,
     125,   474,   474,   119,   118,   116,   481,   116,   483,   119,
      29,   119,    29,   119,   119,    29,   383,   119,    29,   119,
     119,   119,   389,   116,   499,   116,    29,   119,    29,    63,
     119,   119,    29,   119,   119,   116,    38,   500,   116,    72,
     484,   118,   118,   118,   126,   116,   116,   119,   112,   112,
     119,   119,    99,   268,   171,    95,    27,   114,   474,   311,
     291,   280,   425,   116,   116,    -1,   253,   335,    -1,   173,
      -1,    -1,   231,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   500
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     3,     4,   128,   129,     0,   130,     8,   131,   132,
     105,   106,   277,     5,     6,     7,    10,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,   105,   106,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   151,   152,   153,   183,   184,   185,   186,   193,
     194,   195,   257,   259,   260,   263,   276,   277,   114,   261,
     105,   105,   105,    71,   106,   157,   165,   264,   157,   157,
     157,   157,   157,   105,   120,   121,   156,   179,   181,   256,
     157,   157,   105,   106,   184,   122,   178,   106,   158,   173,
     158,   158,   159,   160,   173,   158,   114,   115,   114,     9,
      11,   105,   262,   124,   124,   118,   124,   197,    34,    35,
      36,    43,    65,    66,    80,   265,   116,   107,   108,   109,
     110,   177,   116,   116,   116,   116,   116,   107,   110,   111,
     176,    29,    30,    44,    68,    79,   102,   106,   117,   125,
     164,   166,   187,   201,   204,   240,   244,   247,   251,   253,
     254,   116,   116,   178,   106,   179,   180,   182,   116,   177,
     116,   116,   105,   258,   116,   106,   187,    29,   196,   200,
     204,   240,   250,   252,   254,   255,   256,   118,    33,    45,
     266,   118,   270,   156,   178,   154,   256,   156,   154,   156,
     156,    35,    43,    66,    80,   192,    40,    52,    34,    36,
      42,    49,    50,    51,    53,    55,    64,    81,    82,   205,
     207,   210,   212,   214,   218,   221,   223,   225,   227,   230,
     239,    35,    43,    56,    59,    66,    80,   103,   104,   188,
     164,   255,   176,   118,   164,   156,   123,   123,   176,   154,
     178,   156,   159,   124,   262,   105,   119,    29,    30,    29,
     224,    67,    74,   267,    29,   273,   116,   116,   117,   155,
     164,   253,   116,   116,    67,    74,   269,   270,   118,   118,
     118,    70,    35,    62,   222,   118,    31,    33,    45,   211,
     268,   118,   268,    54,    57,    58,    61,    69,    87,   234,
      32,    76,   226,   118,   272,   270,    72,   269,   118,   270,
     118,   118,   190,   176,   116,   126,    29,   167,   168,   169,
     173,   116,   116,   264,   124,   198,   119,   119,   156,   154,
     155,   110,   175,    89,   149,   277,   156,    29,   248,    29,
     249,   224,    29,   217,    73,   217,    31,    37,    39,    75,
      77,   206,   216,   118,   118,   235,   118,   270,    47,    48,
      88,   231,   232,    29,   275,    35,   215,    41,    60,   219,
     118,    29,   191,    29,   189,   191,   117,   255,   119,   110,
     174,   161,   162,   256,   156,   125,   117,   118,   271,   116,
     271,   116,   119,   119,   119,   119,   119,    29,   238,    29,
     236,    29,   237,   119,    83,    84,    85,    86,   220,    29,
     233,   119,   119,   119,   116,   126,   120,   121,   170,   116,
      29,   163,   277,   116,    68,    79,   199,   202,   203,   204,
     241,   242,   245,   250,    29,   274,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   150,   277,
     156,    63,    32,    46,    66,    78,   208,   216,   268,   119,
     119,   119,   119,   255,    29,   171,    29,   172,   162,   156,
      40,    52,   228,   230,   116,   126,   119,    38,   209,   213,
     216,   116,   126,   116,   116,   118,   118,    72,   229,   202,
     255,   162,   149,   243,   248,   246,   249,   118,   126,   116,
     116,   119,   112,   119,   112,   233,   162,   150,   248,   249,
     112,   233,   119
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
#line 280 "program_parse.y"
    {
	   if (state->prog->Target != GL_VERTEX_PROGRAM_ARB) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid fragment program header");

	   }
	   state->mode = ARB_vertex;
	;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 288 "program_parse.y"
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
#line 304 "program_parse.y"
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
#line 330 "program_parse.y"
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
#line 349 "program_parse.y"
    {
	   (yyval.inst) = (yyvsp[(1) - (1)].inst);
	   state->prog->NumAluInstructions++;
	;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 354 "program_parse.y"
    {
	   (yyval.inst) = (yyvsp[(1) - (1)].inst);
	   state->prog->NumTexInstructions++;
	;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 388 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (4)].temp_inst), & (yyvsp[(2) - (4)].dst_reg), & (yyvsp[(4) - (4)].src_reg), NULL, NULL);
	;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 392 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (4)].temp_inst), & (yyvsp[(2) - (4)].dst_reg), & (yyvsp[(4) - (4)].src_reg), NULL, NULL);
	;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 398 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_ctor(OPCODE_ARA, & (yyvsp[(2) - (4)].dst_reg), & (yyvsp[(4) - (4)].src_reg), NULL, NULL);
	;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 404 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (2)].temp_inst), NULL, & (yyvsp[(2) - (2)].src_reg), NULL, NULL);
	;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 408 "program_parse.y"
    {
	   if ((yyvsp[(2) - (2)].dst_reg).WriteMask != WRITEMASK_XYZW) {
	      yyerror(& (yylsp[(2) - (2)]), state, "POPA only supports .xyzw write mask");
	      YYERROR;
	   }

	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (2)].temp_inst), & (yyvsp[(2) - (2)].dst_reg), NULL, NULL, NULL);
	;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 420 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (4)].temp_inst), & (yyvsp[(2) - (4)].dst_reg), & (yyvsp[(4) - (4)].src_reg), NULL, NULL);
	;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 426 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (4)].temp_inst), & (yyvsp[(2) - (4)].dst_reg), & (yyvsp[(4) - (4)].src_reg), NULL, NULL);
	;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 432 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (6)].temp_inst), & (yyvsp[(2) - (6)].dst_reg), & (yyvsp[(4) - (6)].src_reg), & (yyvsp[(6) - (6)].src_reg), NULL);
	;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 439 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (6)].temp_inst), & (yyvsp[(2) - (6)].dst_reg), & (yyvsp[(4) - (6)].src_reg), & (yyvsp[(6) - (6)].src_reg), NULL);
	;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 446 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (8)].temp_inst), & (yyvsp[(2) - (8)].dst_reg), & (yyvsp[(4) - (8)].src_reg), & (yyvsp[(6) - (8)].src_reg), & (yyvsp[(8) - (8)].src_reg));
	;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 452 "program_parse.y"
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

  case 41:

/* Line 1455 of yacc.c  */
#line 496 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_ctor(OPCODE_KIL, NULL, & (yyvsp[(2) - (2)].src_reg), NULL, NULL);
	   state->fragment.UsesKill = 1;
	;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 501 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_ctor(OPCODE_KIL_NV, NULL, NULL, NULL, NULL);
	   (yyval.inst)->Base.DstReg.CondMask = (yyvsp[(2) - (2)].dst_reg).CondMask;
	   (yyval.inst)->Base.DstReg.CondSwizzle = (yyvsp[(2) - (2)].dst_reg).CondSwizzle;
	   (yyval.inst)->Base.DstReg.CondSrc = (yyvsp[(2) - (2)].dst_reg).CondSrc;
	   state->fragment.UsesKill = 1;
	;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 511 "program_parse.y"
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

  case 44:

/* Line 1455 of yacc.c  */
#line 555 "program_parse.y"
    {
	   (yyval.integer) = (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 559 "program_parse.y"
    {
	   /* The NV_vertex_program3 spec is just plain broken.  While
	    * ARB_fragment_program correctly makes "texture" a reserved word,
	    * NV_vp3 does not.  To work around this, the lexer treats it as
	    * IDENTIFIER or USED_IDENTIFIER, and the parser has to do the
	    * matching.
	    */
	   if (strcmp("texture", (yyvsp[(1) - (2)].string)) == 0) {
	      (yyval.integer) = (yyvsp[(2) - (2)].integer);
	   } else {
	      yyerror(& (yylsp[(1) - (2)]), state, "expected \"texture\", got IDENTIFIER");
	      YYERROR;
	   }
	;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 575 "program_parse.y"
    { (yyval.integer) = TEXTURE_1D_INDEX; ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 576 "program_parse.y"
    { (yyval.integer) = TEXTURE_2D_INDEX; ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 577 "program_parse.y"
    { (yyval.integer) = TEXTURE_3D_INDEX; ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 578 "program_parse.y"
    { (yyval.integer) = TEXTURE_CUBE_INDEX; ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 579 "program_parse.y"
    { (yyval.integer) = TEXTURE_RECT_INDEX; ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 580 "program_parse.y"
    { (yyval.integer) = -TEXTURE_1D_INDEX; ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 581 "program_parse.y"
    { (yyval.integer) = -TEXTURE_2D_INDEX; ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 582 "program_parse.y"
    { (yyval.integer) = -TEXTURE_RECT_INDEX; ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 583 "program_parse.y"
    { (yyval.integer) = TEXTURE_1D_ARRAY_INDEX; ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 584 "program_parse.y"
    { (yyval.integer) = TEXTURE_2D_ARRAY_INDEX; ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 585 "program_parse.y"
    { (yyval.integer) = -TEXTURE_1D_ARRAY_INDEX; ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 586 "program_parse.y"
    { (yyval.integer) = -TEXTURE_2D_ARRAY_INDEX; ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 588 "program_parse.y"
    {
	   int valid = 0;

	   /* The NV_vertex_program3 spec is just plain broken.  While
	    * ARB_fragment_program correctly makes SHADOW1D, SHADOW2D, CUBE,
	    * and RECT reserved words, NV_vp3 does not.  To work around this,
	    * the lexer treats these as IDENTIFIER or USED_IDENTIFIER, and the
	    * parser has to do the matching.
	    */
	   if (state->option.NV_vertex3) {
	      if (strncmp("SHADOW", (yyvsp[(1) - (1)].string), 6) == 0) {
		 const char *const subTarget = & (yyvsp[(1) - (1)].string)[6];

		 if (strcmp("1D", subTarget) == 0) {
		    (yyval.integer) = -TEXTURE_1D_INDEX;
		    valid = state->option.Shadow;
		 } else if (strcmp("2D", subTarget) == 0) {
		    (yyval.integer) = -TEXTURE_2D_INDEX;
		    valid = state->option.Shadow;
		 } else if (strcmp("RECT", subTarget) == 0) {
		    (yyval.integer) = -TEXTURE_RECT_INDEX;
		    valid = state->option.Shadow && state->option.TexRect;
		 }
	      } else if (strcmp("CUBE", (yyvsp[(1) - (1)].string)) == 0) {
		 (yyval.integer) = TEXTURE_CUBE_INDEX;
		 valid = 1;
	      } else if (strcmp("RECT", (yyvsp[(1) - (1)].string)) == 0) {
		 (yyval.integer) = TEXTURE_RECT_INDEX;
		 valid = state->option.TexRect;
	      }
	   }

	   if (!valid) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid texture target");
	      YYERROR;
	   }
	;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 628 "program_parse.y"
    {
	   /* FIXME: Is this correct?  Should the extenedSwizzle be applied
	    * FIXME: to the existing swizzle?
	    */
	   (yyvsp[(4) - (6)].src_reg).Base.Swizzle = (yyvsp[(6) - (6)].swiz_mask).swizzle;
	   (yyvsp[(4) - (6)].src_reg).Base.Negate = (yyvsp[(6) - (6)].swiz_mask).mask;

	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (6)].temp_inst), & (yyvsp[(2) - (6)].dst_reg), & (yyvsp[(4) - (6)].src_reg), NULL, NULL);
	;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 640 "program_parse.y"
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

  case 61:

/* Line 1455 of yacc.c  */
#line 657 "program_parse.y"
    {
	   (yyval.inst) = asm_instruction_copy_ctor(& (yyvsp[(1) - (2)].temp_inst), NULL, NULL, NULL, NULL);

	   (yyval.inst)->Base.DstReg.CondMask = (yyvsp[(2) - (2)].dst_reg).CondMask;
	   (yyval.inst)->Base.DstReg.CondSwizzle = (yyvsp[(2) - (2)].dst_reg).CondSwizzle;
	   (yyval.inst)->Base.DstReg.CondSrc = (yyvsp[(2) - (2)].dst_reg).CondSrc;
	;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 667 "program_parse.y"
    {
	   (yyval.src_reg) = (yyvsp[(2) - (2)].src_reg);

	   if ((yyvsp[(1) - (2)].negate)) {
	      (yyval.src_reg).Base.Negate = ~(yyval.src_reg).Base.Negate;
	   }
	;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 675 "program_parse.y"
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

  case 64:

/* Line 1455 of yacc.c  */
#line 692 "program_parse.y"
    {
	   (yyval.src_reg) = (yyvsp[(1) - (2)].src_reg);

	   (yyval.src_reg).Base.Swizzle = _mesa_combine_swizzles((yyval.src_reg).Base.Swizzle,
						    (yyvsp[(2) - (2)].swiz_mask).swizzle);
	;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 699 "program_parse.y"
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

  case 66:

/* Line 1455 of yacc.c  */
#line 718 "program_parse.y"
    {
	   (yyval.src_reg) = (yyvsp[(2) - (3)].src_reg);

	   if ((yyvsp[(1) - (3)].negate)) {
	      (yyval.src_reg).Base.Negate = ~(yyval.src_reg).Base.Negate;
	   }

	   (yyval.src_reg).Base.Swizzle = _mesa_combine_swizzles((yyval.src_reg).Base.Swizzle,
						    (yyvsp[(3) - (3)].swiz_mask).swizzle);
	;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 729 "program_parse.y"
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

  case 68:

/* Line 1455 of yacc.c  */
#line 749 "program_parse.y"
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

  case 69:

/* Line 1455 of yacc.c  */
#line 774 "program_parse.y"
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
	   (yyval.dst_reg).Index = (yyvsp[(1) - (3)].sym)->binding;
	   (yyval.dst_reg).WriteMask = (yyvsp[(2) - (3)].swiz_mask).mask;
	   (yyval.dst_reg).CondMask = (yyvsp[(3) - (3)].dst_reg).CondMask;
	   (yyval.dst_reg).CondSwizzle = (yyvsp[(3) - (3)].dst_reg).CondSwizzle;
	   (yyval.dst_reg).CondSrc = (yyvsp[(3) - (3)].dst_reg).CondSrc;
	;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 799 "program_parse.y"
    {
	   init_src_reg(& (yyval.src_reg));
	   (yyval.src_reg).Base.File = PROGRAM_ADDRESS;
	   (yyval.src_reg).Base.Index = (yyvsp[(1) - (1)].sym)->binding;
	   (yyval.src_reg).Symbol = (yyvsp[(1) - (1)].sym);
	;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 808 "program_parse.y"
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

  case 73:

/* Line 1455 of yacc.c  */
#line 841 "program_parse.y"
    {
	   (yyval.ext_swizzle) = (yyvsp[(2) - (2)].ext_swizzle);
	   (yyval.ext_swizzle).negate = ((yyvsp[(1) - (2)].negate)) ? 1 : 0;
	;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 848 "program_parse.y"
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

  case 75:

/* Line 1455 of yacc.c  */
#line 863 "program_parse.y"
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

  case 76:

/* Line 1455 of yacc.c  */
#line 913 "program_parse.y"
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
	      (yyval.src_reg).Base.Index = s->binding;
	      break;
	   case at_param:
	      (yyval.src_reg).Base.File = s->param_binding_type;
	      (yyval.src_reg).Base.Index = s->param_binding_begin;
	      break;
	   case at_attrib:
	      (yyval.src_reg).Base.File = PROGRAM_INPUT;
	      (yyval.src_reg).Base.Index = s->binding;
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

  case 77:

/* Line 1455 of yacc.c  */
#line 955 "program_parse.y"
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

  case 78:

/* Line 1455 of yacc.c  */
#line 966 "program_parse.y"
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
	      (yyval.src_reg).Base.AddrReg = (yyvsp[(3) - (4)].src_reg).Base.AddrReg;
	      (yyval.src_reg).Base.AddrComponent = (yyvsp[(3) - (4)].src_reg).Base.AddrComponent;
	      (yyval.src_reg).Symbol = (yyvsp[(1) - (4)].sym);
	   } else {
	      (yyval.src_reg).Base.Index = (yyvsp[(1) - (4)].sym)->param_binding_begin + (yyvsp[(3) - (4)].src_reg).Base.Index;
	   }
	;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 989 "program_parse.y"
    {
	   init_src_reg(& (yyval.src_reg));
	   (yyval.src_reg).Base.File = ((yyvsp[(1) - (1)].temp_sym).name != NULL) 
	      ? (yyvsp[(1) - (1)].temp_sym).param_binding_type
	      : PROGRAM_CONSTANT;
	   (yyval.src_reg).Base.Index = (yyvsp[(1) - (1)].temp_sym).param_binding_begin;
	;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 999 "program_parse.y"
    {
	   init_dst_reg(& (yyval.dst_reg));
	   (yyval.dst_reg).File = PROGRAM_OUTPUT;
	   (yyval.dst_reg).Index = (yyvsp[(1) - (1)].result);
	;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 1005 "program_parse.y"
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
	      (yyval.dst_reg).Index = s->binding;
	      break;
	   case at_output:
	      (yyval.dst_reg).File = PROGRAM_OUTPUT;
	      (yyval.dst_reg).Index = s->binding;
	      break;
	   default:
	      (yyval.dst_reg).File = s->param_binding_type;
	      (yyval.dst_reg).Index = s->param_binding_begin;
	      break;
	   }
	;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 1036 "program_parse.y"
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

  case 85:

/* Line 1455 of yacc.c  */
#line 1055 "program_parse.y"
    {
	   init_src_reg(& (yyval.src_reg));
	   (yyval.src_reg).Base.Index = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 1062 "program_parse.y"
    {
	   init_src_reg(& (yyval.src_reg));
	   (yyval.src_reg).Base.RelAddr = 1;
	   (yyval.src_reg).Base.Index = (yyvsp[(3) - (3)].integer);
	   (yyval.src_reg).Base.AddrReg = (yyvsp[(1) - (3)].sym)->binding;
	   (yyval.src_reg).Base.AddrComponent = (yyvsp[(2) - (3)].integer);
	;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 1071 "program_parse.y"
    { (yyval.integer) = 0; ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 1072 "program_parse.y"
    { (yyval.integer) = (yyvsp[(2) - (2)].integer); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 1073 "program_parse.y"
    { (yyval.integer) = -(yyvsp[(2) - (2)].integer); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 1077 "program_parse.y"
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

  case 91:

/* Line 1455 of yacc.c  */
#line 1091 "program_parse.y"
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

  case 92:

/* Line 1455 of yacc.c  */
#line 1105 "program_parse.y"
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

  case 93:

/* Line 1455 of yacc.c  */
#line 1123 "program_parse.y"
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

  case 98:

/* Line 1455 of yacc.c  */
#line 1148 "program_parse.y"
    { (yyval.swiz_mask).swizzle = SWIZZLE_NOOP; (yyval.swiz_mask).mask = WRITEMASK_XYZW; ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 1152 "program_parse.y"
    { (yyval.swiz_mask).swizzle = SWIZZLE_NOOP; (yyval.swiz_mask).mask = WRITEMASK_XYZW; ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 1156 "program_parse.y"
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

  case 105:

/* Line 1455 of yacc.c  */
#line 1167 "program_parse.y"
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

  case 106:

/* Line 1455 of yacc.c  */
#line 1178 "program_parse.y"
    {
	   (yyval.dst_reg).CondMask = COND_TR;
	   (yyval.dst_reg).CondSwizzle = SWIZZLE_NOOP;
	   (yyval.dst_reg).CondSrc = 0;
	;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 1186 "program_parse.y"
    {
	   (yyval.dst_reg) = (yyvsp[(1) - (2)].dst_reg);
	   (yyval.dst_reg).CondSwizzle = (yyvsp[(2) - (2)].swiz_mask).swizzle;
	;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 1193 "program_parse.y"
    {
	   (yyval.dst_reg) = (yyvsp[(1) - (2)].dst_reg);
	   (yyval.dst_reg).CondSwizzle = (yyvsp[(2) - (2)].swiz_mask).swizzle;
	;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 1200 "program_parse.y"
    {
	   const int good_cond = _mesa_parse_cc(state, (yyvsp[(1) - (1)].string), & (yyval.dst_reg));
	   if (! good_cond) {
	      char *const err_str =
		 make_error_string("invalid condition code \"%s\"", (yyvsp[(1) - (1)].string));

	      yyerror(& (yylsp[(1) - (1)]), state, (err_str != NULL)
		      ? err_str : "invalid condition code");

	      if (err_str != NULL) {
		 _mesa_free(err_str);
	      }

	      YYERROR;
	   }

	   (yyval.dst_reg).CondSwizzle = SWIZZLE_NOOP;
	;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 1221 "program_parse.y"
    {
	   const int good_cond = _mesa_parse_cc(state, (yyvsp[(1) - (1)].string), & (yyval.dst_reg));
	   if (! good_cond) {
	      char *const err_str =
		 make_error_string("invalid condition code \"%s\"", (yyvsp[(1) - (1)].string));

	      yyerror(& (yylsp[(1) - (1)]), state, (err_str != NULL)
		      ? err_str : "invalid condition code");

	      if (err_str != NULL) {
		 _mesa_free(err_str);
	      }

	      YYERROR;
	   }

	   (yyval.dst_reg).CondSwizzle = SWIZZLE_NOOP;
	;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 1242 "program_parse.y"
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

  case 120:

/* Line 1455 of yacc.c  */
#line 1272 "program_parse.y"
    {
	   struct asm_symbol *const s =
	      declare_variable(state, (yyvsp[(2) - (4)].string), at_attrib, & (yylsp[(2) - (4)]));

	   if (s == NULL) {
	      YYERROR;
	   } else {
	      s->binding = (yyvsp[(4) - (4)].attrib);
	      state->InputsBound |= (1U << s->binding);

	      if (!validate_inputs(& (yylsp[(4) - (4)]), state)) {
		 YYERROR;
	      }
	   }
	;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 1290 "program_parse.y"
    {
	   (yyval.attrib) = (yyvsp[(2) - (2)].attrib);
	;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 1294 "program_parse.y"
    {
	   (yyval.attrib) = (yyvsp[(2) - (2)].attrib);
	;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 1300 "program_parse.y"
    {
	   (yyval.attrib) = VERT_ATTRIB_POS;
	;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 1304 "program_parse.y"
    {
	   (yyval.attrib) = VERT_ATTRIB_WEIGHT;
	;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 1308 "program_parse.y"
    {
	   (yyval.attrib) = VERT_ATTRIB_NORMAL;
	;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 1312 "program_parse.y"
    {
	   if (!state->ctx->Extensions.EXT_secondary_color) {
	      yyerror(& (yylsp[(2) - (2)]), state, "GL_EXT_secondary_color not supported");
	      YYERROR;
	   }

	   (yyval.attrib) = VERT_ATTRIB_COLOR0 + (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 1321 "program_parse.y"
    {
	   if (!state->ctx->Extensions.EXT_fog_coord) {
	      yyerror(& (yylsp[(1) - (1)]), state, "GL_EXT_fog_coord not supported");
	      YYERROR;
	   }

	   (yyval.attrib) = VERT_ATTRIB_FOG;
	;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 1330 "program_parse.y"
    {
	   (yyval.attrib) = VERT_ATTRIB_TEX0 + (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 1334 "program_parse.y"
    {
	   yyerror(& (yylsp[(1) - (4)]), state, "GL_ARB_matrix_palette not supported");
	   YYERROR;
	;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 1339 "program_parse.y"
    {
	   (yyval.attrib) = VERT_ATTRIB_GENERIC0 + (yyvsp[(3) - (4)].integer);
	;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 1345 "program_parse.y"
    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->limits->MaxAttribs) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid vertex attribute reference");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 1359 "program_parse.y"
    {
	   (yyval.attrib) = FRAG_ATTRIB_WPOS;
	;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 1363 "program_parse.y"
    {
	   (yyval.attrib) = FRAG_ATTRIB_COL0 + (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 1367 "program_parse.y"
    {
	   (yyval.attrib) = FRAG_ATTRIB_FOGC;
	;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 1371 "program_parse.y"
    {
	   (yyval.attrib) = FRAG_ATTRIB_TEX0 + (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 1379 "program_parse.y"
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

  case 142:

/* Line 1455 of yacc.c  */
#line 1395 "program_parse.y"
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

  case 143:

/* Line 1455 of yacc.c  */
#line 1417 "program_parse.y"
    {
	   (yyval.integer) = 0;
	;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 1421 "program_parse.y"
    {
	   if (((yyvsp[(1) - (1)].integer) < 1) || ((unsigned) (yyvsp[(1) - (1)].integer) >= state->limits->MaxParameters)) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid parameter array size");
	      YYERROR;
	   } else {
	      (yyval.integer) = (yyvsp[(1) - (1)].integer);
	   }
	;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 1432 "program_parse.y"
    {
	   (yyval.temp_sym) = (yyvsp[(2) - (2)].temp_sym);
	;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 1438 "program_parse.y"
    {
	   (yyval.temp_sym) = (yyvsp[(3) - (4)].temp_sym);
	;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 1445 "program_parse.y"
    {
	   (yyvsp[(1) - (3)].temp_sym).param_binding_length += (yyvsp[(3) - (3)].temp_sym).param_binding_length;
	   (yyval.temp_sym) = (yyvsp[(1) - (3)].temp_sym);
	;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 1452 "program_parse.y"
    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_state(state->prog, & (yyval.temp_sym), (yyvsp[(1) - (1)].state));
	;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 1458 "program_parse.y"
    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_param(state->prog, & (yyval.temp_sym), (yyvsp[(1) - (1)].state));
	;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 1464 "program_parse.y"
    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_const(state->prog, & (yyval.temp_sym), & (yyvsp[(1) - (1)].vector));
	;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 1472 "program_parse.y"
    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_state(state->prog, & (yyval.temp_sym), (yyvsp[(1) - (1)].state));
	;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 1478 "program_parse.y"
    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_param(state->prog, & (yyval.temp_sym), (yyvsp[(1) - (1)].state));
	;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 1484 "program_parse.y"
    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_const(state->prog, & (yyval.temp_sym), & (yyvsp[(1) - (1)].vector));
	;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 1492 "program_parse.y"
    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_state(state->prog, & (yyval.temp_sym), (yyvsp[(1) - (1)].state));
	;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 1498 "program_parse.y"
    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_param(state->prog, & (yyval.temp_sym), (yyvsp[(1) - (1)].state));
	;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 1504 "program_parse.y"
    {
	   memset(& (yyval.temp_sym), 0, sizeof((yyval.temp_sym)));
	   (yyval.temp_sym).param_binding_begin = ~0;
	   initialize_symbol_from_const(state->prog, & (yyval.temp_sym), & (yyvsp[(1) - (1)].vector));
	;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 1511 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(1) - (1)].state), sizeof((yyval.state))); ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1512 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 1515 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 1516 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 1517 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 1518 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1519 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1520 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1521 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1522 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1523 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1524 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1525 "program_parse.y"
    { memcpy((yyval.state), (yyvsp[(2) - (2)].state), sizeof((yyval.state))); ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1529 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_MATERIAL;
	   (yyval.state)[1] = (yyvsp[(2) - (3)].integer);
	   (yyval.state)[2] = (yyvsp[(3) - (3)].integer);
	;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1538 "program_parse.y"
    {
	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1542 "program_parse.y"
    {
	   (yyval.integer) = STATE_EMISSION;
	;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1546 "program_parse.y"
    {
	   (yyval.integer) = STATE_SHININESS;
	;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1552 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_LIGHT;
	   (yyval.state)[1] = (yyvsp[(3) - (5)].integer);
	   (yyval.state)[2] = (yyvsp[(5) - (5)].integer);
	;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1561 "program_parse.y"
    {
	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1565 "program_parse.y"
    {
	   (yyval.integer) = STATE_POSITION;
	;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1569 "program_parse.y"
    {
	   if (!state->ctx->Extensions.EXT_point_parameters) {
	      yyerror(& (yylsp[(1) - (1)]), state, "GL_ARB_point_parameters not supported");
	      YYERROR;
	   }

	   (yyval.integer) = STATE_ATTENUATION;
	;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1578 "program_parse.y"
    {
	   (yyval.integer) = (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1582 "program_parse.y"
    {
	   (yyval.integer) = STATE_HALF_VECTOR;
	;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1588 "program_parse.y"
    {
	   (yyval.integer) = STATE_SPOT_DIRECTION;
	;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1594 "program_parse.y"
    {
	   (yyval.state)[0] = (yyvsp[(2) - (2)].state)[0];
	   (yyval.state)[1] = (yyvsp[(2) - (2)].state)[1];
	;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1601 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_LIGHTMODEL_AMBIENT;
	;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 1606 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_LIGHTMODEL_SCENECOLOR;
	   (yyval.state)[1] = (yyvsp[(1) - (2)].integer);
	;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1614 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_LIGHTPROD;
	   (yyval.state)[1] = (yyvsp[(3) - (6)].integer);
	   (yyval.state)[2] = (yyvsp[(5) - (6)].integer);
	   (yyval.state)[3] = (yyvsp[(6) - (6)].integer);
	;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1626 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = (yyvsp[(3) - (3)].integer);
	   (yyval.state)[1] = (yyvsp[(2) - (3)].integer);
	;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1634 "program_parse.y"
    {
	   (yyval.integer) = STATE_TEXENV_COLOR;
	;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1640 "program_parse.y"
    {
	   (yyval.integer) = STATE_AMBIENT;
	;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1644 "program_parse.y"
    {
	   (yyval.integer) = STATE_DIFFUSE;
	;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1648 "program_parse.y"
    {
	   (yyval.integer) = STATE_SPECULAR;
	;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1654 "program_parse.y"
    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->MaxLights) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid light selector");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1665 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_TEXGEN;
	   (yyval.state)[1] = (yyvsp[(2) - (4)].integer);
	   (yyval.state)[2] = (yyvsp[(3) - (4)].integer) + (yyvsp[(4) - (4)].integer);
	;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1674 "program_parse.y"
    {
	   (yyval.integer) = STATE_TEXGEN_EYE_S;
	;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1678 "program_parse.y"
    {
	   (yyval.integer) = STATE_TEXGEN_OBJECT_S;
	;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1683 "program_parse.y"
    {
	   (yyval.integer) = STATE_TEXGEN_EYE_S - STATE_TEXGEN_EYE_S;
	;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1687 "program_parse.y"
    {
	   (yyval.integer) = STATE_TEXGEN_EYE_T - STATE_TEXGEN_EYE_S;
	;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1691 "program_parse.y"
    {
	   (yyval.integer) = STATE_TEXGEN_EYE_R - STATE_TEXGEN_EYE_S;
	;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1695 "program_parse.y"
    {
	   (yyval.integer) = STATE_TEXGEN_EYE_Q - STATE_TEXGEN_EYE_S;
	;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1701 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1708 "program_parse.y"
    {
	   (yyval.integer) = STATE_FOG_COLOR;
	;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1712 "program_parse.y"
    {
	   (yyval.integer) = STATE_FOG_PARAMS;
	;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1718 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_CLIPPLANE;
	   (yyval.state)[1] = (yyvsp[(3) - (5)].integer);
	;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1726 "program_parse.y"
    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->MaxClipPlanes) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid clip plane selector");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1737 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1744 "program_parse.y"
    {
	   (yyval.integer) = STATE_POINT_SIZE;
	;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1748 "program_parse.y"
    {
	   (yyval.integer) = STATE_POINT_ATTENUATION;
	;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1754 "program_parse.y"
    {
	   (yyval.state)[0] = (yyvsp[(1) - (5)].state)[0];
	   (yyval.state)[1] = (yyvsp[(1) - (5)].state)[1];
	   (yyval.state)[2] = (yyvsp[(4) - (5)].integer);
	   (yyval.state)[3] = (yyvsp[(4) - (5)].integer);
	   (yyval.state)[4] = (yyvsp[(1) - (5)].state)[2];
	;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1764 "program_parse.y"
    {
	   (yyval.state)[0] = (yyvsp[(1) - (2)].state)[0];
	   (yyval.state)[1] = (yyvsp[(1) - (2)].state)[1];
	   (yyval.state)[2] = (yyvsp[(2) - (2)].state)[2];
	   (yyval.state)[3] = (yyvsp[(2) - (2)].state)[3];
	   (yyval.state)[4] = (yyvsp[(1) - (2)].state)[2];
	;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1774 "program_parse.y"
    {
	   (yyval.state)[2] = 0;
	   (yyval.state)[3] = 3;
	;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1779 "program_parse.y"
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

  case 212:

/* Line 1455 of yacc.c  */
#line 1797 "program_parse.y"
    {
	   (yyval.state)[0] = (yyvsp[(2) - (3)].state)[0];
	   (yyval.state)[1] = (yyvsp[(2) - (3)].state)[1];
	   (yyval.state)[2] = (yyvsp[(3) - (3)].integer);
	;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1805 "program_parse.y"
    {
	   (yyval.integer) = 0;
	;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1809 "program_parse.y"
    {
	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1815 "program_parse.y"
    {
	   (yyval.integer) = STATE_MATRIX_INVERSE;
	;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1819 "program_parse.y"
    {
	   (yyval.integer) = STATE_MATRIX_TRANSPOSE;
	;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1823 "program_parse.y"
    {
	   (yyval.integer) = STATE_MATRIX_INVTRANS;
	;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1829 "program_parse.y"
    {
	   if ((yyvsp[(1) - (1)].integer) > 3) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid matrix row reference");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1840 "program_parse.y"
    {
	   (yyval.state)[0] = STATE_MODELVIEW_MATRIX;
	   (yyval.state)[1] = (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1845 "program_parse.y"
    {
	   (yyval.state)[0] = STATE_PROJECTION_MATRIX;
	   (yyval.state)[1] = 0;
	;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1850 "program_parse.y"
    {
	   (yyval.state)[0] = STATE_MVP_MATRIX;
	   (yyval.state)[1] = 0;
	;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1855 "program_parse.y"
    {
	   (yyval.state)[0] = STATE_TEXTURE_MATRIX;
	   (yyval.state)[1] = (yyvsp[(2) - (2)].integer);
	;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1860 "program_parse.y"
    {
	   yyerror(& (yylsp[(1) - (4)]), state, "GL_ARB_matrix_palette not supported");
	   YYERROR;
	;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1865 "program_parse.y"
    {
	   (yyval.state)[0] = STATE_PROGRAM_MATRIX;
	   (yyval.state)[1] = (yyvsp[(3) - (4)].integer);
	;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1872 "program_parse.y"
    {
	   (yyval.integer) = 0;
	;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1876 "program_parse.y"
    {
	   (yyval.integer) = (yyvsp[(2) - (3)].integer);
	;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1881 "program_parse.y"
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

  case 228:

/* Line 1455 of yacc.c  */
#line 1894 "program_parse.y"
    {
	   /* Since GL_ARB_matrix_palette isn't supported, just let any value
	    * through here.  The error will be generated later.
	    */
	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1902 "program_parse.y"
    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->MaxProgramMatrices) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid program matrix selector");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1913 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = STATE_DEPTH_RANGE;
	;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1925 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = state->state_param_enum;
	   (yyval.state)[1] = STATE_ENV;
	   (yyval.state)[2] = (yyvsp[(4) - (5)].state)[0];
	   (yyval.state)[3] = (yyvsp[(4) - (5)].state)[1];
	;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1935 "program_parse.y"
    {
	   (yyval.state)[0] = (yyvsp[(1) - (1)].integer);
	   (yyval.state)[1] = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1940 "program_parse.y"
    {
	   (yyval.state)[0] = (yyvsp[(1) - (3)].integer);
	   (yyval.state)[1] = (yyvsp[(3) - (3)].integer);
	;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1947 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = state->state_param_enum;
	   (yyval.state)[1] = STATE_ENV;
	   (yyval.state)[2] = (yyvsp[(4) - (5)].integer);
	   (yyval.state)[3] = (yyvsp[(4) - (5)].integer);
	;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1957 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = state->state_param_enum;
	   (yyval.state)[1] = STATE_LOCAL;
	   (yyval.state)[2] = (yyvsp[(4) - (5)].state)[0];
	   (yyval.state)[3] = (yyvsp[(4) - (5)].state)[1];
	;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1966 "program_parse.y"
    {
	   (yyval.state)[0] = (yyvsp[(1) - (1)].integer);
	   (yyval.state)[1] = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1971 "program_parse.y"
    {
	   (yyval.state)[0] = (yyvsp[(1) - (3)].integer);
	   (yyval.state)[1] = (yyvsp[(3) - (3)].integer);
	;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1978 "program_parse.y"
    {
	   memset((yyval.state), 0, sizeof((yyval.state)));
	   (yyval.state)[0] = state->state_param_enum;
	   (yyval.state)[1] = STATE_LOCAL;
	   (yyval.state)[2] = (yyvsp[(4) - (5)].integer);
	   (yyval.state)[3] = (yyvsp[(4) - (5)].integer);
	;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1988 "program_parse.y"
    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->limits->MaxEnvParams) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid environment parameter reference");
	      YYERROR;
	   }
	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1998 "program_parse.y"
    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->limits->MaxLocalParams) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid local parameter reference");
	      YYERROR;
	   }
	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 2013 "program_parse.y"
    {
	   (yyval.vector).count = 4;
	   (yyval.vector).data[0] = (yyvsp[(1) - (1)].real);
	   (yyval.vector).data[1] = (yyvsp[(1) - (1)].real);
	   (yyval.vector).data[2] = (yyvsp[(1) - (1)].real);
	   (yyval.vector).data[3] = (yyvsp[(1) - (1)].real);
	;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 2023 "program_parse.y"
    {
	   (yyval.vector).count = 1;
	   (yyval.vector).data[0] = (yyvsp[(1) - (1)].real);
	   (yyval.vector).data[1] = (yyvsp[(1) - (1)].real);
	   (yyval.vector).data[2] = (yyvsp[(1) - (1)].real);
	   (yyval.vector).data[3] = (yyvsp[(1) - (1)].real);
	;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 2031 "program_parse.y"
    {
	   (yyval.vector).count = 1;
	   (yyval.vector).data[0] = (float) (yyvsp[(1) - (1)].integer);
	   (yyval.vector).data[1] = (float) (yyvsp[(1) - (1)].integer);
	   (yyval.vector).data[2] = (float) (yyvsp[(1) - (1)].integer);
	   (yyval.vector).data[3] = (float) (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 2041 "program_parse.y"
    {
	   (yyval.vector).count = 4;
	   (yyval.vector).data[0] = (yyvsp[(2) - (3)].real);
	   (yyval.vector).data[1] = 0.0f;
	   (yyval.vector).data[2] = 0.0f;
	   (yyval.vector).data[3] = 1.0f;
	;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 2049 "program_parse.y"
    {
	   (yyval.vector).count = 4;
	   (yyval.vector).data[0] = (yyvsp[(2) - (5)].real);
	   (yyval.vector).data[1] = (yyvsp[(4) - (5)].real);
	   (yyval.vector).data[2] = 0.0f;
	   (yyval.vector).data[3] = 1.0f;
	;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 2058 "program_parse.y"
    {
	   (yyval.vector).count = 4;
	   (yyval.vector).data[0] = (yyvsp[(2) - (7)].real);
	   (yyval.vector).data[1] = (yyvsp[(4) - (7)].real);
	   (yyval.vector).data[2] = (yyvsp[(6) - (7)].real);
	   (yyval.vector).data[3] = 1.0f;
	;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 2067 "program_parse.y"
    {
	   (yyval.vector).count = 4;
	   (yyval.vector).data[0] = (yyvsp[(2) - (9)].real);
	   (yyval.vector).data[1] = (yyvsp[(4) - (9)].real);
	   (yyval.vector).data[2] = (yyvsp[(6) - (9)].real);
	   (yyval.vector).data[3] = (yyvsp[(8) - (9)].real);
	;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 2077 "program_parse.y"
    {
	   (yyval.real) = ((yyvsp[(1) - (2)].negate)) ? -(yyvsp[(2) - (2)].real) : (yyvsp[(2) - (2)].real);
	;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 2081 "program_parse.y"
    {
	   (yyval.real) = (float)(((yyvsp[(1) - (2)].negate)) ? -(yyvsp[(2) - (2)].integer) : (yyvsp[(2) - (2)].integer));
	;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 2086 "program_parse.y"
    { (yyval.negate) = FALSE; ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 2087 "program_parse.y"
    { (yyval.negate) = TRUE;  ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 2088 "program_parse.y"
    { (yyval.negate) = FALSE; ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 2091 "program_parse.y"
    { (yyval.integer) = (yyvsp[(2) - (2)].integer); ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 2095 "program_parse.y"
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

  case 264:

/* Line 1455 of yacc.c  */
#line 2129 "program_parse.y"
    {
	;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 2133 "program_parse.y"
    { (yyval.integer) = (yyvsp[(1) - (1)].integer); ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 2137 "program_parse.y"
    {
	   if (!declare_variable(state, (yyvsp[(3) - (3)].string), (yyvsp[(0) - (3)].integer), & (yylsp[(3) - (3)]))) {
	      YYERROR;
	   }
	;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 2143 "program_parse.y"
    {
	   if (!declare_variable(state, (yyvsp[(1) - (1)].string), (yyvsp[(0) - (1)].integer), & (yylsp[(1) - (1)]))) {
	      YYERROR;
	   }
	;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 2151 "program_parse.y"
    {
	   struct asm_symbol *const s =
	      declare_variable(state, (yyvsp[(3) - (5)].string), at_output, & (yylsp[(3) - (5)]));

	   if (s == NULL) {
	      YYERROR;
	   } else {
	      s->binding = (yyvsp[(5) - (5)].result);
	   }
	;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 2164 "program_parse.y"
    {
	   if (state->mode == ARB_vertex) {
	      (yyval.result) = VERT_RESULT_HPOS;
	   } else {
	      yyerror(& (yylsp[(2) - (2)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 2173 "program_parse.y"
    {
	   if (state->mode == ARB_vertex) {
	      (yyval.result) = VERT_RESULT_FOGC;
	   } else {
	      yyerror(& (yylsp[(2) - (2)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 2182 "program_parse.y"
    {
	   (yyval.result) = (yyvsp[(2) - (2)].result);
	;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 2186 "program_parse.y"
    {
	   if (state->mode == ARB_vertex) {
	      (yyval.result) = VERT_RESULT_PSIZ;
	   } else {
	      yyerror(& (yylsp[(2) - (2)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 2195 "program_parse.y"
    {
	   if (state->mode == ARB_vertex) {
	      (yyval.result) = VERT_RESULT_TEX0 + (yyvsp[(3) - (3)].integer);
	   } else {
	      yyerror(& (yylsp[(2) - (3)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 2204 "program_parse.y"
    {
	   if (state->mode == ARB_fragment) {
	      (yyval.result) = FRAG_RESULT_DEPTH;
	   } else {
	      yyerror(& (yylsp[(2) - (2)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 2213 "program_parse.y"
    {
	   yyerror(& (yylsp[(2) - (5)]), state, "user clip planes not yet supported");
	   YYERROR;
	;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 2220 "program_parse.y"
    {
	   (yyval.result) = (yyvsp[(2) - (3)].integer) + (yyvsp[(3) - (3)].integer);
	;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 2226 "program_parse.y"
    {
	   (yyval.integer) = (state->mode == ARB_vertex)
	      ? VERT_RESULT_COL0
	      : FRAG_RESULT_COLOR;
	;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 2232 "program_parse.y"
    {
	   if (state->mode == ARB_vertex) {
	      (yyval.integer) = VERT_RESULT_COL0;
	   } else {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 2241 "program_parse.y"
    {
	   if (state->mode == ARB_vertex) {
	      (yyval.integer) = VERT_RESULT_BFC0;
	   } else {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 2252 "program_parse.y"
    {
	   (yyval.integer) = 0; 
	;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 2256 "program_parse.y"
    {
	   if (state->mode == ARB_vertex) {
	      (yyval.integer) = 0;
	   } else {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 2265 "program_parse.y"
    {
	   if (state->mode == ARB_vertex) {
	      (yyval.integer) = 1;
	   } else {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid program result name");
	      YYERROR;
	   }
	;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 2275 "program_parse.y"
    { (yyval.integer) = 0; ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 2276 "program_parse.y"
    { (yyval.integer) = 0; ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 2277 "program_parse.y"
    { (yyval.integer) = 1; ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 2280 "program_parse.y"
    { (yyval.integer) = 0; ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 2281 "program_parse.y"
    { (yyval.integer) = 0; ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 2282 "program_parse.y"
    { (yyval.integer) = 1; ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 2285 "program_parse.y"
    { (yyval.integer) = 0; ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 2286 "program_parse.y"
    { (yyval.integer) = (yyvsp[(2) - (3)].integer); ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 2289 "program_parse.y"
    { (yyval.integer) = 0; ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 2290 "program_parse.y"
    { (yyval.integer) = (yyvsp[(2) - (3)].integer); ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 2293 "program_parse.y"
    { (yyval.integer) = 0; ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 2294 "program_parse.y"
    { (yyval.integer) = (yyvsp[(2) - (3)].integer); ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2298 "program_parse.y"
    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->MaxTextureCoordUnits) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid texture coordinate unit selector");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2309 "program_parse.y"
    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->MaxTextureImageUnits) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid texture image unit selector");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2320 "program_parse.y"
    {
	   if ((unsigned) (yyvsp[(1) - (1)].integer) >= state->MaxTextureUnits) {
	      yyerror(& (yylsp[(1) - (1)]), state, "invalid texture unit selector");
	      YYERROR;
	   }

	   (yyval.integer) = (yyvsp[(1) - (1)].integer);
	;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2331 "program_parse.y"
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
#line 5170 "program_parse.tab.c"
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
#line 2355 "program_parse.y"


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
    *     PUSHA (GL_NV_vertex_program3)
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
    *     POPA (GL_NV_vertex_program3)
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

	 s->binding = state->prog->NumTemporaries;
	 state->prog->NumTemporaries++;
	 break;

      case at_address:
	 if (state->prog->NumAddressRegs >= state->limits->MaxAddressRegs) {
	    yyerror(locp, state, "too many address registers declared");
	    free(s);
	    return NULL;
	 }

	 s->binding = state->prog->NumAddressRegs;
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

