//calder birdsey
//cs315 systems programming 
//assignment 3: my-malloc 

#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h> 
#include "my-malloc.h"

#define BLOCK_SIZE sizeof(struct block)

static char *heap_brk = NULL; 
static char *cur_brk = NULL;  
struct block *first_node;

struct block *
new_block(int size, struct block *last) {
    if((cur_brk + size + 1024) >= heap_brk) { 
        cur_brk = sbrk(size + 1024); 
        if (cur_brk == (void *) -1) {
            return (struct block *) cur_brk;  
        }
        heap_brk = sbrk(0); 
        if (heap_brk == (void *) -1) {
            return (struct block *) heap_brk; 
        }
    }
    struct block *block = (struct block *) cur_brk;
    cur_brk += (BLOCK_SIZE + size + 100); //adding 100 for possible shift in case of alignment issues

    //align meta data block
    int align_meta = 16 - (((intptr_t) (block)) % 16); 
    if(align_meta < 16) {
        block = (struct block *)((char *) block + align_meta);  
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
    if(first_node == NULL) { 
        struct block *block;  
        block = new_block(size, NULL); 
        first_node = block; 
        return (block + 1); 
    }
    //search for open block
    struct block *cur = first_node; 
    while(cur) { 
        if(((cur->free_flag) == 0) && ((cur->size) >= size)) {
            cur->size = size;
            cur->free_flag = 1; 
            return (cur + 1); 
        } 
        if(cur->next == NULL) {
            //char buf[6] = "test\n"; 
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
    } else {
        struct block *new_block = malloc(size); 
        memcpy(new_block, ptr, block->size); 
        return new_block; 
    }
}