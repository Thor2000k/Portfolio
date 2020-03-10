#include "array_list.h"
#include "memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

array_list *al_init_list(int initial_max_size, int element_size)
{
    array_list *list = NEW(array_list);
    void *array = Malloc(element_size * initial_max_size);
    memset(array, 0, element_size * initial_max_size);
    
    list->size = 0;
    list->max_size = initial_max_size;
    list->element_size = element_size;
    list->array = array;
    list->next_size = &_al_next_size;

    return list;
}

void al_add(array_list *list, void *value)
{
    list->size += 1;
    if (list->size > list->max_size) 
    {
        _al_resize(list);
    }

    al_set(list, list->size - 1, value);
}

void al_set_max(array_list *list)
{
    list->size = list->max_size;
}

void al_set(array_list *list, int index, void *value)
{
    if (index >= list->size)
    {
        fprintf(stderr, "array_list index to large %d >= %d.\n", index, list->size);
        fflush(stderr);
        abort();
    }

    char *to;
    
    to = list->array;
    to += index * list->element_size;

    memcpy(to, value, list->element_size);
}

void *al_get(array_list *list, int index)
{
    if (index >= list->size)
    {
        fprintf(stderr, "array_list index to large %d >= %d.\n", index, list->size);
        fflush(stderr);
        abort();
    }

    char *ptr = list->array;

    ptr += index * list->element_size;
    return ptr;
}

int _al_next_size(int size)
{
    return size * 2;
}

void _al_resize(array_list *list)
{
    int new_size = list->next_size(list->max_size);
    void *new_array = Malloc(new_size * list->element_size);

    memcpy(new_array, list->array, list->max_size * list->element_size);
    free(list->array);

    list->max_size = new_size;
    list->array = new_array;    
}

void al_clean(array_list *list, void (*clean_value)(void *))
{
    if (list->element_size == sizeof(void *) && clean_value != NULL)
    {
        void **element = list->array;
        for (int index = 0; index < list->size; index++)
        {
            clean_value(element);
            element++;
        }
    }

    free(list->array);
    free(list);
}