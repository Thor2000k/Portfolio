%{
#include <stdio.h>
#include "tree.h"

extern char *yytext;
extern body_t *asm_tree;
extern int lineCount;
extern int yylex();
extern int yyparse();
 
void yyerror() {
   fprintf(stderr, "syntax error in line %d before %s.\n", lineCount, yytext); 
}
%}

// Includes needed in header
%code requires {
#include "tree.h"
}

// Bison declarations

// Types
%union
{
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
}

// Declarations
%token <str> FUNC "func"
%token <str> RETURN "return"  
%token <str> END "end" 
%token <str> TYPE "type" 
%token <str> VAR "var"

// Types
%token <str> INT "int"
%token <str> BOOL "bool"
%token <str> ARRAY_OF "array of" 
%token <str> RECORD_OF "record of"
%token <str> ALLOCATE "allocate"
%token <str> LENGTH "of length"

// Statements
%token <str> WRITE "write"
%token <str> IF "if" 
%token <str> THEN "then" 
%token <str> ELSE "else" 
%token <str> WHILE "while"
%token <str> DO "do"

// literals
%token <str> TRUE "true" 
%token <str> FALSE "false" 
%token <str> NIL "null"

%token <str> ID "identifier"
%token <intconst> NUM

// Unary Operators
%token <str> ABS "|"
%token <str> NEG "!"

// Binary Operators
%token <str> PLUS "+"
%token <str> MINUS "-"
%token <str> MULT "*"
%token <str> DIV "/"
%token <str> EQUAL "=="
%token <str> NEQ "!="
%token <str> LEQ "<="
%token <str> GEQ ">="
%token <str> GE ">"
%token <str> LE "<"
%token <str> OR "||"
%token <str> AND "&&"

// Others
%token <str> LCURL "{"
%token <str> RCURL "}"
%token <str> LPAREN "("
%token <str> RPAREN ")"
%token <str> LKPAREN "["
%token <str> RKPAREN "]"
%token <str> ASSIGNMENT "="
%token <str> DOT "."
%token <str> COMMA ","
%token <str> COLON ":"
%token <str> SEMICOLON ";"

// Precedence
%precedence "then" 
%precedence "else"
%left "||"
%left "&&"
%nonassoc "<" ">" ">=" "<=" "==" "!="
%left "+" "-" 
%left "*" "/"

// Nonterminal types
%type <list> act_list 
%type <list> decl_list 
%type <list> par_list
%type <list> var_list 
%type <list> statement_list 
%type <list> exp_list

%type <function> function
%type <head> head
%type <str> tail
%type <type> type
%type <var_type> var_type
%type <body> body
%type <declaration> declaration
%type <statement> statement
%type <variable> variable
%type <exp> exp
%type <term> term

%start program

%%
program: body
         { asm_tree = $1;}
;

function:        head body tail
                    { $$ = make_function($1, $2, $3); }
                ;
head:            "func" ID "(" par_list ")" ":" type
                    { $$ = make_head($2, $4, $7); } 
                ;
tail:            "end" ID
                    { $$ = $2; }
                ;
type:            ID
                    { $$ = make_type($1, kind_t_identifier); }
                |"int"
                    { $$ = make_type(NULL, kind_t_integer); }
                |"bool"
                    { $$ = make_type(NULL, kind_t_boolean); }
                |"array of" type
                    { $$ = make_type($2, kind_t_array); }
                |"record of" "{" var_list "}"
                    { $$ = make_type($3, kind_t_record); }
                ;
par_list:        var_list
                    { $$ = $1; }
                |/* empty */ 
                    { $$ = NULL; }
                ;
var_list:        var_type "," var_list
                    { $$ = append_list($3, $1); }
                |var_type
                    { $$ = append_list(NULL, $1); }
                ;
var_type:        ID ":" type
                    { $$ = make_var_type($1, $3); }
                ;
body:            decl_list statement_list
                    { $$ = make_body($1, $2); }
                ;
decl_list:       declaration decl_list
                    { $$ = append_list($2, $1); }
                |/* empty */
                    { $$ = NULL; }
                ;
