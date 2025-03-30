PREFIX=C:/Users/User/Documents/code/.cpp/PrizmSDK-win-0.6/
CC=$(PREFIX)bin/sh3eb-elf-g++.exe
MKG3A=$(PREFIX)bin/mkg3a.exe
OUTDIR=.
RM=del
CFLAGS=-m4-nofpu -mb -Os -mhitachi -Wall -nostartfiles -I$(PREFIX)include -lfxcg -lgcc -L$(PREFIX)lib -fpermissive
LDFLAGS=$(CFLAGS) -T$(PREFIX)/toolchain/prizm.x -Wl,-static -Wl,-gc-sections

# Specify your source files here
CSOURCES=main.cpp src\\utils.cpp
SHSOURCES=
OBJECTS=$(SHSOURCES:.s=.o) $(CSOURCES:.cpp=.o)

# Specify output binary and G3A files
BIN=$(OUTDIR)/template.bin
ADDIN=$(BIN:.bin=.g3a)

all: $(ADDIN)

$(BIN): $(OBJECTS)
	$(CC) $(addprefix $(OUTDIR)/,$^) $(LDFLAGS) -o $@


$(ADDIN): $(BIN)
	$(MKG3A) -n "[tmp-name]" $< $@

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

%.o: src/%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	del /Q "main.o"
	del /Q "src\\utils.o"
	del /Q "template.bin"
	del /Q "template.g3a"


