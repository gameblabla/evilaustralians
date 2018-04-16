CC          = vc
DSP 	    = YES
# change compilation / linking flag options
CFLAGS		= -cpu=68030 -O4 -size -const-in-data -sd -no-intz -unroll-all -warn=all -notmpfile

# Files to be compiled
SRC   = ./src/falcsys.c  ./src/graphics.c  ./src/main.c ./src/dma_atari.c ./src/atari_dma_sound.s ./src/falcsysa_asm.s ./src/itoa.c  
ifeq ($(DSP), YES)
SRC  += ./src/mp2_asm.s ./src/mp2.c
PRGNAME     = ev_dsp.prg
endif

ifeq ($(DSP), NO)
CFLAGS  += -DNO_DSP
CFLAGS += -sc
PRGNAME  = ev_nodsp.prg
endif

# Rules to make executable
$(PRGNAME): $(SRC)  
	$(CC) +tos $(CFLAGS) -o $(PRGNAME) $^
clean:
	rm -f $(PRGNAME) *.o
