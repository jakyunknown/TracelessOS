# TRACELESS OS
# Makefile

all: traceless.img

bootloader/boot.bin:
	nasm -f bin bootloader/boot.asm -o bootloader/boot.bin

kernel/stub.o:
	nasm -f elf32 kernel/stub.asm -o kernel/stub.o

kernel/kernel.o:
	gcc -m32 -ffreestanding -fno-builtin -nostdlib -nostdinc \
	    -fno-pie -c kernel/kernel.c -o kernel/kernel.o

kernel/kernel.bin: kernel/stub.o kernel/kernel.o
	ld -m elf_i386 -T kernel/linker.ld \
	   kernel/stub.o kernel/kernel.o \
	   -o kernel/kernel.bin --oformat binary

traceless.img: bootloader/boot.bin kernel/kernel.bin
	cat bootloader/boot.bin kernel/kernel.bin > traceless.img

run: traceless.img
	qemu-system-x86_64 \
	-drive format=raw,file=traceless.img,if=floppy \
	-boot a
clean:
	rm -f bootloader/boot.bin \
	      kernel/stub.o \
	      kernel/kernel.o \
	      kernel/kernel.bin \
	      traceless.img

.PHONY: all run clean
