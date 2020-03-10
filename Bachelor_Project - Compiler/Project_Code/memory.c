
#ifdef __APPLE__
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif

#include "memory.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void *Malloc(unsigned n)
{
    void *p;
    if(!(p = malloc(n)))
    {
        fprintf(stderr,"Malloc(%d) failed.\n",n);
        fflush(stderr);
        abort();
    }
    return p;
}

char *malloc_string(char *str)
{
    void *ptr = NEWARRAY(char, strlen(str) + 1);
    sprintf(ptr, "%s", str);
    
    return ptr;
}
