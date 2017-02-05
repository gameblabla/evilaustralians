ROOTDIR = /opt/n64
GCCN64PREFIX = $(ROOTDIR)/bin/mips64-elf-
CHKSUM64PATH = $(ROOTDIR)/bin/chksum64
MKDFSPATH = $(ROOTDIR)/bin/mkdfs
HEADERPATH = $(ROOTDIR)/mips64-elf/lib
N64TOOL = $(ROOTDIR)/bin/n64tool
HEADERNAME = header
LINK_FLAGS = -L$(ROOTDIR)/mips64-elf/lib -ldragon -lmikmod -lc -lm -ldragonsys -Tn64ld.x
PROG_NAME = evil_australians
CFLAGS = -std=gnu99 -march=vr4300 -mtune=vr4300 -Ofast -Wall -I$(ROOTDIR)/mips64-elf/include -s
ASFLAGS = -mtune=vr4300 -march=vr4300
CC = $(GCCN64PREFIX)gcc
AS = $(GCCN64PREFIX)as
LD = $(GCCN64PREFIX)ld
OBJCOPY = $(GCCN64PREFIX)objcopy
OBJECTS = src/main.o src/N64.o

$(PROG_NAME).v64: $(PROG_NAME).elf spritemap.dfs
	$(OBJCOPY) $(PROG_NAME).elf $(PROG_NAME).bin -O binary
	rm -f $(PROG_NAME).v64
	$(N64TOOL) -b -l 4M -t "Evil Australians" -h $(HEADERPATH)/$(HEADERNAME) -o $(PROG_NAME).v64 $(PROG_NAME).bin -s 1M spritemap.dfs
	$(CHKSUM64PATH) $(PROG_NAME).v64

$(PROG_NAME).elf : $(OBJECTS)
	$(LD) -o $(PROG_NAME).elf $(OBJECTS) $(LINK_FLAGS)

spritemap.dfs:
	$(MKDFSPATH) spritemap.dfs ./filesystem/

all: $(PROG_NAME).v64

clean:
	rm -f *.v64 *.elf src/*.o *.bin *.dfs
