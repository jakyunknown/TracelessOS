// TRACELESS OS
// kernel/keyboard.c
// PS/2 keyboard driver

#include "keyboard.h"

// Keyboard buffer
#define BUFFER_SIZE 256
static char buffer[BUFFER_SIZE];
static int  buf_head = 0;
static int  buf_tail = 0;

// Scancode to ASCII map
static char scancode_map[] = {
    0, 0, '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', '\n', 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*',
    0, ' ', 0
};

// Shift scancode map
static char scancode_shift[] = {
    0, 0, '!', '@', '#', '$', '%', '^',
    '&', '*', '(', ')', '_', '+', '\b', '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
    'O', 'P', '{', '}', '\n', 0, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
    '"', '~', 0, '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', 0, '*',
    0, ' ', 0
};

static int shift_pressed = 0;

// Read from port
static unsigned char inb(unsigned short port) {
    unsigned char val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}


// KEYBOARD INTERRUPT HANDLER
// Called from interrupts.asm
void keyboard_handler() {
    // Wait for keyboard buffer
    if (!(inb(0x64) & 0x01)) return;
    
    unsigned char scancode = inb(0x60);
    // Shift pressed
    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = 1;
        return;
    }
    // Shift released
    if (scancode == 0xAA || scancode == 0xB6) {
        shift_pressed = 0;
        return;
    }

    // Key released — ignore
    if (scancode & 0x80) return;

    // Get ASCII
    char c = 0;
    if (scancode < sizeof(scancode_map)) {
        c = shift_pressed ? scancode_shift[scancode] : scancode_map[scancode];
    }

    if (c) {
        buffer[buf_head] = c;
        buf_head = (buf_head + 1) % BUFFER_SIZE;
    }
}

// CHECK IF KEY AVAILABLE
int keyboard_haskey() {
    return buf_head != buf_tail;
}

// GET CHARACTER
char keyboard_getchar() {
    while (!keyboard_haskey()) {}
    char c = buffer[buf_tail];
    buf_tail = (buf_tail + 1) % BUFFER_SIZE;
    return c;
}

// INIT KEYBOARD
void keyboard_init() {
    // Keyboard uses IRQ1 = interrupt 33
    // Will be set up in interrupts.c
}
