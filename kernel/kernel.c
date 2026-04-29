// TRACELESS OS
// kernel/kernel.c
// Main kernel — written in C

// VGA text buffer
#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH   80
#define VGA_HEIGHT  25

#include "memory.h"

// Colors
#define BLACK   0
#define GREEN   2
#define RED     4
#define WHITE   15

// Current cursor position
static int cursor_x = 0;
static int cursor_y = 0;

// PUT CHARACTER ON SCREEN
void put_char(char c, int color, int x, int y) {
    unsigned short *vga = (unsigned short*)VGA_ADDRESS;
    unsigned short entry = (unsigned short)c | (unsigned short)(color << 8);
    vga[y * VGA_WIDTH + x] = entry;
}

// CLEAR SCREEN
void clear_screen() {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            put_char(' ', BLACK, x, y);
        }
    }
    cursor_x = 0;
    cursor_y = 0;
}

// PRINT STRING
void print(const char *str, int color) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            cursor_x = 0;
            cursor_y++;
        } else {
            put_char(str[i], color, cursor_x, cursor_y);
            cursor_x++;
            if (cursor_x >= VGA_WIDTH) {
                cursor_x = 0;
                cursor_y++;
            }
        }
    }
}

// PRINT A FULL ROW BAR
void print_bar(int color) {
    for (int x = 0; x < VGA_WIDTH; x++) {
        put_char('=', color, x, cursor_y);
    }
    cursor_y++;
    cursor_x = 0;
}

// PRINT NUMBER
void print_int(int n, int color) {
    if (n < 0) {
        print("-", color);
        n = -n;
    }
    if (n == 0) {
        print("0", color);
        return;
    }
    char buf[32];
    int i = 0;
    while (n > 0) {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }
    // Reverse
    char result[32];
    for (int j = 0; j < i; j++) {
        result[j] = buf[i - j - 1];
    }
    result[i] = '\0';
    print(result, color);
}


// PRINT HEX
void print_hex(unsigned int n, int color) {
    char hex_chars[] = "0123456789ABCDEF";
    char buf[9];
    buf[8] = '\0';
    for (int i = 7; i >= 0; i--) {
        buf[i] = hex_chars[n & 0xF];
        n >>= 4;
    }
    print("0x", color);
    print(buf, color);
}

// PRINT NEWLINE
void println(const char *str, int color) {
    print(str, color);
    cursor_x = 0;
    cursor_y++;
}

// KERNEL MAIN
void kernel_main() {
    clear_screen();

    // Start at row 0
    cursor_x = 0;
    cursor_y = 0;

    print_bar(GREEN);

    cursor_y = 1;
    cursor_x = 0;
    print("  TRACELESS OS  //  You were never here.\n", GREEN);

    cursor_y = 2;
    cursor_x = 0;
    print("  Privacy-hardened. Anti-forensic. Armed.\n", WHITE);

    cursor_y = 3;
    cursor_x = 0;
    print_bar(GREEN);

    cursor_y = 5;
    cursor_x = 0;
    print("  [ OK ] Kernel loaded in protected mode\n", GREEN);

    cursor_y = 6;
    cursor_x = 0;
    print("  [ OK ] Memory initialized\n", GREEN);

    cursor_y = 7;
    cursor_x = 0;
    print("  [ OK ] Anti-forensic layer active\n", GREEN);

    cursor_y = 8;
    cursor_x = 0;
    print("  [ !! ] All network traffic routed through Tor\n", RED);

     cursor_y = 10;
    cursor_x = 0;
    print("  Kernel address: ", WHITE);
    print_hex(0x8000, GREEN);

    cursor_y = 11;
    cursor_x = 0;
    print("  VGA address:    ", WHITE);
    print_hex(0xB8000, GREEN);

    cursor_y = 12;
    cursor_x = 0;
    print("  Sectors loaded: ", WHITE);
    print_int(32, GREEN);

    // Init memory
    memory_init();

    cursor_y = 14;
    cursor_x = 0;
    print("  [ OK ] Memory manager initialized", GREEN);

    cursor_y = 15;
    cursor_x = 0;
    print("  Heap start: ", WHITE);
    print_hex(0x100000, GREEN);

    cursor_y = 16;
    cursor_x = 0;
    print("  Heap size:  ", WHITE);
    print_hex(0x100000, GREEN);
    print(" (1MB)", WHITE);

    while(1) {}
}
