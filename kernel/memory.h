// TRACELESS OS
// kernel/memory.h
// 64-bit memory manager

#ifndef MEMORY_H
#define MEMORY_H

#define HEAP_START  0x200000
#define HEAP_SIZE   0x100000

void  memory_init();
void *kmalloc(unsigned long size);
void  kfree(void *ptr);

#endif
