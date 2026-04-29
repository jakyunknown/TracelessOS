; TRACELESS OS
; kernel/stub.asm
; Hands control from bootloader to C kernel

[BITS 32]

extern kernel_main      ; tell assembler kernel_main is in C

global _start

_start:
    call kernel_main    ; jump into C
    hlt                 ; if C returns, halt
