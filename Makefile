CC = gcc

CFLAGS = -O0 -g -Wall -I./WRAPPER
LDFLAGS = -Wl,--as-needed `sdl-config --cflags --libs` -lSDL_mixer 
DEFINES = -DSOUND_ENABLED
OUTPUT = violent

SOURCES = src/main.c WRAPPER/PC_SDL12.c
OBJS = ${SOURCES:.c=.o}

${OUTPUT}:${OBJS}
	${CC} -o ${OUTPUT} ${SOURCES} ${CFLAGS} ${LDFLAGS} ${DEFINES} 
	
clean:
	rm src/*.o
	rm WRAPPER/*.o
	rm ${OUTPUT}
