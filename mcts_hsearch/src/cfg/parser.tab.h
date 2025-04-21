/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SRC_CFG_PARSER_TAB_H_INCLUDED
# define YY_YY_SRC_CFG_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INTNUMBER = 258,               /* INTNUMBER  */
    REALNUMBER = 259,              /* REALNUMBER  */
    ARG_VALUE = 260,               /* ARG_VALUE  */
    SEPARATOR = 261,               /* SEPARATOR  */
    N = 262,                       /* N  */
    K = 263,                       /* K  */
    MAXWEIGHT = 264,               /* MAXWEIGHT  */
    EXPLORE_COEFF = 265,           /* EXPLORE_COEFF  */
    DEPTH_COEFF = 266,             /* DEPTH_COEFF  */
    NUM_SEARCHES = 267,            /* NUM_SEARCHES  */
    NUM_GAMES = 268,               /* NUM_GAMES  */
    EXPERIMENT_RUNS = 269,         /* EXPERIMENT_RUNS  */
    ENABLE_SHARED_MEMORY = 270,    /* ENABLE_SHARED_MEMORY  */
    ENABLE_VERBATIM = 271,         /* ENABLE_VERBATIM  */
    NUM_THREADS = 272,             /* NUM_THREADS  */
    ENABLE_ACTION_SPACE_SPLITTING = 273, /* ENABLE_ACTION_SPACE_SPLITTING  */
    SPLITTING_COL = 274,           /* SPLITTING_COL  */
    LEFT_WEIGHT = 275,             /* LEFT_WEIGHT  */
    RIGHT_WEIGHT = 276,            /* RIGHT_WEIGHT  */
    HIGHSCORE = 277,               /* HIGHSCORE  */
    HEATING_RATE = 278,            /* HEATING_RATE  */
    HEATING_COLUMN_LIMIT = 279     /* HEATING_COLUMN_LIMIT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 20 "src/cfg/parser.y"

  int ival;     /* used for passing int values from lexer to parser */
  double dval;  /* used for passing double values from lexer to parser */

#line 93 "src/cfg/parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_SRC_CFG_PARSER_TAB_H_INCLUDED  */
