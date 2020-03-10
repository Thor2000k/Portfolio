
#ifndef __H_SYMBOL_TABLE__
#define __H_SYMBOL_TABLE__

#include "hash_table.h"
#include "intermediate_tree.h"
#include "tree.h"

typedef struct _sym_symbol symbol_t;
typedef struct _sym_table symbol_table;

// forward declaration
typedef struct _function function_t;
typedef struct _type type_t;
typedef struct _imt_arg arg_t;

// Symbol node
typedef enum
{
    kind_s_type,
    kind_s_var,
    kind_s_ID,
    kind_s_function
} symbol_kind_t;

typedef union
{
    type_t *type;
    function_t *func;
} symbol_value_t;

struct _sym_symbol {
    char *name;
    int is_marked;
    int is_parameter;
    symbol_kind_t kind;
    symbol_value_t value;
    int longest_jump;
    arg_t *arg;
};

// Symbol table
struct _sym_table {
    hash_table_t *hash_table;
    int depth;
    symbol_table *parent;
};

/* get a numerical representation of the string str. */
long long int Hash(char *str);

/* get the hashed index of str for hash_table. */
int hash_index(array_list *hash_table, char *str); 

/* create a new symbol table. */
symbol_table *sym_init_table();

/* create a new scope with table as parant. */
symbol_table *sym_scope_table(symbol_table *table);

/* add identifier name to the scope of table. */
symbol_t *sym_put(symbol_table *table, char *name, symbol_kind_t kind, void* value);

/* get the first symbol that has idenfier name in the table or its parents. */
int sym_get(symbol_table *table, char *name, symbol_t **symbol_ptr, int set_jumps);

/* get all symbols in table that has kind. */
array_list *sym_get_all(symbol_table *table, symbol_kind_t kind);

/* print a string representation of the symbol table and its parents. */
void sym_print(symbol_table *table);

/* only prints the passed table. */
void sym_print_table(symbol_table *table);

/* print a single symbol. Used by sym_print.  */
void sym_print_symbol(symbol_t *symbol);

#endif
