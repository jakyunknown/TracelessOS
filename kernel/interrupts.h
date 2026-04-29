// TRACELESS OS
// kernel/interrupts.h
// Interrupt handling

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

// IDT entry structure
typedef struct {
    unsigned short offset_low;
    unsigned short selector;
    unsigned char  zero;
    unsigned char  type_attr;
    unsigned short offset_high;
} __attribute__((packed)) idt_entry_t;

// IDT pointer structure
typedef struct {
    unsigned short limit;
    unsigned int   base;
} __attribute__((packed)) idt_ptr_t;

void interrupts_init();
void idt_set_gate(int n, unsigned int handler);

#endif
