#ifndef __H_ARRAY_LIST__
#define __H_ARRAY_LIST__

// computes the result and dereferences the pointer, saving the job of having to do this explicitly
// Gets the I'th element and assigns it to V. 
#define AL_GET(T, V, L, I) V = ((I) < (L)->size && (I) >= 0) ? *(T*)(al_get((L), (I))) : (T)0

// Shorter version for pointers.
#define AL_GET_PTR(V, L, I) AL_GET(void *, V, L, I)

// Initialize a list of elements with size of a pointer.
#define AL_INIT_PTR(Size) al_init_list(Size, sizeof(void *))

// Iterate all elements in the list L, with I being a variable for the index.
#define AL_FOR_EACH(T, V, L, I) \
    I = 0; \
    for (AL_GET(T, V, L, I); I < L->size; I++, AL_GET(T, V, L, I))

// Iterate all elements in reverse order.
#define AL_FOR_EACH_REVERSE(T, V, L, I) \
    I = L->size - 1; \
    for (AL_GET(T, V, L, I); I >= 0; I--, AL_GET(T, V, L, I))

typedef struct al_list_ array_list;

struct al_list_ 
{
    int size;
    int max_size;
    int element_size;
    void *array;
    int (*next_size)(int size);
};

/* methods */
/* allocate an empty list with initial inner array size of 
 argument 1 and elements of element_size bytes. */ 
array_list *al_init_list(int initial_max_size, int element_size);

/* add element value to the end of the list. */
void al_add(array_list *list, void *value);

/* add elements from range to the list. */
void al_add_range(array_list *list, array_list *range);

/* fill the inner array with 0 and increase size to that of the inner. */
void al_set_max(array_list *list);

/* change the value of an element at index. */
void al_set(array_list *list, int index, void *value);

/* return the address of the index in the list. */
void *al_get(array_list *list, int index);

/* runs destroy_value for each element and the deallocates 
    the list and its inner array. */
void al_destroy(array_list *list, void (*destroy_value)(void *));

/* set the size of list to 0 and set all bytes in the inner array to 0. */
void al_reset(array_list *list);

#endif
