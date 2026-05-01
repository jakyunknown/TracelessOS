ARCH = x86_64
CC = gcc
LD = ld

EFIINC = /usr/include/efi
EFIINCS = -I$(EFIINC) -I$(EFIINC)/$(ARCH) -I$(EFIINC)/protocol
EFILIB = /usr/lib

EFI_CRT_OBJS = $(EFILIB)/crt0-efi-$(ARCH).o
EFI_LDS = $(EFILIB)/elf_$(ARCH)_efi.lds

CFLAGS = $(EFIINCS) \
         -fno-stack-protector \
         -fpic \
         -fshort-wchar \
         -mno-red-zone \
         -maccumulate-outgoing-args \
         -Wall \
         -Wextra \
         -std=c11 \
         -DGNU_EFI_USE_MS_ABI \
         -DEFI_FUNCTION_WRAPPER

LDFLAGS = -nostdlib \
          -znocombreloc \
          -T $(EFI_LDS) \
          -shared \
          -Bsymbolic \
          -L $(EFILIB) \
          -l:libgnuefi.a \
          -l:libefi.a \
          $(EFI_CRT_OBJS)

all: BOOTX64.EFI

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

bootloader.so: main.o
	$(LD) $(LDFLAGS) main.o -o bootloader.so

BOOTX64.EFI: bootloader.so
	objcopy \
	-j .text \
	-j .sdata \
	-j .data \
	-j .dynamic \
	-j .dynsym \
	-j .rel \
	-j .rela \
	-j .reloc \
	--target=efi-app-$(ARCH) \
	bootloader.so BOOTX64.EFI

clean:
	rm -f main.o bootloader.so BOOTX64.EFI
