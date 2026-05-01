#ifndef INTERRUPTS_H
#define INTERRUPTS_H

typedef struct {
    unsigned short offset_low;
    unsigned short selector;
    unsigned char  zero1;
    unsigned char  type_attr;
    unsigned short offset_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    unsigned short limit;
    unsigned int   base;
} __attribute__((packed)) idt_ptr_t;

void interrupts_init();
void idt_set_gate(int n, unsigned int handler);

#endif
