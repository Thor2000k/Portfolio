
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

//-----------------------------------------------------------------------------
// Vector, a dynamic array using size doubling to achieve amortized constant time insertion
// There are prerequisites mentions in each function description. The user has to make sure
// that these conditions are fulfilled before the function call. Nevertheless, the function
// should check these conditions as well and react accordingly (e.g., exit the program)
//-----------------------------------------------------------------------------

// Main method for testing the program.


// Initialize a vector to be empty
// Pre: 'vector' != NULL
void vector_init(Vector *vector)
{
    if(vector != NULL) //check if vector exists
    {
        void* ptr; //void ptr to fill the array
        void** arr = (void**) malloc((sizeof(void*) * 10)); //array
        
        for(int i = 0; i < 8; i++)
        {
            arr[i] = ptr; //put a void pointer into the array
        }
        
        vector->strings = arr; //set the internal array to this newly created array
        vector->capacity = 8; //set the capacity to 10
        
        }
}

// Deallocate internal structures of the vector
// Note: the user is responsible for deleting all values
// Note: the user is responsible for deleting the actual vector if it was dynamically allocated
void vector_delete(Vector *vector)
{
    void** arr = vector_get_array(vector);
    size_t size = vector_size(vector);
    
    for(int i = 0; i < size; i++)
    {
        free(arr[i]);
    }
    
    free(vector->strings);
}

// Insert a new element at the end of the vector
// Pre: 'vector' != NULL
void vector_push(Vector *vector, void *value)
{
    if(vector != NULL)
    {
       size_t n = strlen(value); 
       char* ptr = (char*) malloc(sizeof(char) * n);
            
       strcpy(ptr, (char*) value);
        
        if(vector_size(vector) < vector_capacity(vector))
        {
            void** arr = vector_get_array(vector);
            arr[vector_size(vector)] = ptr;
            
        } else {
            incSize(vector);
            void** arr = vector_get_array(vector);
            arr[vector_size(vector)] = ptr;
        }
    }
}

// Remove the last element in the vector and return the value
// Pre: the vector is non-empty, 'vector' != NULL
void *vector_pop(Vector *vector)
{
    if(vector != NULL)   
    {
        void* temp = (void*) malloc(sizeof(void*));
        temp = vector_get_element(vector, (vector_size(vector)-1));
        void** arr = vector_get_array(vector);
        arr[(vector_size(vector)-1)] = NULL;
        
        if(vector_size(vector) <= (vector_capacity(vector) / 4))
        {
            decSize(vector);
        }
        
        return temp;
    }
}

// Return the number of elements in the vector
// Pre: 'vector' != NULL
size_t vector_size(const Vector *vector)
{
    size_t counter = 0;
    size_t capa = vector_capacity(vector);
    if(vector != NULL)
    {

        void** arr = vector_get_array(vector);
        
        for(int i = 0; i < capa; i++)
        {
            if(arr[i] != NULL)
            {
                counter++;
            }
        }
    }
    return counter;
}

// Return the current capacity of the vector
// Pre: 'vector' != NULL
size_t vector_capacity(const Vector *vector)
{
    if(vector != NULL){
        return vector->capacity;
    }
}

// Return the value at the given index
// Pre: index < vector_size(vector)
void* vector_get_element(const Vector *vector, size_t index)
{
    if(index < vector_size(vector))
    {
        void** arr = (void**) malloc((sizeof(void**) * vector_capacity(vector)));
        arr = vector_get_array(vector);
        return arr[index];
    }
}

// Return a pointer to the underlying array
void **vector_get_array(const Vector *vector)
{
    return vector->strings;
}

// increase the size of the "array"
void incSize(Vector* vector)
{
    if(vector != NULL)
    {
        size_t newC = (vector_capacity(vector) * 2);
        void** newArr = (void**) malloc((sizeof(void*) * newC));
 
        void** oldArr = (void**) malloc((sizeof(void**) * vector_capacity(vector)));
        oldArr = vector_get_array(vector);
        
        for(int i = 0; i < vector_size(vector); i++)
        {
            newArr[i] = oldArr[i];
        }
        
        
        vector->strings = newArr;
        vector->capacity = newC;
        
        free(oldArr);
    }
}

// decrease the sie of the "array"
void decSize(Vector* vector)
{
    if(vector != NULL)
    {
        
        size_t newC = ((vector_capacity(vector) / 2));
        void** newArr = (void**) malloc((sizeof(void*) * newC));
        
        void** oldArr = vector_get_array(vector);
        
        for(int i = 0; i < vector_size(vector); i++)
        {
            newArr[i] = oldArr[i];
        }
        
        
        vector->strings = newArr;
        vector->capacity = newC;
        
        free(oldArr);
    }
}
