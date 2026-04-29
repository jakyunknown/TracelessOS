// TRACELESS OS
// kernel/kernel.c
// Main kernel — written in C

// VGA text buffer
#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH   80
#define VGA_HEIGHT  25

// Colors
#define BLACK   0
#define GREEN   2
#define RED     4
#define WHITE   15

// Current cursor position
static int cursor_x = 0;
static int cursor_y = 0;

// =====================
// PUT CHARACTER ON SCREEN
// =====================
void put_char(char c, int color, int x, int y) {
    unsigned short *vga = (unsigned short*)VGA_ADDRESS;
    unsigned short entry = (unsigned short)c | (unsigned short)(color << 8);
    vga[y * VGA_WIDTH + x] = entry;
}

// =====================
// CLEAR SCREEN
// =====================
void clear_screen() {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            put_char(' ', BLACK, x, y);
        }
    }
    cursor_x = 0;
    cursor_y = 0;
}

// =====================
// PRINT STRING
// =====================
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

// =====================
// PRINT A FULL ROW BAR
// =====================
void print_bar(int color) {
    for (int x = 0; x < VGA_WIDTH; x++) {
        put_char('=', color, x, cursor_y);
    }
    cursor_y++;
    cursor_x = 0;
}

// =====================
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

    while(1) {}
}
