
#include "symbol.h"
#include "misc.h"
#include "pretty_printer.h"
#include <memory.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

symbol_t *sym_init_symbol(char *name, symbol_kind_t kind, void* value)
{    
    symbol_t* sprt = NEW(symbol_t);
    sprt->name = name;
    sprt->is_marked = 0;
    sprt->is_parameter = 0;
    sprt->longest_jump = -1;
    sprt->kind = kind; 
    sprt->value.type = value;
    sprt->arg = NULL;

    return sprt;
}

symbol_table *sym_init_table()
{
    symbol_table *tbl = NEW(symbol_table);
    tbl->hash_table = ht_init_hash_table(kind_hash_pointer, kind_ht_key_string); 
    tbl->parent = NULL;
    tbl->depth = 0;

    return tbl;
}

symbol_table *sym_scope_table(symbol_table *table)
{
    symbol_table *new = sym_init_table();
    new->parent = table;
    new->depth = table->depth + 1;
    return new;
}

symbol_t *sym_put(symbol_table *table, char *name, symbol_kind_t kind, void* value)
{
    if (ht_contains(table->hash_table, name))
    {
        return NULL;
    }

    symbol_t *sptr = sym_init_symbol(name, kind, value);
    ht_put_pointer(table->hash_table, name, sptr);

    return sptr;
}

int sym_get(symbol_table *table, char *name, symbol_t **symbol_ptr, int set_jumps)
{    
    int jumps = 0;
    while(table != NULL)
    {
        symbol_t *ptr = ht_get_pointer(table->hash_table, name, NULL);

        if (ptr != NULL)
        {
            if (ptr->longest_jump < jumps && set_jumps)
                ptr->longest_jump = jumps;
                
            *symbol_ptr = ptr;
            return jumps;
        }

        table = table->parent;
        jumps++;
    }

    return -1;
}

array_list *sym_get_all(symbol_table *table, symbol_kind_t kind)
{
    array_list *inner_list = table->hash_table->inner_array;
    array_list *symbol_col = AL_INIT_PTR(5);
    symbol_t *symbol;
    ht_node_t *node;

    for (int i = 0; i < inner_list->size; i++)
    {
        AL_GET_PTR(node, inner_list, i);
        
        while (node != NULL)
        {
            symbol = node->value.pointer;
            if (symbol->kind == kind)
                al_add(symbol_col, &symbol);

            node = node->next;
        }        
    }

    return symbol_col;
}

void sym_print(symbol_table *table)
{
    while(table != NULL){
        sym_print_table(table);
        table = table->parent;
    }
}

void sym_print_table(symbol_table *table)
{
    array_list *inner_list = table->hash_table->inner_array;
    fprintf(stderr, "=== Table ===\nDepth: %d\n", table->depth);

    for (int index = 0; index < inner_list->size; index++)
    {
        symbol_t *ptr;
        ht_node_t *node;
        AL_GET_PTR(node, inner_list, index);
        fprintf(stderr, "%d ", index);

        if (node == NULL)
        {
            fprintf(stderr, "NULL\n");
            continue;
        }

        ptr = node->value.pointer;
        sym_print_symbol(ptr);
        while(node->next != NULL)
        {
            node = node->next;
            fprintf(stderr, " -> ");
            ptr = node->value.pointer;
            sym_print_symbol(ptr);
        }
        
        fprintf(stderr, "\n");
    }

    fprintf(stderr, "------------------\n");
    fprintf(stderr, "\n");

}

void sym_print_symbol(symbol_t *symbol)
{
    switch (symbol->kind)
    {
        case kind_s_type:
            fprintf(stderr, "(KIND: s_type, NAME: %s, TYPE: <", symbol->name);
            print_type(symbol->value.type);
            fprintf(stderr, ">");
            break;

        case kind_s_var:
            fprintf(stderr, "(KIND: s_var, NAME: %s, TYPE: <", symbol->name);
            print_type(symbol->value.type);
            fprintf(stderr, ">");
            break;
            
        case kind_s_ID:
            fprintf(stderr, "(KIND: s_id, NAME: %s, TYPE: <", symbol->name);
            print_type(symbol->value.type);
            fprintf(stderr, ">");
            break;

        case kind_s_function:
            fprintf(stderr, "(KIND: s_func, NAME: %s, TYPE: <(", symbol->name);
            print_list(symbol->value.func->head->par_list, ", ", &print_var_type);
            fprintf(stderr, "):");
            print_type(symbol->value.func->head->type);
            fprintf(stderr, ">");
            break;            
    }
    
    fprintf(stderr, ")");
}
