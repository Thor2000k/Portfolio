#ifndef __H_ARRAY_LIST__
#define __H_ARRAY_LIST__

#define AL_GET(T, V, L, I) T* result = al_get(L, I); V = *result // computes the result and dereferences the pointer, saving the job of having to do this explicitly

typedef struct al_list_ array_list;
//typedef enum { NUM, REal, CHAR, VOID} al_kind;

struct al_list_ 
{
    int size;
    int max_size;
    int element_size;
    void *array;
    int (*next_size)(int size);
};

/* Methods */
array_list *al_init_list(int initial_max_size, int element_size);
void al_add(array_list *list, void *value);
void al_set_max(array_list *list);
void al_set(array_list *list, int index, void *value);
void *al_get(array_list *list, int index);
int _al_next_size(int size);
void _al_resize(array_list *list);
void al_clean(array_list *list, void (*clean_value)(void *));

#endif
