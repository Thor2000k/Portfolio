#ifndef __H_LIVELINESS__
#define __H_LIVELINESS__

#include "intermediate_tree.h"

int debug_show_liveness;

// Set operations on binary sets (usually uint)

// A U B, get the union of set A and set B.
#define UNION(A, B)     ((A) | (B))

// Get I'th element of set A.
#define GET(A, I)       (((A) >> (I)) & 1U)

// Set the I'th element in set A to 1.
#define SET(A, I)       A = ((A) | (1U << (I)))

// Set the I'th element in set A to 0.
#define REMOVE(A, I)    A = DIFF((A), (1U << (I)))

// A - B, get the difference of set A and B
#define DIFF(A, B)     ((A) & ~(B))

typedef struct _live_flow_node flow_node_t;
typedef struct _live_statement_block statement_block_t;

// Node in flow control graph
struct _live_flow_node
{
    entry_t *entry;
    array_list *used;
    array_list *defined;
    uint64_t *use_row;
    uint64_t *def_row;
    flow_node_t *basic_succ;
    flow_node_t *jmp_succ;
    flow_node_t *next;
    flow_node_t *previous;
};

// A group of nodes making up a statement.
struct _live_statement_block
{
    unsigned int size;
    uint64_t *temp_in_table;
    hash_table_t *temps;
    hash_table_t *variables;
    flow_node_t *start;
    flow_node_t *last;
    statement_block_t *next;
};

/* Performs resource allocation on temporaries in program and 
    returns a list of the first statement block in a function. */
array_list *live_start_analysis(program_t *program);

#endif
