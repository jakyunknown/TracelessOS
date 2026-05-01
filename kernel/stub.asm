; TRACELESS OS
; kernel/stub.asm
; 32-bit entry point

[BITS 32]

global _start
extern kernel_main

section .text

_start:
    call kernel_main
    hlt
