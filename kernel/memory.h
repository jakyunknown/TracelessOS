// TRACELESS OS
// kernel/memory.h
// Memory manager

#ifndef MEMORY_H
#define MEMORY_H

#define HEAP_START  0x100000    // 1MB mark — where our heap begins
#define HEAP_SIZE   0x100000    // 1MB of heap space

void memory_init();
void *kmalloc(unsigned int size);
void kfree(void *ptr);
void memory_dump();

#endif
