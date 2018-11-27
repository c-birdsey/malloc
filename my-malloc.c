//calder birdsey
//cs315 systems programming 
//assignment 3: my-malloc 

#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>

#define BLOCK_SIZE (sizeof(struct block))
struct block * FIRST_NODE; 

//define struct file_struct
struct block {
    int free_flag; //flag to determine whether block is free or not-- 0 if free, 1 if not
    size_t size; 
    struct block *next; 
};

struct block *
new_block(int size, struct block *last) {
    struct block *block;
    block = sbrk(BLOCK_SIZE + size + 15); //adding 15 for possible shift in case of alignment issues-- NOT EFFICIENT  
    int align = 16 - (((intptr_t) (block + 1)) % 16); 
    if(align < 16) {
        block = (struct block *)((char *) block + align);  
    }
    /* FOR TESTING 
    * void * next_break;
    * next_break = sbrk(0); 
    * printf("block address: %p\n", (void *) block); 
    * printf("data address: %p\n", (void *) (block + 1)); 
    * printf("next block: %p\n\n", next_break); 
    */ 
    if(block == (void *) - 1) {
        return block; 
    }
    block->size = size;
    block->free_flag = 1; 
    block->next = NULL;  
    if(last) {
        last->next = block; 
    }
    return block; 
} 

void  *
malloc(size_t size) {
    if(size <= 0) {
        return NULL; 
    }

    //first malloc call 
    if(FIRST_NODE == NULL) { 
        struct block *block;  
        block = new_block(size, NULL); 
        FIRST_NODE = block; 
        return (block + 1); 
    }
    //search for open block
    struct block *cur = FIRST_NODE; 
    while(cur) { 
        if(((cur->free_flag) == 0) && ((cur->size) >= size)) {
            cur->size = size;
            cur->free_flag = 1; 
            return (cur + 1); 
        } 
        if(cur->next == NULL) {
            //char buf[6] = "test\n"; 
            //write(1, buf, 6); 
            break; 
        }
        cur = cur->next; 
    } 
    
    //no open block, create new 
    struct block *new_node; 
    new_node = new_block(size, cur); 
    return (new_node + 1); 
}

void
free(void *ptr) {
    if(!ptr) {
        return; 
    }
    struct block *block = ((struct block *) ptr) - 1; 
    memset(block + 1, 0, (block->size)); 
    block->free_flag = 0; 
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
        return malloc(size);  
    }
    if((size == 0) && (ptr)) {
        free(ptr); 
        return NULL;
    }
    struct block *block = ((struct block *) ptr) - 1; 
    if(size <= (block->size)) {
        block->size = size; 
        memset(((char *) ptr + size), 0, (block->size - size));
        return ptr; 
        // does this mem need to be freed? aka make a new block of size (block->size - size)
    } else {
        struct block *new_block = malloc(size); 
        memcpy(new_block, ptr, block->size); 
        return new_block; 
    }
}