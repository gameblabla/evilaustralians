/*
   COPYRIGHT (C) 2014-2015 GAMEBLABLA
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#define FPS_VIDEO 60
#define MAX_IMAGE 512

#if defined(DEBUG) || defined(DEBUG_CRAZY)
	#include <stdio.h>
#endif

#include <allegro.h>

volatile unsigned short ticks = 0;
void ticker()
{
	ticks++;
}
END_OF_FUNCTION(ticker)
 

#ifdef SOUND_ENABLED
	#define MAX_SFX 32
	SAMPLE *music;
	SAMPLE *gfx_id[MAX_SFX];
#endif

#ifdef IMAGE_CODEC_ENABLED
#endif

#include "INPUT.h"
struct input BUTTON;

unsigned short done = 0;
char* game_name = "";

BITMAP *sprites_img[MAX_IMAGE];
unsigned short sprites_img_tocopy[MAX_IMAGE];

#define Buttons_UP KEY_UP
#define Buttons_LEFT KEY_LEFT
#define Buttons_RIGHT KEY_RIGHT
#define Buttons_DOWN KEY_DOWN
#define Buttons_A KEY_X
#define Buttons_B KEY_C
#define Buttons_C KEY_V
#define Buttons_D KEY_B
#define Buttons_START KEY_SPACE
#define Buttons_SELECT KEY_BACKSPACE
#define Buttons_QUIT KEY_ESC

/*
	The controller used here
	was an official Xbox 360 controller
*/
#define Joypad_A 0 
#define Joypad_B 1
#define Joypad_C 2
#define Joypad_D 3
#define Joypad_START 7
#define Joypad_SELECT 5 
	
void msleep(unsigned char milisec)
{
}

void Init_video()
{
	allegro_init();
	install_timer();
	install_keyboard(); 
	install_joystick(JOY_TYPE_AUTODETECT);
	request_refresh_rate(FPS_VIDEO);
	set_color_depth(32);
	/*set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 320, 240, 0, 0);*/
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 320, 240, 0, 0);
	set_window_title(game_name);
	clear_to_color(screen, makecol(0, 0, 0));
	LOCK_VARIABLE(ticks);
	LOCK_FUNCTION(ticker);
	install_int_ex(ticker, BPS_TO_TIMER(FPS_VIDEO));
}

void Load_Image(unsigned short a, const char* directory)
{
	#ifdef DEBUG
		fprintf(stderr, "Clearing surface for image %d...\n", a);
	#endif
	
	if (sprites_img[a] != NULL)
	{
		destroy_bitmap(sprites_img[a]);
	}
	
	#ifdef DEBUG
		fprintf(stderr, "Trying to load image %d (%s)...\n", a, directory);
	#endif
	
	sprites_img[a] = load_bitmap(directory, NULL);
	
	#ifdef DEBUG
		fprintf(stderr, "Loading image %d (%s) was successful\n", a, directory);
	#endif
	
	sprites_img_tocopy[a] = 0;

}

void Copy_Image(unsigned short a, unsigned short i)
{
	#ifdef DEBUG
		fprintf(stderr, "Transfering the data of id %d to id %d.\n", a, i);
	#endif
	
	sprites_img_tocopy[i] = a;
}

void Put_image(unsigned short a, short x, short y)
{
	#ifdef DEBUG_CRAZY
		fprintf(stderr, "Put image %d on screen and update its position\n X: %d \n Y: %d\n", a, x ,y);
	#endif

	if (sprites_img_tocopy[a] > 0)
	{
		masked_blit(sprites_img[sprites_img_tocopy[a]], screen, 0, 0, x, y, sprites_img[sprites_img_tocopy[a]]->w, sprites_img[sprites_img_tocopy[a]]->h);
	}
	else
	{
		masked_blit(sprites_img[a], screen, 0, 0, x, y, sprites_img[a]->w, sprites_img[a]->h);
	}
		
}

void Put_sprite(unsigned short a, short x, short y, unsigned short w, unsigned short h, unsigned char f)
{
	#ifdef DEBUG_CRAZY
		fprintf(stderr, "Put sprite %d on screen and update its position\n X: %d \n Y: %d\n Frame: %d\n", a, x ,y, f);
	#endif

	if (sprites_img_tocopy[a] > 0)
	{
		masked_blit(sprites_img[sprites_img_tocopy[a]], screen, f*w, 0, x, y, w, h);
	}
	else
	{
		masked_blit(sprites_img[a], screen, f*w, 0, x, y, w, h);
	}
}

void Clear_screen()
{
    /*clear_to_color(screen, 0);*/
}

