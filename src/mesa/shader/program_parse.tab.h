
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

/* Line 1676 of yacc.c  */
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



/* Line 1676 of yacc.c  */
#line 193 "program_parse.tab.h"
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



