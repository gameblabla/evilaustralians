3DODEV	= C:/3DODev/
ARMDEV	= C:/ARM250/
DEBUG	= 0

# Project specific settings
NAME	= LaunchMe
STACKSIZE = 4096
BANNER	= Banner.bmp
FILESYSTEM	= CD

CC		= $(ARMDEV)bin/armcc
AS 		= $(ARMDEV)bin/armas
LD		= $(ARMDEV)bin/armlink
RM		= $(3DODEV)bin/rm
MODBIN	= $(3DODEV)bin/modbin
MAKEBANNER	= $(3DODEV)bin/MakeBanner

CCFLAGS = -O2 -Wd -bi -apcs /swst/fp -d DEBUG=$(DEBUG) -cpu ARM60
ASFLAGS =
INCPATH	= -I$(3DODEV)includes
LDFLAGS = -reloc -nodebug -remove -ro-base 0x80 
LIBPATH	= $(3DODEV)libs/
STARTUP = $(LIBPATH)cstartup.o
LIBS 	= $(LIBPATH)exampleslib.lib $(LIBPATH)Lib3DO.lib $(LIBPATH)audio.lib $(LIBPATH)music.lib $(LIBPATH)operamath.lib \
	$(LIBPATH)filesystem.lib $(LIBPATH)graphics.lib $(LIBPATH)input.lib $(LIBPATH)clib.lib $(LIBPATH)burger.lib
	
ARMLIB	= $(ARMDEV)lib
ARMINC	= $(ARMDEV)inc

SRC_S		= $(wildcard *.s)
SRC_C		= $(wildcard Source/*.c)

OBJ	+= $(SRC_S:.s=.o)
OBJ	+= $(SRC_C:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) BannerScreen
	$(LD) -dupok -o $(FILESYSTEM)/$(NAME). $(LDFLAGS) $(STARTUP) $(LIBS) $(OBJ)
	$(MODBIN) $(STACKSIZE) CD/$(NAME)

%.o: %.c
	$(CC) $(INCPATH) $(CCFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(INCPATH) $(ASFLAGS) $< -o $@
	
BannerScreen: $(BANNER)
	$(MAKEBANNER) $(BANNER) $(FILESYSTEM)/BannerScreen

clean:
	$(RM) -f $(OBJ)
	$(RM) -f $(FILESYSTEM)/$(NAME)
	$(RM) -f $(FILESYSTEM)/BannerScreen
