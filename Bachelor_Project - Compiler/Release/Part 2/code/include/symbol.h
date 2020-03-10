
#ifndef __H_SYMBOL_TABLE__
#define __H_SYMBOL_TABLE__

#include "array_list.h"

// elements in table compared to size of array before resizing 
#define SYM_RESIZE_RATIO 1.3
// how much larger the prime of the next size of the array should at least be
#define SYM_RESIZE_SCALE 2
// max number of charachters used to calculate hash
#define MAX_VARIABLE_SIGNIFICANCE (32)

typedef struct _sym_symbol symbol;
typedef struct _sym_table symbol_table;

struct _sym_symbol {
  char *name;
  int value;
  symbol* next; 
};

struct _sym_table {
  int size;
  array_list *hash_table;
  symbol_table *parent;
};

// Get a numerical representation of the string str
long long int Hash(char *str);

// Get the hashed index of str for hash_table 
int hash_index(array_list *hash_table, char *str); 

// Create a new symbol table
symbol_table *init_symbol_table();

// Create a new scope with table as parant
symbol_table *scope_symbol_table(symbol_table *table);

// Add identifier name to the scope of table
symbol *put_symbol(symbol_table *table, char *name, int value);

// Change identifier name in scope or from parent scope
void sym_assign(symbol_table *table, char *name, int value);

// Create a new symbol with identifier name
symbol *_sym_init_symbol(char *name, int value);

// Increase the size of the symbol table and resize if necesary
void _sym_increment_size(symbol_table *table);

// Get the first symbol that has idenfier name in the table or its parents
symbol *get_symbol(symbol_table *table, char *name);

// Resize the hash table to a new prime
void _sym_resize(symbol_table *table);

// Print a string representation of the symbol table and its parents
void dump_symbol_table(symbol_table *table);

// TODO: method for freeing memory

#endif
