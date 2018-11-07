//calder birdsey
//cs315 systems programming 
//assignment 3 header file 

struct block_struct * new_block(int size, struct block_struct *last, int tag);
void  * malloc(size_t size); 
void free(void *ptr); 
void * calloc(size_t nmemb, size_t size);
void * realloc(void * ptr, size_t size); 