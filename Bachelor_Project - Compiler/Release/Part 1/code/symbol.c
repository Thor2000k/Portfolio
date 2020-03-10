
#include "symbol.h"
#include "prime_generator.h"
#include <memory.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

long long int Hash(char *str)
{
    long long int sum = 0;
    int current;
    int i = 0; 
    sum = (int) str[i];
    i++; 

    while(str[i] != '\0' && i < MAX_VARIABLE_SIGNIFICANCE)
    {
        current = (int) str[i];
        sum = sum << 1; 
        sum = sum + current;

        i++; 
    }
   
    return sum;
}

int hash_index(array_list *hash_table, char *str)
{
    long long int hash = Hash(str);
    int size = hash_table->size;

    return hash % size;
}

symbol_table *init_symbol_table()
{
    symbol_table *tbl = NEW(symbol_table);
    tbl->hash_table = al_init_list(11, sizeof(void*)); // 11 is prime we choose this to avoid colisions -> good for the report 
    tbl->parent = NULL;
    tbl->size = 0;

    al_set_max(tbl->hash_table);
    return tbl;
}

symbol_table *scope_symbol_table(symbol_table *table)
{
    symbol_table *new = init_symbol_table();
    new->parent = table;
    return new;
}

symbol *put_symbol(symbol_table *table, char *name, int value)
{
    int name_hash = hash_index(table->hash_table, name);
    symbol *ptr;
    
    AL_GET(symbol *, ptr, table->hash_table, name_hash);

    if(ptr == NULL)
    {        
        symbol* sprt = _sym_init_symbol(name, value);

        al_set(table->hash_table, name_hash, &sprt);        
        _sym_increment_size(table);

        return sprt;
    }

    while(ptr->next != NULL && strcmp(ptr->name, name) != 0)
    {
        ptr = ptr->next;
    }

    // ERROR: id already exists;
    if(strcmp(ptr->name,name) == 0)
    {
        fprintf(stderr, "Identifier: %s, already exists.\n", ptr->name);
        // abort(); TODO make compiler abort later;
        return NULL;
    }

    symbol *sptr = _sym_init_symbol(name, value); 

    ptr->next = sptr;
    _sym_increment_size(table);

    return sptr;
}

void sym_assign(symbol_table *table, char *name, int value)
{
    symbol *ptr = get_symbol(table, name); 
    // TODO: check values match

    if (ptr != NULL)
    {
        ptr->value = value;
    }   
}

symbol *_sym_init_symbol(char *name, int value)
{    
    symbol* sprt = NEW(symbol);
    sprt->name = name;
    sprt->next = NULL;
    sprt->value = value;

    return sprt;
}

void _sym_increment_size(symbol_table *table)
{    
    table->size += 1;
    double ratio = table->size / (double)table->hash_table->size;
    double tolerence = log10(table->hash_table->size);

    if (ratio >= tolerence)
    {
        _sym_resize(table);
    }
}

symbol *get_symbol(symbol_table *table, char *name)
{    
    while(table != NULL)
    {
        int name_hash = hash_index(table->hash_table, name);
        symbol *ptr;

        AL_GET(symbol *, ptr, table->hash_table, name_hash);

        if(ptr != NULL)
        {
            while(ptr->next != NULL && strcmp(ptr->name, name) != 0)
            {
                ptr = ptr->next;
            }

            if(strcmp(ptr->name,name) == 0)
            {
                return ptr; 
            }
        }

        table = table->parent;
    }

    fprintf(stderr, "Identifier: %s, doesn't exist.\n", name);
    // abort(); TODO make compiler abort later;

    return NULL;
}

void _sym_resize(symbol_table *table)
{
    int new_size;
    new_size = prime_next(table->hash_table->size * SYM_RESIZE_SCALE);    
    
    array_list *new_list, *old_list;
    new_list = al_init_list(new_size, sizeof(void *));
    al_set_max(new_list);

    old_list = table->hash_table;
    table->hash_table = new_list;
    table->size = 0;    

    symbol *ptr;
    for (int i = 0; i < old_list->size; i++)
    {
        AL_GET(symbol *, ptr, old_list, i);
        while(ptr != NULL)
        {
            put_symbol(table, ptr->name, ptr->value);
            ptr = ptr->next;
        }
    }

    al_clean(old_list, NULL);
}

void dump_symbol_table(symbol_table *table)
{
    int table_index = 1;

    while(table != NULL){
        fprintf(stderr, "Table %d\n", table_index);
        
        for (int index = 0; index < table->hash_table->size; index++)
        {
            symbol *ptr;
            AL_GET(symbol *, ptr, table->hash_table, index);
            fprintf(stderr, "%d ", index);

            if (ptr == NULL)
            {
                fprintf(stderr, "NULL\n");
                continue;
            }

            fprintf(stderr, "(%s, %d)", ptr->name, ptr->value);
            while(ptr->next != NULL)
            {
                ptr = ptr->next;
                fprintf(stderr, " -> (%s, %d)", ptr->name, ptr->value);
            }
            
            fprintf(stderr, "\n");
        }

        fprintf(stderr, "------------------\n");
        fprintf(stderr, "\n");
        table = table->parent;
        table_index++;        
    }    

    // TODO: method for freeing memory
}