// TRACELESS OS
// kernel/memory.c
// Simple kernel memory allocator

#include "memory.h"

// Memory block header
typedef struct block {
    unsigned int size;
    int free;
    struct block *next;
} block_t;

// Start of heap
static block_t *heap = (block_t*)HEAP_START;
static int initialized = 0;

// INIT MEMORY
void memory_init() {
    heap->size = HEAP_SIZE - sizeof(block_t);
    heap->free = 1;
    heap->next = 0;
    initialized = 1;
}

// ALLOCATE MEMORY
void *kmalloc(unsigned int size) {
    if (!initialized) memory_init();

    block_t *current = heap;

    while (current) {
        if (current->free && current->size >= size) {
            // Split block if big enough
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
    return 0; // Out of memory
}

// FREE MEMORY
void kfree(void *ptr) {
    if (!ptr) return;

    block_t *block = (block_t*)((char*)ptr - sizeof(block_t));
    block->free = 1;

    // Merge adjacent free blocks
    block_t *current = heap;
    while (current && current->next) {
        if (current->free && current->next->free) {
            current->size += sizeof(block_t) + current->next->size;
            current->next = current->next->next;
        }
        current = current->next;
    }
}

// MEMORY DUMP
// Shows memory usage
void memory_dump() {
    block_t *current = heap;
    int total = 0;
    int used = 0;
    int free = 0;

    while (current) {
        total += current->size;
        if (current->free) {
            free += current->size;
        } else {
            used += current->size;
        }
        current = current->next;
    }

    // These will print via kernel.c functions
    (void)total;
    (void)used;
    (void)free;
}
