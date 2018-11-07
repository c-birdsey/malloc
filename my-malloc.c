//calder birdsey
//cs315 systems programming 
//assignment 3: my-malloc 

#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>

#define BLOCK_SIZE (sizeof(struct block_struct))
void * first_node; 

//define struct file_struct
struct block_struct {
    int free_flag; //flag to determine whether block is free or not-- 0 if free, 1 if not
    size_t size; 
    struct block_struct *next; 
    int tag; // FOR TESTING
};

struct block_struct *
new_block(int size, struct block_struct *last, int tag) {
    struct block_struct *block;
    block = sbrk(BLOCK_SIZE + size + 15); //adding 15 for possible shift in case of alignment issues-- NOT EFFICIENT  
    int align =  16 - (((intptr_t) block) % 16); 
    if(align < 16) {
        block = (struct block_struct *)((char *) block + align);  
    }
    /* FOR TESTING 
    * void * next_break;
    * next_break = sbrk(0); 
    * printf("block address: %p\n", (void *) block); 
    * printf("data address: %p\n", (void *) (block + 1)); 
    * printf("next block: %p\n\n", next_break); 
    */ 
    if(block == (void *) - 1) {
        perror("sbrk"); 
        return NULL; 
    }
    block -> size = size;
    block -> free_flag = 1; 
    block -> next = NULL;  
    block -> tag = tag; 
    if(last) {
        last -> next = block; 
    }
    return block; 
} 

void  *
malloc(size_t size) {
    int tag = 0; 
    if(size <= 0) {
        return NULL; 
    }

    //first malloc call 
    if(first_node == NULL) { 
        struct block_struct *block;  
        block = new_block(size, NULL, tag); 
        first_node = block; 
        //printf("tag (first node): %d\n", block -> tag); 
        return (block + 1); 
    } else {

        //search for open block
        struct block_struct *cur = first_node; 
        while(cur) { 
            if((cur->free_flag == 0) && ((cur-> size) >= size)) {
                cur -> size = size; 
                cur -> free_flag = 1; 
                cur -> tag = tag; 
                //printf("tag (free block found): %d\n", cur -> tag); 
                return (cur + 1); 
            }
            if(!(cur -> next)) {
                break; 
            } else {
                cur = cur -> next; 
            }
        } 

        //no open block, create new 
        struct block_struct *new_node; 
        new_node = new_block(size, cur, tag); 
        //printf("tag (new node): %d\n", new_node -> tag); 
        return (new_node + 1); 
    }
}

void
free(void *ptr) {
    if(!ptr) {
        return; 
    }
    struct block_struct *block = ((struct block_struct *) ptr) - 1; 
    memset(block + 1, 0, (block -> size)); 
    block -> free_flag = 0; 
}

void *
calloc(size_t nmemb, size_t size) {
    if((nmemb == 0) || (size == 0)) {
        return NULL; 
    }
    size_t alloc_size = nmemb * size; 
    void * ptr = malloc(10); 
    return memset(ptr, 0, alloc_size); 
}

void *
realloc(void * ptr, size_t size) {
    if(!ptr) { 
        malloc(size); 
    }
    if((size == 0) && (ptr)) {
        free(ptr); 
    }
    struct block_struct *block = ((struct block_struct *) ptr) - 1; 
    size_t ptr_size = block -> size; 
    if(size <= ptr_size) {
        block -> size = size; 
        memset(((char *) ptr + size), 0, (ptr_size - size));
        return ptr; 
        // does this mem need to be freed? aka make a new block of size (ptr_size - size)
    } else {
        struct block_struct *new_block = malloc(size); 
        memcpy(new_block, ptr, ptr_size); 
        return new_block; 
    }
}