#include <stdint.h>
#include <stddef.h>
#include <lib/io.h>
#include <drivers/vga_textmode.h>

#define VGA_BUFFER_END ((VGA_ROWS * VGA_COLUMNS) - 1)
#define VGA_COLUMNS (80 * 2)
#define VGA_ROWS 25

static char* video_memory = (char*)0xB8000;

static uint8_t text_palette = 0x07;

static void vga_clear_screen()
{
    for(size_t i = 0; i < VGA_BUFFER_END; i += 2)
    {
        video_memory[i] = ' ';
        video_memory[i + 1] = text_palette;
    }

    return;
}

void init_vga_text_mode(void)
{
    bytes_out_by_port(0x3d4, 0x0a);
    bytes_out_by_port(0x3d5, 0x20);
    vga_clear_screen();
}
