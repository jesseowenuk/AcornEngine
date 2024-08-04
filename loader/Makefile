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
		-Tlinker.ld

.PHONY: all clean

C_FILES := main.c
OBJ := main.o

all: acorn_loader.img

acorn_loader.img: $(OBJ)
	$(LD) $(LDFLAGS) $(INTERNAL_LDFLAGS) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INTERNAL_CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) acorn_loader.img