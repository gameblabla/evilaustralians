/*

The MIT License (MIT)

Copyright (c) 2016 Gameblabla

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

*/

#include <animutils.h>
#include <graphics.h>
#include <getvideoinfo.h>
#include <displayutils.h>
#include <mem.h>
#include <umemory.h>
#include <list.h>
#include <event.h>

#include <Portfolio.h>
#include <event.h>
#include <Init3do.h>
#include <FileFunctions.h>
#include <blockfile.h>
#include <Time.h>
#include <audio.h>
#include <celutils.h>
#include <filesystem.h>
#include <filefunctions.h>
#include <string.h>
#include <operror.h>
#include <controlpad.h>

#define FPS_VIDEO 60
#define MAX_IMAGE 8

#include <burger.h>
#include <audio.h>
#include <types.h>
#include <graphics.h>
#include <effectshandler.h>
#include <soundplayer.h>

#include <music.h>
#include <event.h>
#include <operror.h>
#include <filefunctions.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SFX 16
Item AllSamples[MAX_SFX];			/* Items to sound samples */
Word AllRates[MAX_SFX];
uint32 MainTask;					/* My own task item */
static TagArg SoundRateArgs[] = {
	AF_TAG_SAMPLE_RATE,(void*)0,	/* Get the sample rate */
	TAG_END,0		/* End of the list */
};

#define SCREENS	2

ubyte	*gBackground	= NULL;
ANIM* anim[MAX_IMAGE];
unsigned char already_inmemory[MAX_IMAGE];

static Item vbitem, spitem;
static ScreenContext TheScreen;
static int32 NumFrameBufPages;

#include "INPUT.h"
struct input BUTTON;

unsigned short done = 0;
char* game_name = "";

static const int	sNumControlPads	= 1;

/*  
	3DO
	CONTROLLER KEY MAPPINGS
*/
#define Buttons_UP ControlUp
#define Buttons_LEFT ControlLeft
#define Buttons_RIGHT ControlRight
#define Buttons_DOWN ControlDown
#define Buttons_A ControlA
#define Buttons_B ControlB
#define Buttons_C ControlC
#define Buttons_D ControlLeftShift
#define Buttons_START ControlStart
#define Buttons_SELECT ControlRightShift
#define Buttons_QUIT 0

unsigned char	gCurrentScreen	= 0;

static int32 get_joystick_state()
{
	ControlPadEventData cpaddata;
	cpaddata.cped_ButtonBits=0;
	GetControlPad(1,0,&cpaddata);

	return ( cpaddata.cped_ButtonBits );
}

void msleep(unsigned char milisec)
{
}

void Init_video(char* argv[])
{
	unsigned char i;
	/* Open the graphics folio */
	CreateBasicDisplay(&TheScreen, DI_TYPE_DEFAULT, SCREENS);
	/* Init the math folio (Required) */
	OpenMathFolio();
	
	/* This disables interlacing and uses progressive scan instead */
	/*DisableHAVG(TheScreen.sc_Screens[0]);
	DisableVAVG(TheScreen.sc_Screens[0]);*/
	/* Also disable interlacing for the second screen */
	/*DisableHAVG(TheScreen.sc_Screens[1]);
	DisableVAVG(TheScreen.sc_Screens[1]);*/
	
	/* Init my var */
	NumFrameBufPages=TheScreen.sc_nFrameBufferPages;	
	/* Get IO request item */
	vbitem = GetVBLIOReq();
	/* Get VRAM IO request item */
	spitem = GetVRAMIOReq();
	
    InitControlPad(sNumControlPads);
    
    for(i=0;i<MAX_IMAGE;i++)
    {
		already_inmemory[i] = 0;
	}
}

void Load_Background(char* directory)
{
	if (gBackground != NULL)
	{
		unsigned int iImageSize	= TheScreen.sc_nFrameByteCount;
		FreeMem(gBackground, iImageSize);
		gBackground	= NULL;
	}
    gBackground	= (ubyte *)AllocMem((int)(TheScreen.sc_nFrameByteCount), MEMTYPE_STARTPAGE | MEMTYPE_VRAM | MEMTYPE_CEL);
    SetVRAMPages(spitem, gBackground, 0, TheScreen.sc_nFrameBufferPages, -1);
	LoadImage(directory,  gBackground, (VdlChunk **)NULL, &TheScreen);
}

void Draw_Background()
{
	CopyVRAMPages(spitem, TheScreen.sc_Bitmaps[gCurrentScreen]->bm_Buffer, gBackground, TheScreen.sc_nFrameBufferPages, 0xFFFFFFFF);
}

