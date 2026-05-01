// TRACELESS OS
// kernel/interrupts.c
// 64-bit IDT and interrupt handling

#include "interrupts.h"

#define IDT_SIZE 256

static idt_entry_t idt[IDT_SIZE];
static idt_ptr_t   idt_ptr;

extern void idt_load(idt_ptr_t *);
extern void isr0();
extern void isr1();
extern void irq1();

static void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void idt_set_gate(int n, unsigned int handler) {
    idt[n].offset_low  = handler & 0xFFFF;
    idt[n].selector    = 0x08;
    idt[n].zero1       = 0;
    idt[n].type_attr   = 0x8E;
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

void pic_init() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0xFD);
    outb(0xA1, 0xFF);
}

void interrupts_init() {
    idt_ptr.limit = (sizeof(idt_entry_t) * IDT_SIZE) - 1;
    idt_ptr.base  = (unsigned long)&idt;

    for (int i = 0; i < IDT_SIZE; i++) {
        idt_set_gate(i, 0);
    }

    pic_init();

    idt_set_gate(0,  (unsigned long)isr0);
    idt_set_gate(1,  (unsigned long)isr1);
    idt_set_gate(33, (unsigned long)irq1);

    idt_load(&idt_ptr);
}

void isr_handler(int num) {
    (void)num;
}
