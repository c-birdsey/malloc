//calder birdsey
//cs315 systems programming 
//assignment 3 header file 

#include <stdio.h>

//define struct file_struct
struct block {
    int free_flag; //flag to determine whether block is free or not-- 0 if free, 1 if not
    size_t size; 
    struct block *next; 
};

struct block * new_block(int size, struct block *last);
void * malloc(size_t size); 
void free(void *ptr); 
void * calloc(size_t nmemb, size_t size);
void * realloc(void * ptr, size_t size); 