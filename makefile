CC=/usr/local/cross/bin/sh3eb-elf-gcc
CFLAGS=-I$(HOME)/libfxcg/include -I./ -I./src -O2 -Wall -fno-lto -m4-nofpu -std=c99
LDFLAGS = -L$(HOME)/libfxcg/lib -lfxcg -lc -lgcc -nostdlib -nostartfiles
LKR=$(HOME)/libfxcg/toolchain/prizm.x

#All src files
SRCS = main.c src/graphics.c
#Convert src file paths to *.o file paths
OBJS = $(SRCS:.c=.o)

all: prgm.g3a

#Link .o files
prgm.bin: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -T $(LKR) -o prgm.bin

#Compile any .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

#Convert .bin → .g3a
prgm.g3a: prgm.bin
	mkg3a prgm.bin

clean:
	rm -f $(OBJS) *.bin *.g3a