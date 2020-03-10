#include "link_list.h"
#include "memory.h"
#include <stdlib.h>

linked_list *ll_init_list()
{
    linked_list *list = NEW(linked_list);

    list->size = 0;
    list->first_node = NULL;
    list->last_node = NULL;

    return list;
}

void ll_add(linked_list *list, ll_value value)
{
    ll_node *new_node;
    
    new_node = NEW(linked_list);

    new_node->value = value;
    new_node->next = NULL;
    new_node->previous = list->last_node;

    if (list->size == 0)
    {
        list->first_node = new_node;
    }
    else
    {
        list->last_node->next = new_node;
    }
    
    list->last_node = new_node;
    list->size += 1;
}

ll_value ll_get_at(linked_list *list, int index)
{
    if (index >= list->size)
    {
        // print error
    }
    
    ll_node *node;
    int i;
    node = list->first_node;

    while(i < index)
    {
        i++;
        node = node->next;
    }
}


ll_value ll_get_from(linked_list *list, int (*predicate)(ll_value), ll_value not_found)
{
    ll_node *node = list->first_node;

    while(node != NULL && predicate(node->value) != 1)
    {
        node = node->next;
    }
    
    return node == NULL ? not_found : node->value;    
}

void ll_clean(linked_list *list)
{
    ll_node *node, *next;
    node = list->first_node;

    while(node != NULL)
    {
        next = node->next;
        free(node);
        node = next;
    }
    
    free(list);
}