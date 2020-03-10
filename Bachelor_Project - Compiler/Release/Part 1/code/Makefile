GCC = gcc
SRC = symbol.c array_list.c memory.c prime_generator.c
IDIR = include
OBJS := $(patsubst %.c, %.o, $(SRC)) 
OBJ_TEST1 = test_symbol_table.o
OBJ_TEST2 = test_symbol_resize.o
CFLAGS = -I$(IDIR) -std=c11 -Wall -Wextra -pedantic

.PHONY: compiler test2 clean

all: compiler

%.o: %.c
	$(GCC) $(CFLAGS) -c -o $@ $<

compiler: $(OBJ_TEST1) $(OBJS)
	$(GCC) $(OBJ_TEST1) $(OBJS) $(CFLAGS) -o compiler -lm

test2: $(OBJ_TEST2) $(OBJS)
	$(GCC) $(OBJ_TEST2) $(OBJS) $(CFLAGS) -o compiler -lm

clean:
	rm -f $(OBJ_TEST1) $(OBJ_TEST2) $(OBJS) compiler
