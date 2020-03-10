#ifndef __FILE_H__
#define __FILE_H__

#include <stdio.h>
#define DEBUG 0
#define PRINT(S) if (DEBUG) fprintf(stderr, "Found token: %s, in line %d\n", S, lineCount)
#define KEYWORD(K) PRINT(#K); return K;

extern int lineCount;

#endif
