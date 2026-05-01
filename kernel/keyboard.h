// TRACELESS OS
// kernel/keyboard.h
// PS/2 Keyboard driver

#ifndef KEYBOARD_H
#define KEYBOARD_H

void keyboard_init();
char keyboard_getchar();
int  keyboard_haskey();

#endif
