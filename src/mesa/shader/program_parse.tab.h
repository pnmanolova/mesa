
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
     INTEGER = 279,
     REAL = 280,
     AMBIENT = 281,
     ATTENUATION = 282,
     BACK = 283,
     CLIP = 284,
     COLOR = 285,
     DEPTH = 286,
     DIFFUSE = 287,
     DIRECTION = 288,
     EMISSION = 289,
     ENV = 290,
     EYE = 291,
     FOG = 292,
     FOGCOORD = 293,
     FRAGMENT = 294,
     FRONT = 295,
     HALF = 296,
     INVERSE = 297,
     INVTRANS = 298,
     LIGHT = 299,
     LIGHTMODEL = 300,
     LIGHTPROD = 301,
     LOCAL = 302,
     MATERIAL = 303,
     MAT_PROGRAM = 304,
     MATRIX = 305,
     MATRIXINDEX = 306,
     MODELVIEW = 307,
     MVP = 308,
     NORMAL = 309,
     OBJECT = 310,
     PALETTE = 311,
     PARAMS = 312,
     PLANE = 313,
     POINT_TOK = 314,
     POINTSIZE = 315,
     POSITION = 316,
     PRIMARY = 317,
     PROGRAM = 318,
     PROJECTION = 319,
     RANGE = 320,
     RESULT = 321,
     ROW = 322,
     SCENECOLOR = 323,
     SECONDARY = 324,
     SHININESS = 325,
     SIZE_TOK = 326,
     SPECULAR = 327,
     SPOT = 328,
     STATE = 329,
     TEXCOORD = 330,
     TEXENV = 331,
     TEXGEN = 332,
     TEXGEN_Q = 333,
     TEXGEN_R = 334,
     TEXGEN_S = 335,
     TEXGEN_T = 336,
     TEXTURE = 337,
     TRANSPOSE = 338,
     TEXTURE_UNIT = 339,
     TEX_1D = 340,
     TEX_2D = 341,
     TEX_3D = 342,
     TEX_CUBE = 343,
     TEX_RECT = 344,
     TEX_SHADOW1D = 345,
     TEX_SHADOW2D = 346,
     TEX_SHADOWRECT = 347,
     TEX_ARRAY1D = 348,
     TEX_ARRAY2D = 349,
     TEX_ARRAYSHADOW1D = 350,
     TEX_ARRAYSHADOW2D = 351,
     VERTEX = 352,
     VTXATTRIB = 353,
     WEIGHT = 354,
     IDENTIFIER = 355,
     USED_IDENTIFIER = 356,
     MASK4 = 357,
     MASK3 = 358,
     MASK2 = 359,
     MASK1 = 360,
     SWIZZLE = 361,
     DOT_DOT = 362,
     DOT = 363
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
#line 188 "program_parse.tab.h"
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



