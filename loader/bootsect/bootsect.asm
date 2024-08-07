org 0x7C00
bits 16

code_start:
    cli
    jmp 0x0000:initialise_cs

initialise_cs:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    mov byte [drive_number], dl                         ; save the drive number

    mov si, loading_message
    call print_string_16

    ;=======================================================
    ; Load Stage 2
    ;=======================================================
    

    cli
    hlt

    ;=======================================================
    ; Includes
    ;=======================================================

    %include "print_string_16.asm"

    ;=======================================================
    ; Data
    ;=======================================================

loading_message db 13, 10, '<Acorn>', 13, 10, 10, 0
drive_number db 0

    ;=======================================================
    ; Padding and magic number
    ;=======================================================
    times 0x1b8 - ($-$$) db 0
    times 510 - ($-$$) db 0
    dw 0xaa55