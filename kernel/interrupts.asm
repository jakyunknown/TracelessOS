; TRACELESS OS
; kernel/interrupts.asm
; 64-bit interrupt handlers

[BITS 64]

extern keyboard_handler
extern isr_handler

global idt_load
global isr0
global isr1
global irq1

idt_load:
    lidt [rdi]
    ret

isr0:
    cli
    push rax
    mov rdi, 0
    call isr_handler
    pop rax
    sti
    iret

isr1:
    cli
    push rax
    mov rdi, 1
    call isr_handler
    pop rax
    sti
    iret

irq1:
    cli
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi

    call keyboard_handler

    mov al, 0x20
    out 0x20, al

    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    sti
    iret
