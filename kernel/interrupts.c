// TRACELESS OS
// kernel/interrupts.c
// IDT and interrupt handling

#include "interrupts.h"

#define IDT_SIZE 256

static idt_entry_t idt[IDT_SIZE];
static idt_ptr_t   idt_ptr;

// Defined in interrupts.asm
extern void idt_load(unsigned int);
extern void isr0();
extern void isr1();

// SET IDT GATE
void idt_set_gate(int n, unsigned int handler) {
    idt[n].offset_low  = handler & 0xFFFF;
    idt[n].selector    = 0x08;
    idt[n].zero        = 0;
    idt[n].type_attr   = 0x8E;
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

// INIT INTERRUPTS
void interrupts_init() {
    idt_ptr.limit = (sizeof(idt_entry_t) * IDT_SIZE) - 1;
    idt_ptr.base  = (unsigned int)&idt;

    // Clear IDT
    for (int i = 0; i < IDT_SIZE; i++) {
        idt_set_gate(i, 0);
    }

    // Set up basic handlers
    idt_set_gate(0, (unsigned int)isr0);   // Division by zero
    idt_set_gate(1, (unsigned int)isr1);   // Debug

    // Load IDT
    idt_load((unsigned int)&idt_ptr);
}
