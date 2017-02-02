/*

The MIT License (MIT)

Copyright (c) 2016 Gameblabla

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

*/

#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <psppower.h>

/* Define the module info section */
PSP_MODULE_INFO("GAMEBLABLA", 0, 1, 1);
PSP_HEAP_SIZE_MAX();
/* Define the main thread's attribute value (optional) */
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

#include <stdlib.h>
#include <SDL/SDL.h>

#define FPS_VIDEO 60
#define MAX_IMAGE 512

#ifdef SOUND_ENABLED
	#include <SDL/SDL_mixer.h>
	#define MAX_SFX 16
	int music;
	int sfx_id[32];
#endif

#ifdef IMAGE_CODEC_ENABLED
	#include <SDL/SDL_image.h>
#endif

#include "INPUT.h"
struct input BUTTON;

const float real_FPS = 1000/FPS_VIDEO;

int done = 0;
char* game_name = "";

SDL_Surface *sprites_img[MAX_IMAGE];
SDL_Surface *screen;

int exit_callback(int arg1, int arg2, void *common);
int CallbackThread(SceSize args, void *argp);
int SetupCallbacks(void);

#define Buttons_UP PSP_CTRL_UP
#define Buttons_LEFT PSP_CTRL_LEFT
#define Buttons_RIGHT PSP_CTRL_RIGHT
#define Buttons_DOWN PSP_CTRL_DOWN
#define Buttons_A PSP_CTRL_CROSS
#define Buttons_B PSP_CTRL_SQUARE
#define Buttons_C PSP_CTRL_CIRCLE
#define Buttons_D PSP_CTRL_TRIANGLE
#define Buttons_START PSP_CTRL_START
#define Buttons_SELECT PSP_CTRL_SELECT
#define Buttons_QUIT 0


void msleep(unsigned char milisec)
{
	SDL_Delay(milisec);
}

