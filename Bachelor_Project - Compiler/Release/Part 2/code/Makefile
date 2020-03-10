GCC = gcc
SRC = lex.yy.c parser.tab.c pretty_printer.c tree.c symbol.c array_list.c memory.c prime_generator.c
#SRC_ALL = test_symbol_resize.c test_symbol_table.c $(SRC)
#SRC_TEST1 = test_symbol_table.c $(SRC)
#SRC_TEST2 = test_symbol_resize.c $(SRC)
IDIR = include
OBJS := $(patsubst %.c, build/%.o, $(SRC)) 
OBJ_TEST1 = build/test_symbol_table.o
OBJ_TEST2 = build/test_symbol_resize.o
OBJ_MAIN = build/main.o
CFLAGS = -I$(IDIR) -ggdb -std=c11 -Wall -Wextra -pedantic

.PHONY: compiler test1 test2 bison flex clean 

all: 
	mkdir -p build
	make flex 
	make compiler

build/%.o: %.c
	$(GCC) $(CFLAGS) -c -o $@ $<

build/%.%.o: %.%.c
	$(GCC) $(CFLAGS) -c -o $@ $<

build/lex.yy.o: lex.yy.c
	$(GCC) $(CFLAGS) -c -o $@ $<

build/parser.tab.o: parser.tab.c
	$(GCC) $(CFLAGS) -c -o $@ $<

compiler: $(OBJ_MAIN) $(OBJS)
	$(GCC) $(OBJ_MAIN) $(OBJS) $(CFLAGS) -o compiler -lm

test1: $(OBJ_TEST1) $(OBJS)
	$(GCC) $(OBJ_TEST1) $(OBJS) $(CFLAGS) -o compiler -lm

test2: $(OBJ_TEST2) $(OBJS)
	$(GCC) $(OBJ_TEST2) $(OBJS) $(CFLAGS) -o compiler -lm

flex: bison 
	flex lex.l

bison:
	bison --defines='include/parser.tab.h' parser.y

clean:
	rm -rf build compiler parser.tab.c include/parser.tab.h lex.yy.c 
