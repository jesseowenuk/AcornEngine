#ifndef __VGA_TEXTMODE_H
#define __VGA_TEXTMODE_H

#include <stddef.h>

void init_vga_text_mode(void);
void text_write(const char *, size_t);

#endif