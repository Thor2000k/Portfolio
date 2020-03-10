#ifndef __MEMORY_H__
#define __MEMORY_H__

void *Malloc(unsigned n);

#define NEW(type) (type *)Malloc(sizeof(type))
#define NEWARRAY(type, size) (type *)Malloc(sizeof(type) * (size))

#endif
