asm
(
    ".section .entry\n\t"
    "xor dh, dh\n\t"
    "push edx\n\t"
    "call main\n\t"
);

#include <drivers/vga_textmode.h>

void main(int boot_drive)
{
    // TODO
    init_vga_text_mode();

    // Infinite loop
    for(;;);
}