declaration:    "type" ID "=" type ";"
                    { $$ = make_declaration($2, $4, kind_typedef); }
                |function
                    { $$ = make_declaration($1, NULL, kind_function); }
                |"var" var_list ";"
                    { $$ = make_declaration($2, NULL, kind_var_list); }
                ;
statement_list:  statement statement_list
                    { $$ = append_list($2, $1); }
                |statement
                    { $$ = append_list(NULL, $1); }
                ;
statement:       "return" exp ";"
                    { $$ = make_statement($2, NULL, NULL, kind_return); }
                |"write" exp ";"
                    { $$ = make_statement($2, NULL, NULL, kind_write); }
                |"allocate" variable ";"
                    { $$ = make_statement($2, NULL, NULL, kind_allocate_record); }
                |"allocate" variable "of length" exp ";"
                    { $$ = make_statement($2, $4, NULL, kind_allocate_array); }
                |variable "=" exp ";"
                    { $$ = make_statement($1, $3, NULL, kind_assign); }
                |"if" exp "then" statement
                    { $$ = make_statement($2, $4, NULL, kind_if); }
                |"if" exp "then" statement "else" statement
                    { $$ = make_statement($2, $4, $6, kind_if_else); }
                |"while" exp "do" statement
                    { $$ = make_statement($2, $4, NULL, kind_while); }
                |"{" statement_list "}"
                    { $$ = make_statement($2, NULL, NULL, kind_statement_list); }
                ;
variable:        ID
                    { $$ = make_variable($1, NULL, kind_id); }
                |variable "[" exp "]"
                    { $$ = make_variable($1, $3, kind_index); }
                |variable "." ID
                    { $$ = make_variable($1, $3, kind_field); }
                ;
exp:             exp "+" exp
                    { $$ = make_exp($1, $3, kind_plus); }
                |exp "-" exp 
                    { $$ = make_exp($1, $3, kind_minus); }
                |exp "*" exp
                    { $$ = make_exp($1, $3, kind_multiply); }
                |exp "/" exp
                    { $$ = make_exp($1, $3, kind_divide); }
                |exp ">" exp
                    { $$ = make_exp($1, $3, kind_greater); }
                |exp "<" exp
                    { $$ = make_exp($1, $3, kind_less); }
                |exp ">=" exp
                    { $$ = make_exp($1, $3, kind_greater_equal); }
                |exp "<=" exp
                    { $$ = make_exp($1, $3, kind_less_equal); }
                |exp "==" exp
                    { $$ = make_exp($1, $3, kind_equal); }
                |exp "!=" exp
                    { $$ = make_exp($1, $3, kind_not_equal); }
                |exp "&&" exp
                    { $$ = make_exp($1, $3, kind_and); }
                |exp "||" exp
                    { $$ = make_exp($1, $3, kind_or); }
                |term
                    { $$ = make_exp($1, NULL, kind_term); }
                ;
term:            variable
                    { $$ = make_term($1, NULL, kind_variable); }
                |ID "(" act_list ")"
                    { $$ = make_term($1, $3, kind_method); }
                |"(" exp ")"
                    { $$ = make_term($2, NULL, kind_parenthess); }
                |"!" term
                    { $$ = make_term($2, NULL, kind_negate); }
                |"|" exp "|"
                    { $$ = make_term($2, NULL, kind_absolute); }
                |NUM
                    { $$ = make_term(&$1, NULL, kind_integer); }
                |"true"
                    { $$ = make_term(NULL, NULL, kind_true); }
                |"false"
                    { $$ = make_term(NULL, NULL, kind_false); }
                |"null"
                    { $$ = make_term(NULL, NULL, kind_null); }
                ;
act_list:        exp_list
                    { $$ = $1; }
                |/* empty */
                    { $$ = NULL; }
                ;
exp_list:        exp "," exp_list
                    { $$ = append_list($3, $1); }
                |exp
                    { $$ = append_list(NULL, $1); }
                ;

%%

// Additional C code