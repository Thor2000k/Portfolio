#include "array_list.h"
#include <stdio.h>

int main()
{
    array_list *list = al_init_list(10, sizeof(int));
    al_set_max(list);
    int temp = 10;
    al_set(list, 2, &temp);

    int *result = al_get(list, 2);
    if (temp == *result)
        fprintf(stderr, "List working\n");
    else
        fprintf(stderr, "List not working: got %d\n", *result);

    return 0;
}