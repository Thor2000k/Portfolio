#include "symbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *random_name(int size)
{
    char *str = malloc(size + 1);
    char *ptr = str;
    while(size){
        memset(ptr, 'a' + (rand() % 26), 1);
        ptr++;
        size--;
    }
    
    memset(ptr, 0, 1);
    return str;
}

int main()
{ 
    symbol_table *tbl = init_symbol_table();

    // make sure no symbol got lost in resizing
    fprintf(stderr, "\nChecking all symbols added\n");
    for (char value = 'a'; value <= 'z'; value++)
    {
        char *str = malloc(sizeof(char) *3);
        memset(str, 0, sizeof(*str) * 3);
        memcpy(str, &value, sizeof(char));
        put_symbol(tbl, str, (int)value);
    }
    
    int count = 0;

    for (char value = 'a'; value <= 'z'; value++)
    {
        char *str = malloc(sizeof(char) *3);
        memset(str, 0, sizeof(*str) * 3);
        memcpy(str, &value, sizeof(char));
        symbol *ptr = get_symbol(tbl, str);

        if (ptr == NULL)
        {
            fprintf(stderr, "Name not found %s\n", str);
            count++;
        }
    }

    if (count == 0)
    {
        fprintf(stderr, "No input lost!\n\n");
    }

    // check spread of random names
    fprintf(stderr, "Checking resize with large quantity of names.\n");
    symbol_table *tbl_rnd = init_symbol_table();

    for (int i = 0; i < 1000; i++)
    {
        char *str = random_name(10);
        put_symbol(tbl_rnd, str, rand() % 26);
    }

    dump_symbol_table(tbl_rnd);
    fprintf(stderr, "\n");

    // Check symbol name length
    fprintf(stderr, "Checking name lenght\n");
    symbol_table *tbl_large = init_symbol_table();

    for (int i = 0; i < 10; i++)
    {
        char *str = random_name(70);
        put_symbol(tbl_large, str, rand() % 26);
    }

    dump_symbol_table(tbl_large);
    fprintf(stderr, "\n");
}

