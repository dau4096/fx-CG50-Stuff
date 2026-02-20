CC=/usr/local/cross/bin/sh3eb-elf-gcc
CFLAGS=-I$(HOME)/libfxcg/include -O2 -Wall -fno-lto
LDFLAGS=-L$(HOME)/libfxcg/lib -lfxcg
LKR=$(HOME)/libfxcg/toolchain/prizm.x

all: prgm.g3a

prgm.bin: main.o
	$(CC) main.o \
	-L$(HOME)/libfxcg/lib -lfxcg \
	-T $(LKR) \
	-nostdlib -nostartfiles \
	-o prgm.bin

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

prgm.g3a: prgm.bin
	mkg3a prgm.bin

clean:
	rm -f *.o *.bin *.g3a
