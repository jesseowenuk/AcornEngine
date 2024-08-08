    ;=======================================================
    ; Read Sector
    ; Reads a disk sector with an LBA address
    ;
    ; IN:
    ; EAX - LBA sector to load
    ; DL - drive number
    ; ES - buffer segment
    ; BX - buffer offset
    ;
    ; OUT:
    ; Carry if error
    ;=======================================================
read_sector:
    push eax                                        ; save the EAX register
    push ebx                                        ; save the EBX register
    push ecx                                        ; save the ECX register
    push edx                                        ; save the EDX register
    push esi                                        ; save the ESI register
    push edi                                        ; save the EDI register

    push es                                         ; save the ES register so
    pop word [.target_segment]                      ; we can save it to .target_segment
    mov word [.target_offset], bx                   ; save the offset from BX to .target_offset
    mov dword [.lba_address_low], eax               ; save the LBA sector to load into .lba_address_low - this is from the EAX register

    xor esi, esi                                    ; zero out the ESI register
    mov si, .da_struct                              ; move the address of the .da_struct (defined below) into the SI (source index) register
    mov ah, 0x42                                    ; set the fuction for int 0x13 

    clc                                             ; clear the carry flag to make sure it's clear before continuing

    int 0x13                                        ; call interrupt 13

    .done:
        pop edi                                     ; restore the EDI register
        pop esi                                     ; restore the ESI register
        pop edx                                     ; restore the EDX register
        pop ecx                                     ; restore the ECX register
        pop ebx                                     ; restore the EBX register
        pop eax                                     ; restore the EAX register  
        ret                                         ; return to calling code

align 4
.da_struct:                                         ; disk packet structure
    .packet_size        db 16                       ; packet size
    .unused             db 0                        ; unused
    .count              dw 1                        ; number of bytes to read
    .target_offset      dw 0                        ; the target offset
    .target_segment     dw 0                        ; the target segment
    .lba_address_low    dd 0                        ; low LBA address
    .lba_address_high   dd 0                        ; high LBA address

    ;=======================================================
    ; Read Sectors
    ; Read multiple sectors via LBA
    ; IN:
    ; EAX - LBA starting sector
    ; DL - Drive number
    ; ES - Buffer segment
    ; EBX - Buffer offset
    ; CX - Sector count
    ;
    ; OUT:
    ; Carry if error
    ;=======================================================
read_sectors:

    push eax                                        ; save the EAX register
    push ebx                                        ; save the EBX register
    push ecx                                        ; save the ECX register
    push edx                                        ; save the EDX register
    push esi                                        ; save the ESI register
    push edi                                        ; save the EDI register

    .loop:

        push es                                     ; save the ES register
        push ebx                                    ; save the EBX register

        mov bx, 0x7000                              ; address to load stage to temporarily
        mov es, bx                                  ; move this address into the ES register
        xor bx, bx                                  ; zero out the BX register

        call read_sector                            ; lets read a sector

        pop ebx                                     ; restore the EBX register
        pop es                                      ; restore the ES register

        jc .done                                    ; if carry flag set - exit with carry (error!)

        push ds                                     ; save the DS register

        mov si, 0x7000                              ; set the SI register to 0x7000
        mov ds, si                                  ; set the DS (Data Segment) register to 0x7000
        mov edi, ebx                                ; move the buffer offset into the ESI register
        xor esi, esi                                ; zero out the ESI register?

        push ecx                                    ; save the ECX register
        mov ecx, 512                                ; move 512 into the ECX register
        a32 o32 rep movsb                           ; move the data from 0x7000 to the target location pointed at by EDI
        pop ecx                                     ; restore the ECX register which will now be pointing at the end of the data to move

        pop ds                                      ; restore the DS register

        inc eax                                     ; increment EAX register to next sector
        add ebx, 512                                ; add 512 to EBX register 

        loop .loop                                  ; loop to read in the next sector and do it al again!

        .done:
            pop edi                                 ; restore the EDI register
            pop esi                                 ; restore the ESI register
            pop edx                                 ; restore the EDX register
            pop ecx                                 ; restore the ECX register
            pop ebx                                 ; restore the EBX register
            pop eax                                 ; restore the EAX register  
            ret                                     ; return to calling code
