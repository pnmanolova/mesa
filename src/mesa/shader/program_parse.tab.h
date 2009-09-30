
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

/* Line 1676 of yacc.c  */
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



/* Line 1676 of yacc.c  */
#line 190 "program_parse.tab.h"
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



