#makefile for the project
#macros
CFLAGS = -Wall
OBJ = string_reader.o vector.o

#file directory
vpath %.c src
vpath %.h src

#targets
program: $(OBJ)
	gcc $(CFLAGS) $(OBJ) -o program
	
#source files
%.o: %.h

#test parameters
test1:
	./program test.txt
	./program alphabet.txt

#should remove all *.o files after compilation
.PHONY: clean
clean:
	rm -f *.o
