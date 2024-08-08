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

    mov si, stage2_message
    call print_string_16

    mov ax, 1                                           ; starting sector
    mov ebx, 0x7e00                                     ; the buffer offset
    mov cx, 7                                           ; number of sectors to read
    call read_sectors

    jc error

error:
    mov si, error_message
    call print_string_16

halt:
    cli
    hlt

    ;=======================================================
    ; Includes
    ;=======================================================

    %include "print_string_16.asm"
    %include "read_disk.asm"

    ;=======================================================
    ; Data
    ;=======================================================

loading_message db 13, 10, '<Acorn>', 13, 10, 10, 0
stage2_message db 'stage1: Loading stage2...', 0
error_message db 13, 10, 'Error, system halted.', 0
drive_number db 0

    ;=======================================================
    ; Padding and magic number
    ;=======================================================
    times 0x1b8 - ($-$$) db 0
    times 510 - ($-$$) db 0
    dw 0xaa55