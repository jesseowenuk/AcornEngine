CC =  i686-elf-gcc
LD = i686-elf-ld

CFLAGS = -O2 -pipe -Wall -Wextra

INTERNAL_CFLAGS = \
		-m16 \
		-ffreestanding \
		-nostdlib \
		-masm=intel \
		-fno-pic \
		-mno-sse \
		-mno-sse2 \
		-fno-stack-protector \
		-I.

LDFLAGS =

INTERNAL_LDFLAGS = \
		-m elf_i386 \
		-nostdlib \
		-I. \
		-Tloader/linker.ld

.PHONY: all clean

C_FILES := $(shell find ./ -type f -name '*.c')
OBJ := $(C_FILES:.c=.o)

all: acorn_engine.bin

acorn_engine.bin: loader/bootsect/bootsect.bin $(OBJ)
	$(LD) $(LDFLAGS) $(INTERNAL_LDFLAGS) $(OBJ) -o loader/bootsect/stage2.bin
	cat loader/bootsect/bootsect.bin loader/bootsect/stage2.bin > $@

loader/bootsect/bootsect.bin: loader/bootsect/bootsect.asm
	cd loader/bootsect && nasm bootsect.asm -fbin -o bootsect.bin

%.o: %.c
	$(CC) $(CFLAGS) $(INTERNAL_CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) loader/bootsect/bootsect.bin