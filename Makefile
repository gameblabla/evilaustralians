CC = gcc

CFLAGS = -O0 -g -Wall -I./WRAPPER -DSOUND_ENABLED
LDFLAGS = `sdl-config --cflags --libs` -lSDL_mixer
DEFINES = -DSOUND_ENABLED
OUTPUT = violent.elf

SOURCES = src/main.c WRAPPER/PC_SDL12.c
OBJS = ${SOURCES:.c=.o}

${OUTPUT}:${OBJS}
	${CC} -o ${OUTPUT} ${SOURCES} ${CFLAGS} ${LDFLAGS} ${DEFINES} 
	
clean:
	rm src/*.o
	rm WRAPPER/*.o
	rm ${OUTPUT}
