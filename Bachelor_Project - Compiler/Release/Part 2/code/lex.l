%{
#include "lex.h"
#include "parser.tab.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

extern int lineCount;
extern int fileno();
extern int yylex();
%}

%option noyywrap nounput noinput
%x COMMENT_STATE

%%
[ \t]+        /* { printf("white space, length %lu\n",yyleng); } */
<COMMENT_STATE>"*)"               { BEGIN(INITIAL); }
<COMMENT_STATE>.                    /* ignore */
<INITIAL,COMMENT_STATE>"\n"       { lineCount ++; }

<INITIAL>[#][^\n]*[\n]            { lineCount ++; }
<INITIAL>"(*"                     { BEGIN(COMMENT_STATE); }

    /* Keywords  */
<INITIAL>"func"                   { KEYWORD(FUNC) }
<INITIAL>"return"                 { KEYWORD(RETURN) }
<INITIAL>"end"                    { KEYWORD(END) }
<INITIAL>"type"                   { KEYWORD(TYPE) }  
<INITIAL>"var"                    { KEYWORD(VAR) }

    /* Types */
<INITIAL>"int"                    { KEYWORD(INT) }
<INITIAL>"bool"                   { KEYWORD(BOOL) }
<INITIAL>"array of"               { KEYWORD(ARRAY_OF) }
<INITIAL>"record of"              { KEYWORD(RECORD_OF) }
<INITIAL>"allocate"               { KEYWORD(ALLOCATE) }
<INITIAL>"of length"              { KEYWORD(LENGTH) }

    /* Statements */
<INITIAL>"write"                  { KEYWORD(WRITE) }
<INITIAL>"if"                     { KEYWORD(IF) }
<INITIAL>"then"                   { KEYWORD(THEN) }
<INITIAL>"else"                   { KEYWORD(ELSE) }
<INITIAL>"while"                  { KEYWORD(WHILE) }
<INITIAL>"do"                     { KEYWORD(DO) }

    /* Literals */
<INITIAL>"true"                   { KEYWORD(TRUE) }
<INITIAL>"false"                  { KEYWORD(FALSE) }
<INITIAL>"null"                   { KEYWORD(NIL) }

<INITIAL>[_a-zA-Z][_a-zA-Z0-9]*   { char *str = NEWARRAY(char, strlen(yytext));
                                     sprintf(str, "%s", yytext);
                                     yylval.str = str;
                                     KEYWORD(ID) }
<INITIAL>0|([1-9][0-9]*)          { yylval.intconst = atoi(yytext);
                                     KEYWORD(NUM) }

    /* Operators */
<INITIAL>"|"                      { KEYWORD(ABS) }
<INITIAL>"!"                      { KEYWORD(NEG) }
<INITIAL>"+"                      { KEYWORD(PLUS) }
<INITIAL>"-"                      { KEYWORD(MINUS) }
<INITIAL>"*"                      { KEYWORD(MULT) }
<INITIAL>"/"                      { KEYWORD(DIV) }
<INITIAL>"=="                     { KEYWORD(EQUAL) }
<INITIAL>"!="                     { KEYWORD(NEQ) }
<INITIAL>"<="                     { KEYWORD(LEQ) }
<INITIAL>">="                     { KEYWORD(GEQ) }
<INITIAL>">"                      { KEYWORD(GE) }
<INITIAL>"<"                      { KEYWORD(LE) }
<INITIAL>"||"                     { KEYWORD(OR) }
<INITIAL>"&&"                     { KEYWORD(AND) }

    /* Others */
<INITIAL>"{"                      { KEYWORD(LCURL) }
<INITIAL>"}"                      { KEYWORD(RCURL) }
<INITIAL>"("                      { KEYWORD(LPAREN) }
<INITIAL>")"                      { KEYWORD(RPAREN) }
<INITIAL>"["                      { KEYWORD(LKPAREN) }
<INITIAL>"]"                      { KEYWORD(RKPAREN) }
<INITIAL>"="                      { KEYWORD(ASSIGNMENT) }
<INITIAL>"."                      { KEYWORD(DOT) }
<INITIAL>","                      { KEYWORD(COMMA) }
<INITIAL>":"                      { KEYWORD(COLON) }
<INITIAL>";"                      { KEYWORD(SEMICOLON) }

    /* <INITIAL>"sqrt"                   { KEYWORD(SQRT) } */

<INITIAL>.                        { fprintf(stderr, 
                                     "Illegal char '%s' found in line %d\n",
                                     yytext, lineCount); }
%%
