%{
#include <stdio.h>
#include "tree.h"
#include "array_list.h"

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
%token <str> READ "read"
%token <str> IF "if" 
%token <str> THEN "then" 
%token <str> ELSE "else" 
%token <str> WHILE "while"
%token <str> DO "do"
%token <str> COPY "copy"
%token <str> FOR "for"

// literals
%token <str> TRUE "true" 
%token <str> FALSE "false" 
%token <str> NIL "null"

%token <str> ID "identifier"
%token <intconst> NUM

// Unary Operators
%token <str> ABS "|"
%token <str> NEG "!"
%token <str> SQRT "sqrt"

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
%token <str> MODULO "%"

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
%left "*" "/" "%"

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
%type <statement> statement_sub
%type <statement> statement_line
%type <statement> statement_compl
%type <statement> statement_opt
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
var_list:        var_list "," var_type 
                    { $$ = append_list($1, $3); }
                |var_type
                    { $$ = append_list(NULL, $1); }
                ;
var_type:        ID ":" type
                    { $$ = make_var_type($1, $3); }
                ;
body:            decl_list statement_list
                    { $$ = make_body($1, $2); }
                ;
decl_list:       decl_list declaration 
                    { $$ = append_list($1, $2); }
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
statement_list:  statement_list statement_sub
                    { $$ = append_list($1, $2); }
                |statement_sub
                    { $$ = append_list(NULL, $1); }
                ;
statement_sub:   statement_line ";"
                    { $$ = $1; }
                |statement_compl
                    { $$ = $1; }
                ;
statement_line:  "return" exp
                    { $$ = make_statement(kind_return, 1, $2); }
                |"write" exp
                    { $$ = make_statement(kind_write, 1, $2); }
                |"read" variable
                    { $$ = make_statement(kind_read, 1, $2); }
                |"allocate" variable
                    { $$ = make_statement(kind_allocate_record, 1, $2); }
                |"allocate" variable "of length" exp
                    { $$ = make_statement(kind_allocate_array, 2, $2, $4); }
                |variable "=" exp
                    { $$ = make_statement(kind_assign, 2, $1, $3); }
                |"copy" variable "," variable
                    { $$ = make_statement(kind_copy, 2, $2, $4); }
                |"copy" variable "," exp "," variable "," exp "," exp
                    { $$ = make_statement(kind_copy_from, 5, $2, $4, $6, $8, $10); }
                |variable "+" "+"
                    {  
                        int i = 1; 
                        term_t *term_one = make_term(&i, NULL, kind_integer);
                        term_t *term_var = make_term($1, NULL, kind_variable);
                        exp_t *left = make_exp(term_var, NULL, kind_term);
                        exp_t *right = make_exp(term_one, NULL, kind_term);
                        exp_t *res = make_exp(left, right, kind_plus);
                        $$ = make_statement(kind_assign, 2, $1, res);
                    }
                |variable "-" "-"
                    {
                        int i = 1; 
                        term_t *term_one = make_term(&i, NULL, kind_integer);
                        term_t *term_var = make_term($1, NULL, kind_variable);
                        exp_t *left = make_exp(term_var, NULL, kind_term);
                        exp_t *right = make_exp(term_one, NULL, kind_term);
                        exp_t *res = make_exp(left, right, kind_minus);
                        $$ = make_statement(kind_assign, 2, $1, res);
                    }
                ;
statement_compl: "{" statement_list "}"
                    { $$ = make_statement(kind_statement_list, 1, $2); }
                |"if" exp "then" statement_sub
                    { $$ = make_statement(kind_if, 2, $2, $4); }
                |"if" exp "then" statement_sub "else" statement_sub
                    { $$ = make_statement(kind_if_else, 3, $2, $4, $6); }
                |"while" exp "do" statement_sub
                    { $$ = make_statement(kind_while, 2, $2, $4); }
                |"for" statement_opt ";" exp ";" statement_opt "do" statement_sub
                    {
                        array_list *list_inner = AL_INIT_PTR(2);
                        array_list *list_outer = AL_INIT_PTR(2);

                        al_add(list_inner, &$8);
                        al_add(list_inner, &$6);
                        statement_t *inner_statement = make_statement(kind_statement_list, 1, list_inner);
                        statement_t *while_statement = make_statement(kind_while, 2, $4, inner_statement);

                        al_add(list_outer, &$2);
                        al_add(list_outer, &while_statement);

                        $$ = make_statement(kind_statement_list, 1, list_outer);
                    }
                ;
statement_opt:   statement_line
                    { $$ = $1; }
                |"{" statement_list "}"
                    { $$ = make_statement(kind_statement_list, 1, $2); }
                |/* empty */
                    { $$ = NULL; }
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
                |exp "%" exp
                    { $$ = make_exp($1, $3, kind_modulo); }
                |"-"term
                    { 
                        int i = 0;
                        term_t *term = make_term(&i, NULL, kind_integer);
                        exp_t *left = make_exp(term, NULL, kind_term);
                        exp_t *right = make_exp($2, NULL, kind_term);
                        $$ = make_exp(left, right, kind_minus);
                    }
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
                |"sqrt" "(" exp ")"
                    { $$ = make_term($3, NULL, kind_sqrt); }
                ;
act_list:        exp_list
                    { $$ = $1; }
                |/* empty */
                    { $$ = NULL; }
                ;
exp_list:        exp_list "," exp
                    { $$ = append_list($1, $3); }
                |exp
                    { $$ = append_list(NULL, $1); }
                ;

%%

