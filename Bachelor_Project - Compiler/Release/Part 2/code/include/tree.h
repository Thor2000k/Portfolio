#ifndef __H_TREE__
#define __H_TREE__

#include "array_list.h"
#include "memory.h"

#define DEBUG_TREE 0
#define PRINT_CALL(F) if (DEBUG_TREE) fprintf(stderr, "Called: %s\n", #F);
#define STRUCT2(A, B, N) struct { void *A; void *B; } N;

typedef enum _bool 
{
    T_TRUE = 1,
    T_FALSE = 0
} bool_t;

// Structs for nodes
typedef struct _function function_t;
typedef struct _head head_t;
typedef struct _type type_t;
typedef struct _var_type var_type_t;
typedef struct _body body_t;
typedef struct _declaration declaration_t;
typedef struct _statement statement_t;
typedef struct _variable variable_t;
typedef struct _exp exp_t;
typedef struct _term term_t;

// function
struct _function
{
    head_t *head;
    body_t *body;
};

// head
struct _head
{
    char *ID;
    array_list *par_list;
    type_t *return_type;
};

// type
typedef enum _type_kind
{
    kind_t_identifier,
    kind_t_integer,
    kind_t_boolean,
    kind_t_array,
    kind_t_record
} type_kind_t;

struct _type
{
    type_kind_t kind;
    void *reference;
};

// var_type
struct _var_type
{
    char *ID;
    type_t *type;
};

// Root of tree
// body
struct _body
{
    array_list *decl_list;
    array_list *stat_list;
};

// decleration
typedef enum _declaration_kind
{   
    kind_typedef,
    kind_function,
    kind_var_list
} declaration_kind_t;

typedef union _declaration_value
{
    STRUCT2(ID, type, assign_type)
    function_t *function;
    array_list *var_list;
} declaration_value_t;

struct _declaration
{
    int lineno;
    declaration_kind_t kind;
    declaration_value_t value;
};

// statement
typedef enum _statement_kind
{
    kind_return,
    kind_write,
    kind_allocate_record,
    kind_allocate_array,
    kind_assign,
    kind_if,
    kind_if_else,
    kind_while,
    kind_statement_list
} statement_kind_t;

typedef union _statement_value
{
    exp_t *expression;
    variable_t *variable;
    STRUCT2(variable, expression, allocate_array)
    STRUCT2(variable, expression, assign)
    STRUCT2(expression, statement, if_statement)
    struct 
    {
        exp_t *expression;
        statement_t *if_statement;
        statement_t *else_statement;
    } if_else;
    STRUCT2(expression, statement, while_statement)
    array_list *statement_list;
} statement_value_t;

struct _statement
{
    int lineno; 
    statement_kind_t kind;
    statement_value_t value;
};

// variable
typedef enum _variable_kind
{
    kind_id,
    kind_index,
    kind_field
} variable_kind_t;

typedef union
{
    char *ID;
    STRUCT2(variable, exp, get_index)
    STRUCT2(variable, ID, field)
} variable_value_t;

struct _variable
{
    int lineno; 
    variable_kind_t kind;
    variable_value_t value;
};

// expression
typedef enum _exp_kind
{
    kind_plus,
    kind_minus,
    kind_multiply,
    kind_divide,
    kind_greater,
    kind_less,
    kind_greater_equal,
    kind_less_equal,
    kind_equal,
    kind_not_equal,
    kind_and,
    kind_or,
    kind_term
} exp_kind_t;

typedef union _exp_value
{
    STRUCT2(left, right, bin_exp)
    term_t *term;
} exp_value_t;

struct _exp
{
    int lineno;
    exp_kind_t kind;
    exp_value_t value;
};

// term
typedef enum _term_kind 
{
    kind_variable,
    kind_method,
    kind_parenthess,
    kind_negate,
    kind_absolute,
    kind_integer,
    kind_true,
    kind_false,
    kind_null,
} term_kind_t;

typedef union _term_value
{
    STRUCT2(ID, actions, method)
    void *ptr;
    int num;
    bool_t boolean;
} term_value_t;

struct _term
{
    int lineno;
    term_kind_t kind;
    term_value_t value;
};

// Constructors
function_t *make_function(head_t *head, body_t *body, char *ID);
head_t *make_head(char *ID, array_list *params, type_t *type);
type_t *make_type(void *reference, type_kind_t kind);
var_type_t *make_var_type(char *ID, type_t *type);
body_t *make_body(array_list *decl_list, array_list *statement_list);
declaration_t *make_declaration(void *param1, void *param2, 
    declaration_kind_t kind);
statement_t *make_statement(void *param1, void *param2, 
    void *param3, statement_kind_t kind);
declaration_t *make_declaration(void *param1, void *param2, 
    declaration_kind_t kind);
variable_t *make_variable(void *param1, void *param2, variable_kind_t kind);
exp_t *make_exp(void *param1, void *param2, exp_kind_t kind);
term_t *make_term(void *param1, void *param2, term_kind_t kind);

// Help Methods
array_list *append_list(array_list *list, void *element);


#endif
