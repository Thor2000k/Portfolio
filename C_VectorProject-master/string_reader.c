#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_reader.h"

int main(int argc, char *argv[])
{

	//initializing the Vector data structure. 

    Vector* myVector = (Vector*) malloc(sizeof(Vector*));
    vector_init(myVector);
    

	//initializing the input file 
	FILE *file_handle;
	file_handle = fopen(argv[1], "r");
	char buffer[1000];

	//checking if the file has been read correct
	if(file_handle == NULL ){
		printf("Error opening file\n");
		return 0; 
	}

	//looping over each string in the input file
	while(fgets(buffer, 100, file_handle) != NULL){
		
        void* bptr = (void*) malloc(sizeof(char*));
		bptr = &buffer;

		vector_push(myVector, bptr);
	}

	//calling qsort
	sortMachine(myVector);
	fclose(file_handle);
    
    //print the sorted list
    size_t size = vector_size(myVector);
    for(int i = 0; i < size; i++)
    {
        char* ele = (char*) vector_get_element(myVector, i);
        printf("%s", ele);
    }
	
	
    return 0;

}

//function to sort a vectors array 
void sortMachine(Vector *vector)
{
	void** array = vector_get_array(vector);
	
	qsort(array, vector_size(vector), (size_t) sizeof(char*), cmpfunc);

}

//Compare function used by qsort. 
int cmpfunc(const void *a , const void *b)
{
    return (strcasecmp(*(char**)a, *(char**)b));
        
}








