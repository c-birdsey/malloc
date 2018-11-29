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
    write(1, "new block\n", 10);
    if((cur_brk + size + 1024) >= heap_brk) { 
        if ((cur_brk = sbrk(size * 1024)) == (void *)-1) {
            return (struct block *) cur_brk;  
        }
        if((heap_brk = sbrk(0)) == (void *)-1) {
            return (struct block *) heap_brk; 
        }
        write(1, "sbrk\n", 5);
    }
    struct block *block = (struct block *) cur_brk;
    cur_brk += (BLOCK_SIZE + size + 15); //adding 15 for possible shift in case of alignment issues-- NOT EFFICIENT  
    //struct block *block = sbrk(BLOCK_SIZE + size + 1500);
    int align = 16 - (((intptr_t) (block + 1)) % 16); 
    if(align < 16) {
        block = (struct block *)((char *) block + align);  
    }
    block->size = size;
    block->free_flag = 1; 
    block->next = NULL;  
    if(last) {
        last->next = block; 
    }
    char buffer1[40];
    intptr_t z = (intptr_t) (block + 1); 
    snprintf(buffer1, sizeof(buffer1), "malloc ret: 0x%lx\n", z);
    write(1, buffer1, strlen(buffer1));
    z = (intptr_t) cur_brk; 
    snprintf(buffer1, sizeof(buffer1), "cur: 0x%lx\n", z);
    write(1, buffer1, strlen(buffer1));
    z = (intptr_t) heap_brk; 
    snprintf(buffer1, sizeof(buffer1), "heap: 0x%lx\n\n", z);
    write(1, buffer1, strlen(buffer1));
    return block; 
} 

void  *
malloc(size_t size) {
    //write(1, "malloc\n", 7);
    if(size <= 0) {
        return NULL; 
    }
    //first malloc call 
    if(first_node == NULL) { 
        //write(1, "FIRST CALL\n", 11);
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
            //write(1, "EXISTING NODE\n", 14);
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
    //write(1, "NEW NODE\n", 9);
    struct block *new_node; 
    new_node = new_block(size, cur); 
    return (new_node + 1); 
}

void
free(void *ptr) {
    write(1, "free\n", 5);
    if(!ptr) {
        return; 
    }
    struct block *block = ((struct block *) ptr) - 1; 
    memset(block + 1, 0, (block->size)); 
    block->free_flag = 0; 
}

void *
calloc(size_t nmemb, size_t size) {
    write(1, "calloc\n", 7);
    if((nmemb == 0) || (size == 0)) {
        return NULL; 
    }
    size_t alloc_size = nmemb * size; 
    void * ptr = malloc(10); 
    return memset(ptr, 0, alloc_size); 
}

void *
realloc(void * ptr, size_t size) {
    write(1, "realloc\n", 8);
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