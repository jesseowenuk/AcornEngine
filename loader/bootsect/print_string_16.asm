    ;=======================================================
    ; Print String 16
    ; Prints a string using BIOS interupts
    ;=======================================================

    ; IN:
    ; SI - this points to a null terminated string to print

print_string_16:
    push ax                             ; save AX register
    push si                             ; save SI register

    mov ah, 0x0E                        ; interrupt 0x10, function 0x0E (Tele-type print)

    .loop:
        lodsb                           ; load the first character and increment SI
        test al, al                     ; is this the null terminating character?
        jz .done                        ; if it is, we're done - jump to our subroutine exit
        int 0x10                        ; call the BIOS to do our bidding
        jmp .loop                       ; back to the beginning of the loop

    .done:
        pop si                          ; restore the SI register
        pop ax                          ; restore the AX register
        ret                             ; return to the calling code