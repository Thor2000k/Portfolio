#ifndef __MEMORY_H__
#define __MEMORY_H__

// allocates the given 
#define NEW(Type) (Type *)Malloc(sizeof(Type))

// allocates an array that can contain Size elements of Type
#define NEWARRAY(Type, Size) (Type *)Malloc(sizeof(Type) * (Size))

/* calls malloc and checks that the memory was allocated. */
void *Malloc(unsigned n);

/* add string literal to heap and returns the address. */
char *malloc_string(char *str);

#endif
