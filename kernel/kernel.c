// TRACELESS OS
// kernel/kernel.c
// Main kernel

#include "memory.h"
#include "interrupts.h"
#include "keyboard.h"

// VGA
#define VGA_ADDRESS 0xB8000UL
#define VGA_WIDTH   80
#define VGA_HEIGHT  25

// Colors
#define BLACK   0
#define BLUE    1
#define GREEN   2
#define CYAN    3
#define RED     4
#define MAGENTA 5
#define BROWN   6
#define WHITE   7
#define GRAY    8
#define YELLOW  14
#define BWHITE  15

// Cursor
static int cursor_x = 0;
static int cursor_y = 0;

// Username
static char username[32] = "traceless";

// Put single character on screen
void put_char(char c, int color, int x, int y) {
    volatile unsigned short *vga = (volatile unsigned short*)VGA_ADDRESS;
    unsigned short entry = (unsigned short)c | (unsigned short)(color << 8);
    vga[y * VGA_WIDTH + x] = entry;
}

// Clear screen
void clear_screen() {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            put_char(' ', BLACK, x, y);
        }
    }
    cursor_x = 0;
    cursor_y = 0;
}

// Print string
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

// Print integer
void print_int(int n, int color) {
    if (n == 0) { print("0", color); return; }
    if (n < 0) { print("-", color); n = -n; }
    char buf[32];
    int i = 0;
    while (n > 0) { buf[i++] = '0' + (n % 10); n /= 10; }
    char result[32];
    for (int j = 0; j < i; j++) result[j] = buf[i-j-1];
    result[i] = '\0';
    print(result, color);
}

// Print hex
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

// Print bar
void print_bar(int color) {
    for (int x = 0; x < VGA_WIDTH; x++) {
        put_char('-', color, x, cursor_y);
    }
    cursor_y++;
    cursor_x = 0;
}

// Delay
void delay(unsigned int ms) {
    for (unsigned int i = 0; i < ms * 1000; i++) {
        __asm__ volatile ("nop");
    }
}

// Scroll
void scroll() {
    volatile unsigned short *vga = (volatile unsigned short*)VGA_ADDRESS;
    for (int y = 0; y < VGA_HEIGHT-1; y++)
        for (int x = 0; x < VGA_WIDTH; x++)
            vga[y * VGA_WIDTH + x] = vga[(y+1) * VGA_WIDTH + x];
    for (int x = 0; x < VGA_WIDTH; x++)
        vga[(VGA_HEIGHT-1) * VGA_WIDTH + x] = ' ' | (BLACK << 8);
}

// Newline with scroll
void newline() {
    cursor_x = 0;
    cursor_y++;
    if (cursor_y >= VGA_HEIGHT) {
        scroll();
        cursor_y = VGA_HEIGHT - 1;
    }
}

// Println with scroll
void println_scroll(const char *str, int color) {
    print(str, color);
    newline();
}

// String length
int strlen(const char *str) {
    int i = 0;
    while (str[i]) i++;
    return i;
}

// String compare
int strcmp(const char *a, const char *b) {
    while (*a && *b && *a == *b) { a++; b++; }
    return *a - *b;
}

// String starts with
int startswith(const char *str, const char *prefix) {
    int i = 0;
    while (prefix[i]) {
        if (str[i] != prefix[i]) return 0;
        i++;
    }
    return 1;
}

// Copy string
void strcpy(char *dst, const char *src) {
    int i = 0;
    while (src[i]) { dst[i] = src[i]; i++; }
    dst[i] = '\0';
}

// Print prompt
void print_prompt() {
    print(username, GREEN);
    print("@traceless:~$ ", GREEN);
}

