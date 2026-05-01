// TRACELESS OS
// kernel/memory.c
// 64-bit memory allocator

#include "memory.h"

typedef struct block {
    unsigned long  size;
    int            free;
    struct block  *next;
} block_t;

static block_t *heap = (block_t*)HEAP_START;
static int initialized = 0;

void memory_init() {
    heap->size = HEAP_SIZE - sizeof(block_t);
    heap->free = 1;
    heap->next = 0;
    initialized = 1;
}

void *kmalloc(unsigned long size) {
    if (!initialized) memory_init();
    block_t *current = heap;
    while (current) {
        if (current->free && current->size >= size) {
            if (current->size > size + sizeof(block_t)) {
                block_t *new_block = (block_t*)((char*)current + sizeof(block_t) + size);
                new_block->size = current->size - size - sizeof(block_t);
                new_block->free = 1;
                new_block->next = current->next;
                current->next = new_block;
                current->size = size;
            }
            current->free = 0;
            return (void*)((char*)current + sizeof(block_t));
        }
        current = current->next;
    }
    return 0;
}

void kfree(void *ptr) {
    if (!ptr) return;
    block_t *block = (block_t*)((char*)ptr - sizeof(block_t));
    block->free = 1;
    block_t *current = heap;
    while (current && current->next) {
        if (current->free && current->next->free) {
            current->size += sizeof(block_t) + current->next->size;
            current->next = current->next->next;
        }
        current = current->next;
    }
}
