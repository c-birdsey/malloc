//calder birdsey
//test-malloc
//cs315 assignment 3

#include <stdio.h>
#include "my-malloc.h"

int 
main(int argc, char const *argv[]) {
    char *test1 = malloc(1000); 
    char *test2 = malloc(1000);
    free(test2); 
    char *test3 = malloc(1000);
    free(test1); 
    char *test4 = malloc(1000); 
    printf("mem add of mymalloc 1: %p\n", test1); 
    printf("mem add of mymalloc 2: %p\n", test2); 
    printf("mem add of mymalloc 3: %p\n", test3); 
    printf("mem add of mymalloc 4: %p\n", test4); 
}
