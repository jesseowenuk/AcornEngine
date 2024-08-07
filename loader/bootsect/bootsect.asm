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

    cli
    hlt

    ;=======================================================
    ; Data
    ;=======================================================

drive_number db 0

    ;=======================================================
    ; Padding and magic number
    ;=======================================================
    times 0x1b8 - ($-$$) db 0
    times 510 - ($-$$) db 0
    dw 0xaa55