// TRACELESS OS
// Minimal UEFI test

#include <efi.h>
#include <efilib.h>

EFI_STATUS EFIAPI efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *systab) {
    InitializeLib(image, systab);
    
    Print(L"TracelessOS UEFI Bootloader\n");
    Print(L"You were never here.\n");
    
    while(1) {}
    
    return EFI_SUCCESS;
}