void Load_Image(unsigned short a, char* directory)
{
	
	/* 
		The 3DO only accepts CEL format files.
		Sprites with Multiple frames must be especially
		converted to CEL ANIM format where Width
		and Height must be definied at convertion time.
	*/
	if (already_inmemory[a] == 1) UnloadAnim(anim[a]);
	anim[a] = LoadAnim(directory, MEMTYPE_CEL);
	already_inmemory[a] = 1;
}

void Copy_Image(unsigned short a, unsigned char i)
{
}

void Put_image(unsigned short a, short x, short y)
{
	DrawAnimCel(anim[a], TheScreen.sc_BitmapItems[gCurrentScreen], x, y, 0, 0);
}

void Put_sprite(unsigned short a, short x, short y, unsigned short w, unsigned short h, unsigned char f)
{
	anim[a]->cur_Frame	= Convert32_F16(f);
	DrawAnimCel(anim[a], TheScreen.sc_BitmapItems[gCurrentScreen], x, y, 0, 0);
}

void Faster_clearing(short x, short y, unsigned short w, unsigned short h)
{
	
}

void Clear_screen()
{

}

void Update_video()
{
	DisplayScreen(TheScreen.sc_Screens[gCurrentScreen],0);
	gCurrentScreen	= 1 - gCurrentScreen;
}

void Faster_update(short x, short y, short w, short h)
{
	Update_video();
}

void Sync_Refresh()
{

}

void Draw_Pixel(unsigned short x, unsigned short y, unsigned char R, unsigned char G, unsigned char B)
{
	//fill_rectangle(x, y, 1, R, G, B);
}

void Draw_Rect(unsigned short x, unsigned short y, unsigned short width, unsigned char R, unsigned char G, unsigned char B)
{
	//fill_rectangle(x, y, width, R, G, B);
}

void Controls()
{
	int32 joybits;
	joybits = get_joystick_state();
	/*uint32	gButtons;
	DoControlPad(1, &gButtons, (ControlUp | ControlDown | ControlLeft | ControlRight));*/

	BUTTON.UP 	= (joybits & Buttons_UP)	 ? 1 : 0;
	BUTTON.DOWN 	= (joybits & Buttons_DOWN)	 ? 1 : 0;
	BUTTON.LEFT 	= (joybits & Buttons_LEFT)	 ? 1 : 0;
	BUTTON.RIGHT 	= (joybits & Buttons_RIGHT) ? 1 : 0;
		
	BUTTON.A 	= (joybits & Buttons_A) ? 1 : 0;
	BUTTON.B 	= (joybits & Buttons_B) ? 1 : 0;
	BUTTON.C 	= (joybits & Buttons_C) ? 1 : 0;
	BUTTON.D 	= (joybits & Buttons_D) ? 1 : 0;
	BUTTON.START 	= (joybits & Buttons_START) ? 1 : 0;
	BUTTON.SELECT 	= (joybits & Buttons_SELECT) ? 1 : 0;	
}

void Clear_Image(unsigned short a)
{
	if (already_inmemory[a] == 1) 
	{
		UnloadAnim(anim[a]);
		already_inmemory[a] = 0;
	}
}

void Clear_Images()
{
	unsigned short i;
	for (i=0;i<MAX_IMAGE;i++)
	{
		if (already_inmemory[i])
		{
			UnloadAnim(anim[i]);
			already_inmemory[i] = 0;
		}
	}
}


void Init_sound()
{
	OpenAudioFolio();
	MainTask = KernelBase->kb_CurrentTask->t.n_Item;	
	InitSoundPlayer("system/audio/dsp/varmono8.dsp",0); 		/* Init memory for the sound player */
	InitMusicPlayer("system/audio/dsp/fixedstereosample.dsp");	/* Init memory for the music player */
}

		
void Clean_Music()
{
	PauseMusic();
}

void Load_Music(const char* directory)
{
}

void Play_Music(int id, char loop)
{
	PlaySong(id);
	PauseMusic();
}

void Load_SFX(unsigned char i, char* directory)
{
	AllSamples[i] = LoadSample(directory);
	GetAudioItemInfo(AllSamples[i],SoundRateArgs);
	AllRates[i] = (Word)(((LongWord)SoundRateArgs[0].ta_Arg)/(44100UL*2UL));	/* Get the DSP rate for the sound */	
}

void Play_SFX(unsigned char i)
{
	StopSound(i+1);
	PlaySound(i+1);
}

void Unload_SFX()
{
}


void Clearing()
{
	Clear_Images();
	Clean_Music();
	Unload_SFX();
	KillControlPad();
}
