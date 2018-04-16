/*

The MIT License (MIT)

Copyright (c) 2018 Gameblabla

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

*/

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>

SDL_Surface* screen;
SDL_Surface* tmp;
#define RGB565(r, g, b) ((r << 11) + (g << 5) + b)

int main(int argc, char* argv[])
{
	unsigned long long i;
	unsigned long FZ_SIZE;
	unsigned long  red, green, blue;
	unsigned short width, height;
	unsigned short newwidth, newheight;
	unsigned char* data, *newdata;
	FILE* fp = NULL;
	FILE* output = NULL;
	
	if(argc != 3)
	{
		printf("Converter to Falcon RGB565 format (Gameblabla's Custom Bitmap format)\n Enter your pict and the new format name\n");
		return 0;
	}
	
	fp = fopen(argv[1], "rb");
	if (fgetc(fp)!='B' || fgetc(fp)!='M')
	{
		fclose(fp);
		printf("%s is not a bitmap file.\n", argv[1]);
		exit(1);
	}
	
	fseek(fp,18, SEEK_SET);
	fread(&width, sizeof(unsigned short), 1, fp);
	fseek(fp,22, SEEK_SET);
	fread(&height,sizeof(unsigned short), 1, fp);
	fseek(fp, 0, SEEK_SET);
	FZ_SIZE = (width * height) * 2;

	data = (unsigned char *) malloc(FZ_SIZE+4);
	newdata = (unsigned char *) malloc(FZ_SIZE+4);
	fseek(fp, 70, SEEK_SET); //138 with color information, 70 without
	fread(newdata, 1, FZ_SIZE, fp);
	fclose(fp);

	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(width, height, 16, SDL_SWSURFACE);
	memcpy(screen->pixels, newdata, FZ_SIZE);
	
	while(1)
	{
		Uint8 *keystate = SDL_GetKeyState(NULL);
		if (keystate[SDLK_ESCAPE] || keystate[SDLK_SPACE]) break;
		SDL_Event event;
		SDL_Flip(screen);
		SDL_PollEvent(&event);
	}

	/* Handling endianess */
	output = fopen(argv[2],"wb");
	newwidth = bswap_16(width);
	newheight = bswap_16(height);
	fwrite(&newwidth,sizeof(unsigned short),1,output);
	fwrite(&newheight,sizeof(unsigned short),1,output);
	for(i = FZ_SIZE;i>0;i--)
	{
		fwrite(&newdata[(i-1)],1,1,output);
	}
	
	if (output != NULL) fclose(output);
	if (newdata != NULL) free(newdata);
	if (data != NULL) free(data);
	SDL_Quit();
};
