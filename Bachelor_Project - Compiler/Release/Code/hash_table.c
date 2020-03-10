#include "hash_table.h"
#include "memory.h"
#include "misc.h"
#include <stdlib.h>
#include <string.h>

static unsigned long long int hash_string(char *str);
static unsigned long long int hash_size_t(void *key);
static int hash_index(hash_table_t *table, void *key);
static ht_node_t *ht_init_node(void *key);
static ht_node_t *ht_look_up(hash_table_t *table, void *key);
static void ht_increment_size(hash_table_t *table);
static void ht_resize(hash_table_t *table);
static ht_node_t *ht_put_node(hash_table_t *table, void *key, int *modified);

static unsigned long long int hash_string(char *str)
{
    unsigned long long int sum = 0;
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

static unsigned long long int hash_size_t(void *key)
{
    return 5 * (size_t)key;
}

static int hash_index(hash_table_t *table, void *key)
{
    unsigned long long int hash;
    if (table->key_kind == kind_ht_key_string)
        hash = hash_string(key);
    else 
        hash = hash_size_t(key);

    return hash % table->inner_array->size;
}

static ht_node_t *ht_init_node(void *key)
{    
    ht_node_t *node = NEW(ht_node_t);
    node->key = key;
    node->next = NULL;

    return node;
}

/* get node with key or last node at keys hash */
static ht_node_t *ht_look_up(hash_table_t *table, void *key)
{
    ht_node_t *node;

    int index = hash_index(table, key);

    AL_GET_PTR(node, table->inner_array, index);

    if (node == NULL)
        return NULL;

    if (table->key_kind == kind_ht_key_string)
    {            
        while (node->next != NULL && strcmp(key, node->key) != 0)
        {
            node = node->next;
        }
    }
    else
    {   
        while (node->next != NULL && key != node->key)
        {
            node = node->next;
        }
    }

    return node;
}

/* returns 1 if table has key, otherwise 0. */
int ht_contains(hash_table_t *table, void *key)
{
    ht_node_t *node = ht_look_up(table, key);

    if (node == NULL)
        return 0;
    else if (table->key_kind == kind_ht_key_string && 
        strcmp(node->key, key) == 0)
        return 1;
    else if (table->key_kind == kind_ht_key_pointer &&
        node->key == key)
        return 1;    
    else 
        return 0;
}

static void ht_increment_size(hash_table_t *table)
{    
    double ratio = (table->size + 1) / (double)table->inner_array->size;
    double tolerence = SYM_RESIZE_RATIO;
    
    if (ratio >= tolerence)
    {
        ht_resize(table);
    }

    table->size += 1;
}

static void ht_resize(hash_table_t *table)
{
    int new_size;
    new_size = prime_next(table->inner_array->size * SYM_RESIZE_SCALE);    
    
    array_list *new_list, *old_list;
    new_list = al_init_list(new_size, sizeof(void *));
    al_set_max(new_list);

    old_list = table->inner_array;
    table->inner_array = new_list;
    table->size = 0;    

    ht_node_t *ptr;
    ht_node_t *old_ptr;
    int i;
    
    AL_FOR_EACH(ht_node_t *, ptr, old_list, i)
    {
        while(ptr != NULL)
        {
            if (table->value_kind == kind_hash_int)
                ht_put_int(table, ptr->key, ptr->value.integer);
            else
                ht_put_pointer(table, ptr->key, ptr->value.pointer);

            old_ptr = ptr;
            ptr = ptr->next;
            free(old_ptr);
        }
    }

    al_destroy(old_list, NULL);
}

static ht_node_t *ht_put_node(hash_table_t *table, void *key, int *modified)
{
    ht_node_t *node, *node_lookup;
    int name_hash;

    node_lookup = ht_look_up(table, key);
    
    if (!ht_contains(table, key))
    {
        *modified = 0;
        node = ht_init_node(key);
        ht_increment_size(table);
        node_lookup = ht_look_up(table, key);

        if (node_lookup == NULL)
        {
            name_hash = hash_index(table, key);
            al_set(table->inner_array, name_hash, &node);
        }
        else
        {
            node_lookup->next = node;
        }
        
    }
    else
    {
        node = node_lookup;
        *modified = 1;
    }

    return node;
}

/* create a new hash table. */
hash_table_t *ht_init_hash_table(ht_hash_table_kind_t value_kind, ht_key_kind_t key_kind)
{
    hash_table_t *table = NEW(hash_table_t);
    table->inner_array = al_init_list(11, sizeof(void *));
    table->value_kind = value_kind;
    table->key_kind = key_kind;
    table->size = 0;

    al_set_max(table->inner_array);

    return table;
}

/* add int to hash table. 
 returns 0 if the key was already there, otherwise 1 */
int ht_put_int(hash_table_t *table, void *key, int value)
{
    int modified;
    ht_node_t *node = ht_put_node(table, key, &modified);

    node->value.integer = value;

    return !modified;
}

/* add pointer to hash table. 
 Returns 0 if the key was already there, 1 otherwise. */
int ht_put_pointer(hash_table_t *table, void *key, void *value)
{    
    int modified;
    ht_node_t *node = ht_put_node(table, key, &modified);

    node->value.pointer = value;

    return !modified;
}

/* get int associated with key, return default if not found. */
int ht_get_int(hash_table_t *table, void *key, int default_value)
{
    if (!ht_contains(table, key))
        return default_value;

    ht_node_t *node = ht_look_up(table, key);
    return node->value.integer;
}

/* get pointer associated with key, return default if not found. */
void *ht_get_pointer(hash_table_t *table, void *key, void *default_value)
{
    if (!ht_contains(table, key))
        return default_value;

    ht_node_t *node = ht_look_up(table, key);
    return node->value.pointer;
}


/* get a list of all keys in the hash table. */
array_list *ht_get_keys(hash_table_t *table)
{
    ht_node_t *node;

    array_list *inner_list = table->inner_array;
    array_list *keys = AL_INIT_PTR(table->size);

    int i = 0;
    AL_FOR_EACH(ht_node_t *, node, inner_list, i)
    {
        while (node != NULL)
        {
            al_add(keys, &node->key);
            node = node->next;
        }
    }

    return keys;
}

/* get a list of all values in the hash table. */
array_list *ht_get_values(hash_table_t *table)
{
    ht_node_t *node;

    array_list *inner_list = table->inner_array;
    array_list *values;

    if (table->value_kind == kind_hash_int)
        values = al_init_list(table->size, sizeof(int));
    else
        values = AL_INIT_PTR(table->size);

    int i = 0;
    AL_FOR_EACH(ht_node_t *, node, inner_list, i)
    {
        while (node != NULL)
        {
            if (table->value_kind == kind_hash_int)
                al_add(values, &node->value.integer);
            else
                al_add(values, &node->value.pointer);
                
            node = node->next;
        }
    }

    return values;
}

/* free a chain of nodes from memory */
void ht_destroy_node(void *v_node)
{
    ht_node_t *node, *next;
    node = v_node;

    while (node != NULL)
    {
        next = node->next;
        free(node);
        node = next;
    }
}

/* free hash table from memory. */
void ht_destroy(hash_table_t *table)
{
    if (table != NULL)
    {
        al_destroy(table->inner_array, &ht_destroy_node);
        free(table);
    }
}
