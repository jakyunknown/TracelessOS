# TRACELESS OS
# Makefile — BIOS build

all: traceless.img

bootloader/boot.bin:
	nasm -f bin bootloader/boot.asm -o bootloader/boot.bin

bootloader/stage2.bin:
	nasm -f bin bootloader/stage2.asm -o bootloader/stage2.bin

kernel/stub.o:
	nasm -f elf32 kernel/stub.asm -o kernel/stub.o

kernel/kernel.o:
	gcc -m32 -ffreestanding -fno-builtin -nostdlib -nostdinc \
	    -fno-pie -c kernel/kernel.c -o kernel/kernel.o

kernel/memory.o:
	gcc -m32 -ffreestanding -fno-builtin -nostdlib -nostdinc \
	    -fno-pie -c kernel/memory.c -o kernel/memory.o

kernel/interrupts.o:
	gcc -m32 -ffreestanding -fno-builtin -nostdlib -nostdinc \
	    -fno-pie -c kernel/interrupts.c -o kernel/interrupts.o

kernel/interrupts_asm.o:
	nasm -f elf32 kernel/interrupts.asm -o kernel/interrupts_asm.o

kernel/keyboard.o:
	gcc -m32 -ffreestanding -fno-builtin -nostdlib -nostdinc \
	    -fno-pie -c kernel/keyboard.c -o kernel/keyboard.o

kernel/kernel.bin: kernel/stub.o kernel/kernel.o kernel/memory.o \
                   kernel/interrupts.o kernel/interrupts_asm.o kernel/keyboard.o
	ld -m elf_i386 -T kernel/linker.ld \
	   kernel/stub.o kernel/kernel.o kernel/memory.o \
	   kernel/interrupts.o kernel/interrupts_asm.o \
	   kernel/keyboard.o \
	   -o kernel/kernel.bin --oformat binary

traceless.img: bootloader/boot.bin bootloader/stage2.bin kernel/kernel.bin
	dd if=/dev/zero of=traceless.img bs=512 count=2880
	dd if=bootloader/boot.bin of=traceless.img conv=notrunc
	dd if=bootloader/stage2.bin of=traceless.img bs=512 seek=1 conv=notrunc
	dd if=kernel/kernel.bin of=traceless.img bs=512 seek=5 conv=notrunc

run: traceless.img
	qemu-system-x86_64 \
	-drive format=raw,file=traceless.img,if=floppy \
	-boot a \
	-m 32M \
	-device isa-debug-exit,iobase=0x604,iosize=0x04

clean:
	rm -f bootloader/boot.bin \
	      bootloader/stage2.bin \
	      kernel/stub.o \
	      kernel/kernel.o \
	      kernel/memory.o \
	      kernel/interrupts.o \
	      kernel/interrupts_asm.o \
	      kernel/keyboard.o \
	      kernel/kernel.bin \
	      traceless.img

.PHONY: all run clean
