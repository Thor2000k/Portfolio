#ifndef __MEMORY_H__
#define __MEMORY_H__

void *Malloc(unsigned n);

#define NEW(type) (type *)Malloc(sizeof(type))

#endif