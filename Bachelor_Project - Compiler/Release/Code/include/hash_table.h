
#ifndef __H_HASH_TABLE__
#define __H_HASH_TABLE__

#include "array_list.h"
#include <stdlib.h>

// elements in table compared to size of array before resizing 
#define SYM_RESIZE_RATIO 0.73

// how much larger the prime of the next size of the array should at least be
#define SYM_RESIZE_SCALE 2

// max number of charachters used to calculate hash
#define MAX_VARIABLE_SIGNIFICANCE (32)

typedef struct _ht_node ht_node_t;

// Struct for a key value pair
typedef union
{
    void *pointer;
    int integer;
} ht_node_value_t;

struct _ht_node
{
    void *key;
    ht_node_value_t value;
    ht_node_t *next;
};

// Struct for the hash table
typedef enum
{
    kind_ht_key_string,
    kind_ht_key_pointer
} ht_key_kind_t;

typedef enum
{
    kind_hash_int,
    kind_hash_pointer
} ht_hash_table_kind_t;

typedef struct
{
    int size;
    ht_hash_table_kind_t value_kind;
    ht_key_kind_t key_kind;
    array_list *inner_array;
} hash_table_t;

/* create a new hash table. */
hash_table_t *ht_init_hash_table(ht_hash_table_kind_t value_kind, 
    ht_key_kind_t key_kind);

/* returns 1 if table has key, otherwise 0. */
int ht_contains(hash_table_t *table, void *key);

/* add int to hash table. 
    returns 0 if the key was already there, otherwise 1. */
int ht_put_int(hash_table_t *table, void *key, int value);

/* add pointer to hash table. 
 Returns 0 if the key was already there, otherwise 1. */
int ht_put_pointer(hash_table_t *table, void *key, void *value);

/* get int associated with key, return default if not found. */
int ht_get_int(hash_table_t *table, void *key, int default_value);

/* get pointer associated with key, return default if not found. */
void *ht_get_pointer(hash_table_t *table, void *key, void *default_value);

/* get a list of all keys in the hash table. */
array_list *ht_get_keys(hash_table_t *table);

/* get a list of all values in the hash table. */
array_list *ht_get_values(hash_table_t *table);

/* clear hash table from memory */
void ht_destroy(hash_table_t *table);

#endif