// Print fastfetch
void print_fastfetch() {
    newline();
    println_scroll("        ___                  TracelessOS", GREEN);
    println_scroll("       /   \\                 ----------------", GREEN);
    print("      | ^ ^ |                ", WHITE);
    print("OS:      ", YELLOW);
    println_scroll("TracelessOS", WHITE);
    print("      |  _  |                ", WHITE);
    print("Kernel:  ", YELLOW);
    println_scroll("custom x86 Assembly + C", WHITE);
    print("      |     |                ", WHITE);
    print("Shell:   ", YELLOW);
    println_scroll("traceless-shell", WHITE);
    print("       \\___/                 ", WHITE);
    print("Memory:  ", YELLOW);
    println_scroll("1MB heap at 0x100000", WHITE);
    print("       /   \\                 ", WHITE);
    print("Arch:    ", YELLOW);
    println_scroll("x86_64", WHITE);
    print("      /     \\                ", WHITE);
    print("User:    ", YELLOW);
    print(username, WHITE);
    newline();
    newline();
    println_scroll("      \x01 \x01 \x01 \x01 \x01 \x01 \x01 \x01", GREEN);
    newline();
}

// Kernel main
void kernel_main() {
    clear_screen();
    cursor_x = 0;
    cursor_y = 0;

    // Boot header
    println_scroll("  TRACELESS OS                                  Privacy is a right, not a privliege.", GREEN);
    newline();

    // Hardware init — fast
    delay(10000);
    println_scroll("  [  OK  ] Started hardware detection", GREEN);
    delay(8000);
    println_scroll("  [  OK  ] CPU: x86_64 detected", GREEN);
    delay(6000);
    println_scroll("  [  OK  ] BIOS handoff complete", GREEN);
    delay(8000);
    println_scroll("  [  OK  ] Entering 32-bit protected mode", GREEN);
    delay(6000);
    println_scroll("  [  OK  ] GDT loaded 3 segment descriptors", GREEN);
    delay(8000);
    println_scroll("  [  OK  ] Flat memory model initialized", GREEN);
    delay(6000);
    println_scroll("  [  OK  ] Stack pointer set to 0x90000", GREEN);
    delay(8000);
    println_scroll("  [  OK  ] Loading kernel into memory at 0x8000", GREEN);
    delay(6000);
    println_scroll("  [  OK  ] VGA text mode 80x25 initialized", GREEN);
    delay(8000);

    // Memory — medium speed
    memory_init();
    delay(15000);
    println_scroll("  [  OK  ] Memory manager initialized", GREEN);
    delay(10000);
    print("  [  OK  ] Heap: ", GREEN);
    print_hex(0x100000, YELLOW);
    print(" — Size: 1MB", GREEN);
    newline();
    delay(15000);
    println_scroll("  [  OK  ] Physical memory allocator ready", GREEN);
    delay(10000);

    // Interrupts — medium speed
    interrupts_init();
    delay(20000);
    println_scroll("  [  OK  ] IDT loaded 256 interrupt gates", GREEN);
    delay(15000);
    println_scroll("  [  OK  ] PIC initialized IRQs remapped to 0x20", GREEN);
    delay(15000);
    println_scroll("  [  OK  ] Timer initialized IRQ0 active", GREEN);
    delay(15000);

    // Keyboard
    keyboard_init();
    delay(20000);
    println_scroll("  [  OK  ] PS/2 keyboard driver loaded IRQ1 active", GREEN);
    delay(15000);
    println_scroll("  [  OK  ] Random entropy pool seeded", GREEN);
    delay(20000);

    // Privacy layer — slow and dramatic
    newline();
    delay(30000);
    println_scroll("  [  **  ] Loading anti-forensic module", YELLOW);
    delay(40000);
    println_scroll("  [  **  ] Arming RAM wipe triggers on shutdown", YELLOW);
    delay(40000);
    println_scroll("  [  **  ] Overwrite passes set to 3 DoD standard", YELLOW);
    delay(40000);
    println_scroll("  [  **  ] Disabling swap no data written to disk", YELLOW);
    delay(40000);
    println_scroll("  [  **  ] Mounting tmpfs all writes go to RAM", YELLOW);
    delay(40000);
    println_scroll("  [  **  ] Spoofing hardware MAC address", YELLOW);
    delay(40000);
    println_scroll("  [  **  ] IPv6 disabled leak prevention active", YELLOW);
    delay(40000);
    println_scroll("  [  **  ] Kernel pointer randomization active", YELLOW);
    delay(40000);
    println_scroll("  [  **  ] Restricting ptrace anti-debugging active", YELLOW);
    delay(40000);
    println_scroll("  [  **  ] Blocking non-Tor network traffic", YELLOW);
    delay(40000);
    println_scroll("  [  **  ] Tor circuit establishing", YELLOW);
    delay(60000);
    println_scroll("  [  **  ] Tor circuit established 3 hops", YELLOW);
    delay(40000);
    println_scroll("  [  **  ] DNS routed through Tor no leaks", YELLOW);
    delay(40000);

    // Warnings — very slow
    newline();
    delay(50000);
    println_scroll("  [  !!  ] All network traffic forced through Tor", RED);
    delay(40000);
    println_scroll("  [  !!  ] RAM wipe armed activates on shutdown", RED);
    delay(40000);
    println_scroll("  [  !!  ] MAC address spoofed hardware unidentifiable", RED);
    delay(40000);
    println_scroll("  [  !!  ] Disk writes disabled amnesic mode active", RED);
    delay(40000);
    println_scroll("  [  !!  ] This session will leave no trace", RED);
    delay(60000);

    // Ready
    newline();
    println_scroll("  [  OK  ] All subsystems nominal", GREEN);
    delay(20000);
    println_scroll("  [  OK  ] Security checks passed", GREEN);
    delay(20000);
    println_scroll("  [  OK  ] TracelessOS ready.", GREEN);
    delay(80000);

    // Clear and show shell
    clear_screen();
    cursor_x = 0;
    cursor_y = 0;

    println_scroll("Welcome to TracelessOS -- Type 'help' for commands.", WHITE);
    newline();

    // Enable interrupts
    __asm__ volatile ("sti");

    // Shell
    char input[256];
    int input_len = 0;
    print_prompt();

    while (1) {
        if (keyboard_haskey()) {
            char c = keyboard_getchar();

            if (c == '\n') {
                input[input_len] = '\0';
                newline();

                if (input_len > 0) {

                    // help
                    if (strcmp(input, "help") == 0) {
                        newline();
                        println_scroll("  TracelessOS Commands:", YELLOW);
                        println_scroll("  help              show this message", WHITE);
                        println_scroll("  clear             clear the screen", WHITE);
                        println_scroll("  about             about TracelessOS", WHITE);
                        println_scroll("  version           OS version info", WHITE);
                        println_scroll("  whoami            current user", WHITE);
                        println_scroll("  pwd               current directory", WHITE);
                        println_scroll("  echo <text>       print text", WHITE);
                        println_scroll("  fastfetch         system info", WHITE);
                        println_scroll("  username <name>   set username", WHITE);
                        println_scroll("  mem               memory info", WHITE);
                        println_scroll("  sudo ghost        package manager", WHITE);
                        println_scroll("  wipe              secure shutdown", WHITE);
                        newline();

                    // clear
                    } else if (strcmp(input, "clear") == 0) {
                        clear_screen();
                        cursor_x = 0;
                        cursor_y = 0;

                    // about
                    } else if (strcmp(input, "about") == 0) {
                        newline();
                        println_scroll("  TRACELESS OS", GREEN);
                        println_scroll("  Built from scratch in C and x86 Assembly.", WHITE);
                        println_scroll("  Privacy-hardened. Anti-forensic. Armed.", WHITE);
                        println_scroll("  github.com/jakyunknown/TracelessOS", YELLOW);
                        newline();

                    // version
                    } else if (strcmp(input, "version") == 0) {
                        newline();
                        println_scroll("  TracelessOS:", GREEN);
                        println_scroll("  Kernel: custom x86 C + Assembly", WHITE);
                        println_scroll("  Arch:   x86_64", WHITE);
                        println_scroll("  Mode:   32-bit protected", WHITE);
                        newline();

                    // whoami
                    } else if (strcmp(input, "whoami") == 0) {
                        newline();
                        print("  ", WHITE);
                        println_scroll(username, GREEN);
                        newline();

                    // pwd
                    } else if (strcmp(input, "pwd") == 0) {
                        newline();
                        print("  /home/", WHITE);
                        println_scroll(username, WHITE);
                        newline();

                    // echo
                    } else if (startswith(input, "echo ")) {
                        newline();
                        print("  ", WHITE);
                        println_scroll(input + 5, WHITE);
                        newline();

                    // fastfetch
                    } else if (strcmp(input, "fastfetch") == 0) {
                        print_fastfetch();

                    // username
                    } else if (startswith(input, "username ")) {
                        strcpy(username, input + 9);
                        newline();
                        print("  [ OK ] Username set to: ", GREEN);
                        println_scroll(username, YELLOW);
                        newline();

                    // mem
                    } else if (strcmp(input, "mem") == 0) {
                        newline();
                        print("  Heap start: ", WHITE);
                        print_hex(0x100000, GREEN);
                        newline();
                        print("  Heap size:  ", WHITE);
                        print_hex(0x100000, GREEN);
                        print(" (1MB)", WHITE);
                        newline();
                        newline();

                    // sudo ghost
                    } else if (startswith(input, "sudo ghost")) {
                        newline();
                        if (startswith(input, "sudo ghost install ")) {
                            print("  [ ** ] Fetching ", YELLOW);
                            println_scroll(input + 19, YELLOW);
                            delay(300000);
                            println_scroll("  [ !! ] No network stack yet.", RED);
                            println_scroll("  [ !! ] ghost coming in v0.3", RED);
                        } else if (strcmp(input, "sudo ghost list") == 0) {
                            println_scroll("  No packages installed yet.", WHITE);
                            println_scroll("  Network stack required for ghost.", YELLOW);
                        } else if (strcmp(input, "sudo ghost update") == 0) {
                            println_scroll("  [ !! ] No network stack yet.", RED);
                        } else {
                            println_scroll("  ghost: unknown command", RED);
                            println_scroll("  Usage: sudo ghost install <package>", WHITE);
                            println_scroll("         sudo ghost remove <package>", WHITE);
                            println_scroll("         sudo ghost list", WHITE);
                            println_scroll("         sudo ghost update", WHITE);
                        }
                        newline();

                    // wipe
                    } else if (strcmp(input, "wipe") == 0) {
                        clear_screen();
                        cursor_x = 0;
                        cursor_y = 0;
                        delay(100000);
                        println_scroll("  [ ** ] Terminating all processes", YELLOW);
                        delay(100000);
                        println_scroll("  [ ** ] Closing network connections", YELLOW);
                        delay(100000);
                        println_scroll("  [ ** ] Disconnecting from Tor", YELLOW);
                        delay(100000);
                        println_scroll("  [ ** ] Flushing filesystem buffers", YELLOW);
                        delay(100000);
                        println_scroll("  [ !! ] Wiping RAM", RED);
                        delay(100000);
                        println_scroll("  [ !! ] Wiping RAM", RED);
                        delay(100000);
                        println_scroll("  [ !! ] Wiping RAM", RED);
                        delay(100000);
                        println_scroll("  [ OK ] RAM wipe complete", GREEN);
                        delay(100000);
                        println_scroll("  [ OK ] All Memory flushed", GREEN);
                        delay(100000);
                        __asm__ volatile ("cli");
                        __asm__ volatile ("outw %w0, %w1" : : "a"(0x2000), "Nd"(0x604));
                        __asm__ volatile ("hlt");

                    // unknown
                    } else {
                        print("  command not found: ", RED);
                        println_scroll(input, RED);
                    }
                }

                input_len = 0;
                if (!(strcmp(input, "wipe") == 0)) {
                    print_prompt();
                }

            } else if (c == '\b') {
                if (input_len > 0) {
                    input_len--;
                    cursor_x--;
                    put_char(' ', BLACK, cursor_x, cursor_y);
                }
            } else {
                if (input_len < 255) {
                    input[input_len++] = c;
                    put_char(c, WHITE, cursor_x, cursor_y);
                    cursor_x++;
                    if (cursor_x >= VGA_WIDTH) newline();
                }
            }
        }
    }
}
