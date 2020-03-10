/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_INCLUDE_PARSER_TAB_H_INCLUDED
# define YY_YY_INCLUDE_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 18 "parser.y" /* yacc.c:1909  */

#include "tree.h"

#line 48 "include/parser.tab.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    FUNC = 258,
    RETURN = 259,
    END = 260,
    TYPE = 261,
    VAR = 262,
    INT = 263,
    BOOL = 264,
    ARRAY_OF = 265,
    RECORD_OF = 266,
    ALLOCATE = 267,
    LENGTH = 268,
    WRITE = 269,
    READ = 270,
    IF = 271,
    THEN = 272,
    ELSE = 273,
    WHILE = 274,
    DO = 275,
    COPY = 276,
    FOR = 277,
    TRUE = 278,
    FALSE = 279,
    NIL = 280,
    ID = 281,
    NUM = 282,
    ABS = 283,
    NEG = 284,
    SQRT = 285,
    PLUS = 286,
    MINUS = 287,
    MULT = 288,
    DIV = 289,
    EQUAL = 290,
    NEQ = 291,
    LEQ = 292,
    GEQ = 293,
    GE = 294,
    LE = 295,
    OR = 296,
    AND = 297,
    MODULO = 298,
    LCURL = 299,
    RCURL = 300,
    LPAREN = 301,
    RPAREN = 302,
    LKPAREN = 303,
    RKPAREN = 304,
    ASSIGNMENT = 305,
    DOT = 306,
    COMMA = 307,
    COLON = 308,
    SEMICOLON = 309
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 26 "parser.y" /* yacc.c:1909  */

    int intconst;
    char *str;
    array_list *list;
    function_t *function;
    head_t *head;
    type_t *type;
    var_type_t *var_type;
    body_t *body;
    declaration_t *declaration;
    statement_t *statement;
    variable_t *variable;
    exp_t *exp;
    term_t *term;

#line 131 "include/parser.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INCLUDE_PARSER_TAB_H_INCLUDED  */
