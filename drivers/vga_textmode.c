#include <stdint.h>
#include <stddef.h>
#include <lib/io.h>
#include <drivers/vga_textmode.h>

void init_vga_text_mode(void)
{
    bytes_out_by_port(0x3d4, 0x0a);
    bytes_out_by_port(0x3d5, 0x20);
}