void Init_video(char* argv[])
{
	  SetupCallbacks();
	  sceCtrlSetSamplingCycle(0);
	  sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	  scePowerSetClockFrequency(333, 333, 166);
	  SDL_Init( SDL_INIT_VIDEO );
	  screen = SDL_SetVideoMode(320, 240, 16, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
}

void Load_Image(unsigned short a, const char* directory)
{
	SDL_Surface *tmp;
	
	if (sprites_img[a] != NULL)
	{
		SDL_FreeSurface(sprites_img[a]);
	}

	#ifdef IMAGE_CODEC_ENABLED
		tmp = IMG_Load(directory);
	#else
		tmp = SDL_LoadBMP(directory);
	#endif

	SDL_SetColorKey(sprites_img[a], (SDL_SRCCOLORKEY | SDL_RLEACCEL), SDL_MapRGB(sprites_img[a]->format, 255, 0, 255));
	
}

void Put_image(unsigned short a, short x, short y)
{
	SDL_Rect position;
	position.x = x;
	position.y = y;
	
	#ifdef DEBUG_CRAZY
		fprintf(stderr, "Put image %d on screen and update its position\n X: %d \n Y: %d\n", a, x ,y);
	#endif

	SDL_BlitSurface(sprites_img[a], NULL, screen, &position);
}

void Put_sprite(unsigned short a, short x, short y, unsigned short w,unsigned short h, unsigned char f)
{
	SDL_Rect position;
	position.x = x;
	position.y = y;
	
	SDL_Rect frame;
	frame.x = f*w;
	frame.y = 0;
	frame.w = w;
	frame.h = h;
	
	#ifdef DEBUG_CRAZY
		fprintf(stderr, "Put sprite %d on screen and update its position\n X: %d \n Y: %d\n Frame: %d\n", a, x ,y, f);
	#endif

	SDL_BlitSurface(sprites_img[a], &frame, screen, &position);
}

void Clear_screen()
{
	#ifdef DEBUG_CRAZY
		fprintf(stderr, "Screen was cleared.\n");
	#endif
	
    SDL_FillRect(screen, 0, 0);
}


void Update_video()
{
	#ifdef UNCAPPED
		SDL_Flip(screen);
	#else
		Uint32 start;
		start = SDL_GetTicks();
		SDL_Flip(screen);
		if(real_FPS > SDL_GetTicks()-start) msleep(real_FPS-(SDL_GetTicks()-start));
	#endif
}

void Faster_update(short x, short y, short w, short h)
{
	#ifdef UNCAPPED
		SDL_UpdateRect(screen, x, y, w, h);
	#else
		Uint32 start;
		start = SDL_GetTicks();
		SDL_UpdateRect(screen, x, y, w, h);
		if(real_FPS > SDL_GetTicks()-start) msleep(real_FPS-(SDL_GetTicks()-start));
	#endif
}

void Faster_clearing(short x, short y, unsigned short w, unsigned short h)
{
	SDL_Rect position;
	position.x = x;
	position.y = y;
	position.w = w;
	position.h = h;
	
	#ifdef DEBUG_CRAZY
		fprintf(stderr, "Screen was cleared.\n");
	#endif
	
    SDL_FillRect(screen, &position, 0);
}


void Sync_refresh()
{
	#ifndef UNCAPPED
		Uint32 start;
		start = SDL_GetTicks();
		if(real_FPS > SDL_GetTicks()-start) msleep(real_FPS-(SDL_GetTicks()-start));
	#endif
}

void Draw_Pixel(unsigned short x, unsigned short y, unsigned char R, unsigned char G, unsigned char B)
{
	SDL_LockSurface(screen);
	
	int color_draw;
	SDL_Rect scr_draw;
	scr_draw.x = x;
	scr_draw.y = y;
	scr_draw.w = 1;
	scr_draw.h = 1;
	
	color_draw = SDL_MapRGB(screen->format, R, G, B);
	
	SDL_FillRect(screen, &scr_draw, color_draw);
	
	SDL_UnlockSurface(screen);
}


void Controls()
{
		SceCtrlData pad;
		sceCtrlPeekBufferPositive(&pad,1);
		
		BUTTON.UP 	= (pad.Buttons & Buttons_UP)	 ? 1 : 0;
		BUTTON.DOWN 	= (pad.Buttons & Buttons_DOWN)	 ? 1 : 0;
		BUTTON.LEFT 	= (pad.Buttons & Buttons_LEFT)	 ? 1 : 0;
		BUTTON.RIGHT 	= (pad.Buttons & Buttons_RIGHT) ? 1 : 0;
		
		BUTTON.A 	= (pad.Buttons & Buttons_A) ? 1 : 0;
		BUTTON.B 	= (pad.Buttons & Buttons_B) ? 1 : 0;
		BUTTON.C 	= (pad.Buttons & Buttons_C) ? 1 : 0;
		BUTTON.D 	= (pad.Buttons & Buttons_D) ? 1 : 0;
		BUTTON.START 	= (pad.Buttons & Buttons_START) ? 1 : 0;
		BUTTON.SELECT 	= (pad.Buttons & Buttons_SELECT) ? 1 : 0;
}

void Clear_Image(unsigned short a)
{
	if (sprites_img[a] != NULL)
	{
		SDL_FreeSurface(sprites_img[a]);
	}
}

void Clear_Images()
{
	short i;

	for (i=0;i<MAX_IMAGE+1;i++)
	{
		if (sprites_img[i] != NULL)
		{
			SDL_FreeSurface(sprites_img[i]);
		}
	}
}


#ifdef SOUND_ENABLED

		void Init_sound()
		{
			#ifdef DEBUG
				fprintf(stderr, "Init sound system\n");
			#endif
			Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024);
			Mix_AllocateChannels(MAX_SFX);
		}
		
		void Clean_Music()
		{
			#ifdef DEBUG
				fprintf(stderr, "Stop and clear music from memory\n");
			#endif
			
			if (music)
			{
				Mix_HaltMusic();
				Mix_FreeMusic(music);
				music = NULL;
			}
		}

		void Load_Music(const char* directory)
		{
			#ifdef DEBUG
				fprintf(stderr, "Load music in memory\n");
			#endif
			music = Mix_LoadMUS(directory);
		}

		void Play_Music(char loop)
		{
			if (loop == 1)
			{
				#ifdef DEBUG
					fprintf(stderr, "Playing music in loop\n");
				#endif
				Mix_PlayMusic(music, -1);
			}
			else
			{
				#ifdef DEBUG
					fprintf(stderr, "Playing music\n");
				#endif
				Mix_PlayMusic(music, 0);
			}
		}

		void Load_SFX(unsigned char i, const char* directory)
		{
			#ifdef DEBUG
				fprintf(stderr, "Load sound effect %d (%s) in memory\n", i, directory);
			#endif
			
			if (sfx_id[i])
			{
				Mix_FreeChunk(sfx_id[i]);
				sfx_id[i] = NULL;
			}
			sfx_id[i] = Mix_LoadWAV(directory);
		}

		void Play_SFX(unsigned char i)
		{
			#ifdef DEBUG
				fprintf(stderr, "Play sound effect %d loaded in memory\n", i);
			#endif
			
			Mix_PlayChannel(-1, sfx_id[i], 0) ;
		}

		void Unload_SFX()
		{
			short i;
			
			#ifdef DEBUG
				fprintf(stderr, "Free sound effect from memory\n");
			#endif
			
			for (i=0;i<MAX_SFX;i++) 
			{
				if (sfx_id[i])
				{
					Mix_FreeChunk(sfx_id[i]);
					sfx_id[i] = NULL;
				}
			}
		}

#else

		void Init_sound()
		{
		}
		
		void Clean_Music()
		{
		}

		void Load_Music(const char* directory)
		{
		}

		void Play_Music(char loop)
		{
		}

		void Load_SFX(unsigned char i, const char* directory)
		{
		}

		void Play_SFX(unsigned char i)
		{
		}

		void Unload_SFX()
		{
		}

#endif


void Clearing()
{
	#ifdef SOUND_ENABLED
		Clean_Music();
	#endif

	#ifdef DEBUG
		fprintf(stderr, "Clean surface and free SDL from memory\n");
	#endif
	
	Clear_Images();

	SDL_FreeSurface(screen);
	
	#ifdef SOUND_ENABLED
		Unload_SFX();
		Mix_CloseAudio();
	#endif
	
	sceKernelExitGame();
	return 0;
}


int exit_callback(int arg1, int arg2, void *common) 
{
	done = 1;
	return 0;
}

int CallbackThread(SceSize args, void *argp) 
{
	int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}

int SetupCallbacks(void) 
{
	int thid = sceKernelCreateThread("CallbackThread", CallbackThread, 0x11, 0xFA0, PSP_THREAD_ATTR_USER, 0);
	if (thid >= 0) sceKernelStartThread(thid, 0, 0);
	return thid;
}
