CC          = vc
DSP 	    = YES
# change compilation / linking flag options
CFLAGS		= -cpu=68030 -Os -size -const-in-data -sc -sd -unroll-all -no-intz

# Files to be compiled
SRC   = ./src/falcsys.c  ./src/graphics.c  ./src/main.c ./src/mp2.c ./src/falcsysa_asm.s ./src/itoa.c
ifeq ($(DSP), YES)
SRC  += ./src/mp2_asm.s
PRGNAME     = ev_dsp.prg
endif

ifeq ($(DSP), NO)
CFLAGS  += -DNO_DSP
PRGNAME  = ev_nodsp.prg
endif

# Rules to make executable
$(PRGNAME): $(SRC)  
	$(CC) +tos $(CFLAGS) -o $(PRGNAME) $^
clean:
	rm -f $(PRGNAME) *.o
