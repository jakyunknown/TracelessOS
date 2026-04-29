; TRACELESS OS
; kernel/interrupts.asm
; Interrupt service routines

[BITS 32]

extern isr_handler

global idt_load
global isr0
global isr1

; Load IDT
idt_load:
    mov eax, [esp+4]
    lidt [eax]
    ret

; ISR 0 — Division by zero
isr0:
    cli
    push byte 0
    push byte 0
    call isr_handler
    add esp, 8
    sti
    iret

; ISR 1 — Debug
isr1:
    cli
    push byte 0
    push byte 1
    call isr_handler
    add esp, 8
    sti
    iret
