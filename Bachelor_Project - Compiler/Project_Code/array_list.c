#include "array_list.h"
#include "memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void al_resize(array_list *list);
static int al_next_size(int size);

array_list *al_init_list(int initial_max_size, int element_size)
{
    array_list *list = NEW(array_list);
    void *array = Malloc(element_size * initial_max_size);
    memset(array, 0, element_size * initial_max_size);
    
    list->size = 0;
    list->max_size = initial_max_size;
    list->element_size = element_size;
    list->array = array;
    list->next_size = &al_next_size;

    return list;
}

void al_add(array_list *list, void *value)
{
    if (list == NULL)
    {
        return;
    }

    list->size += 1;
    if (list->size > list->max_size) 
    {
        al_resize(list);
    }

    al_set(list, list->size - 1, value);
}

void al_add_range(array_list *list, array_list *range)
{
    if (range == NULL || list->element_size != range->element_size)
        return;

    for (int i = 0; i < range->size; i++)
        al_add(list, al_get(range, i));
}

void al_set_max(array_list *list)
{
    if (list == NULL)
    {
        return;
    }

    list->size = list->max_size;
}

void al_set(array_list *list, int index, void *value)
{
    if (list == NULL)
    {
        return;
    }

    if (index >= list->size)
    {
        fprintf(stderr, "array_list index too large %d >= %d.\n",
         index, list->size);
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
    if (list == NULL)
    {
        return NULL;
    }

    if (index >= list->size)
    {
        fprintf(stderr, "array_list index too large %d >= %d.\n",
         index, list->size);
        fflush(stderr);
        abort();
    }
    else if (index < 0)
    {
        fprintf(stderr, "array_list index negative %d.\n", index);
        fflush(stderr);
        abort();
    }

    char *ptr = list->array;

    ptr += index * list->element_size;
    return ptr;
}

/* Default function for calculating the next size of the array on resizing. */
static int al_next_size(int size)
{
    return size <= 0 ? 10 : size * 2;
}


/* Function for resizing and reallocating space for the inner array. */
static void al_resize(array_list *list)
{
    if (list == NULL)
    {
        return;
    }

    int new_size = list->next_size(list->max_size);
    void *new_array = Malloc(new_size * list->element_size);

    memcpy(new_array, list->array, list->max_size * list->element_size);
    free(list->array);

    list->max_size = new_size;
    list->array = new_array;    
}

void al_destroy(array_list *list, void (*destroy_value)(void *))
{    
    if (list == NULL)
    {
        return;
    }

    if (list->element_size == sizeof(void *) && destroy_value != NULL)
    {       
        void **element = list->array;
        for (int index = 0; index < list->size; index++)
        {
            destroy_value(*element);
            element++;
        }
    }

    free(list->array);
    free(list);
}

void al_reset(array_list *list)
{
    memset(list->array, 0, list->size);
    list->size = 0;
}
