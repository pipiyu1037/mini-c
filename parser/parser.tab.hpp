
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
     INT = 258,
     ID = 259,
     TYPE = 260,
     FLOAT = 261,
     CHAR = 262,
     DPLUS = 263,
     DMINUS = 264,
     LP = 265,
     RP = 266,
     LB = 267,
     RB = 268,
     LC = 269,
     RC = 270,
     SEMI = 271,
     COMMA = 272,
     PLUS = 273,
     MINUS = 274,
     STAR = 275,
     DIV = 276,
     GE = 277,
     GT = 278,
     LE = 279,
     LT = 280,
     NE = 281,
     EQ = 282,
     ASSIGN = 283,
     AND = 284,
     OR = 285,
     NOT = 286,
     IF = 287,
     ELSE = 288,
     WHILE = 289,
     RETURN = 290,
     FOR = 291,
     COLON = 292,
     STRUCT = 293,
     BREAK = 294,
     CONTINUE = 295,
     EXT_DEF_LIST = 296,
     EXT_VAR_DEF = 297,
     FUNC_DEF = 298,
     FUNC_DEC = 299,
     EXT_DEC_LIST = 300,
     PARAM_LIST = 301,
     PARAM_DEC = 302,
     VAR_DEF = 303,
     DEC_LIST = 304,
     DEF_LIST = 305,
     COMP_STM = 306,
     STM_LIST = 307,
     EXP_STMT = 308,
     IF_THEN = 309,
     IF_THEN_ELSE = 310,
     FUNC_CALL = 311,
     ARGS = 312,
     FUNCTION = 313,
     PARAM = 314,
     ARG = 315,
     CALL = 316,
     CALL0 = 317,
     LABEL = 318,
     GOTO = 319,
     JLT = 320,
     JLE = 321,
     JGT = 322,
     JGE = 323,
     JEQ = 324,
     JNE = 325,
     END = 326,
     READARRAY = 327,
     WRITEARRAY = 328,
     UPLUS = 329,
     UMINUS = 330,
     LOWER_THEN_ELSE = 331
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 45 "parser.ypp"

    int         type_int;
	float       type_float;
    char        type_char;
	char        type_id[32];
    
	ProgAST     *program;
	vector <ExtDefAST *> ExtDefList;  	    //外部定义（外部变量、函数）列表
    ExtDefAST   *ExtDef;
    vector <VarDecAST*>  ExtDecList;        //外部、局部变量列表
    TypeAST     *Specifier;
    VarDecAST *VarDec;
    CompStmAST  *CompSt;
	vector <ParamAST *>  ParamList;         //形参列表
	ParamAST *ParamDec;

	vector <StmAST *>   StmList;
	StmAST      *Stmt;
	vector <DefAST *>   DefList;
	DefAST      *Def;
    vector <VarDecAST *> DecList;
    VarDecAST   *Dec;
    ExpAST      *Exp;
	vector <ExpAST *>    Args;       	//实参列表
    ArrExpAST   *ArrExp;       //数组表达式
    VarAST      *ArrVar;



/* Line 1676 of yacc.c  */
#line 158 "parser.tab.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

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

extern YYLTYPE yylloc;