void Faster_clearing(short x, short y, unsigned short w, unsigned short h)
{
	rectfill(screen, x, y, x+w, x+y, 0);
}

void Update_video()
{
	/*
	 Sync code by Allegro team, thank you guys !
	 https://wiki.allegro.cc/index.php?title=Using_Timers_in_Allegro_4
	*/
	
	
	vsync();
		
	while(ticks == 0)
	{
		rest(1);	
	}
 
	while(ticks > 0)
	{
		int old_ticks = ticks;
		ticks--;

		if(old_ticks <= ticks)	
			break; 
	}
		
	
}

void Faster_update(short x, short y, short w, short h)
{
	Update_video();
}

void Sync_refresh()
{
	Update_video(); 
}

void Draw_Pixel(unsigned short x, unsigned short y, unsigned char R, unsigned char G, unsigned char B)
{
	int color = makecol(R,G,B);
	rectfill(screen, x, y, x+1, x+1, color);
}

void Controls()
{
		poll_joystick();

		BUTTON.UP = (key[Buttons_UP] || joy[0].stick[0].axis[1].d1)	 ? 1 : 0;
		BUTTON.DOWN = (key[Buttons_DOWN] || joy[0].stick[0].axis[1].d2)	 ? 1 : 0;
		BUTTON.LEFT = (key[Buttons_LEFT] || joy[0].stick[0].axis[0].d1)	 ? 1 : 0;
		BUTTON.RIGHT = (key[Buttons_RIGHT] || joy[0].stick[0].axis[0].d2)	 ? 1 : 0;
		
		BUTTON.A = (key[Buttons_A] || joy[0].button[Joypad_A].b) ? 1 : 0;
		BUTTON.B = (key[Buttons_B] || joy[0].button[Joypad_B].b) ? 1 : 0;
		BUTTON.C = (key[Buttons_C] || joy[0].button[Joypad_C].b) ? 1 : 0;
		BUTTON.D = (key[Buttons_D] || joy[0].button[Joypad_D].b) ? 1 : 0;
		BUTTON.START = (key[Buttons_START] || joy[0].button[Joypad_START].b)	 ? 1 : 0;
		BUTTON.SELECT = (key[Buttons_SELECT] || joy[0].button[Joypad_SELECT].b)	 ? 1 : 0;
		BUTTON.QUIT = key[Buttons_QUIT] ? 1 : 0;
}

void Clear_Image(unsigned short a)
{
	if (sprites_img[a] != NULL)
	{
		destroy_bitmap(sprites_img[a]);
	}
	
	sprites_img_tocopy[a] = 0;
}

void Clear_Images()
{
	unsigned short i;
	
	for (i=0;i<MAX_IMAGE+1;i++) 
	{
		if (sprites_img[i] != NULL)
		{
			destroy_bitmap(sprites_img[i]);
		}
		
		sprites_img_tocopy[i] = 0;
	}
}

#ifdef SOUND_ENABLED

		void Init_sound()
		{
			#ifdef DEBUG
				fprintf(stderr, "Init sound system\n");
			#endif
			install_sound(DIGI_AUTODETECT, 0, NULL);
			set_volume(255, 255);
		}
		

		void Clean_Music()
		{
			if (music)
			{
				destroy_sample(music);
			}
		}

		void Load_Music(const char* directory)
		{
			Clean_Music();
			music = load_wav(directory);
		}

		void Play_Music(char loop)
		{
			play_sample(music, 128, 128, 1000, loop);
		}

		void Load_SFX(unsigned char i, const char* directory)
		{
			#ifdef DEBUG
				fprintf(stderr, "Loading sound effect %d (%s) in memory\n", i, directory);
			#endif
			
			if (gfx_id[i])
			{
				destroy_sample(gfx_id[i]);
			}
			
			gfx_id[i] = load_wav(directory);
		}

		void Play_SFX(unsigned char i)
		{
			#ifdef DEBUG
				fprintf(stderr, "Play sound effect %d loaded in memory\n", i);
			#endif
			
			play_sample(gfx_id[i], 128, 128, 1000, 0);
		}

		void Unload_SFX()
		{
			int i;
			
			#ifdef DEBUG
				fprintf(stderr, "Free sound effect from memory\n");
			#endif
			
			for (i=0;i<MAX_SFX;i++) 
			{
				if (gfx_id[i])
				{
					destroy_sample(gfx_id[i]);
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
		fprintf(stderr, "Clean surface and free Allegro from memory\n");
	#endif

	Clear_Images();

	remove_keyboard();
	allegro_exit();
}
