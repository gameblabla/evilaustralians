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

/*                
 * ALLEGRO 5 Port is functionnal but still unfinished :
 * Input is laggy (at least on Ubuntu 14.04), no idea why. (perhaps packages are broken)
 * The scaling code is 'hacky'. 
 * Using a float should work, right ?
 * Well with Allegro5, it doesn't.
 * 
 * Could someone enlighten me, please ?
*/

#define FPS_VIDEO 60.0
#define MAX_IMAGE 512

#if defined(DEBUG) || defined(DEBUG_CRAZY)
	#include <stdio.h>
#endif

#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#ifdef SOUND_ENABLED
	#include <allegro5/allegro_audio.h>
	#define MAX_SFX 32
	ALLEGRO_SAMPLE *music;
	ALLEGRO_SAMPLE *gfx_id[MAX_SFX];
#endif

#include "INPUT.h"
struct input BUTTON;

unsigned short done = 0;
char* game_name = " ";

ALLEGRO_DISPLAY *screen;
ALLEGRO_BITMAP *sprites_img[MAX_IMAGE];
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER *timer ;
ALLEGRO_BITMAP *buffer;

unsigned short sprites_img_tocopy[MAX_IMAGE];

float scaleW,scaleH,scaleX,scaleY;

/* 	PC
	CONTROLLER KEY MAPPINGS
*/
	#define Buttons_UP ALLEGRO_KEY_UP
	#define Buttons_LEFT ALLEGRO_KEY_LEFT
	#define Buttons_RIGHT ALLEGRO_KEY_RIGHT
	#define Buttons_DOWN ALLEGRO_KEY_DOWN
	#define Buttons_A ALLEGRO_KEY_X
	#define Buttons_B ALLEGRO_KEY_C
	#define Buttons_C ALLEGRO_KEY_V
	#define Buttons_D ALLEGRO_KEY_B
	#define Buttons_START ALLEGRO_KEY_SPACE
	#define Buttons_SELECT ALLEGRO_KEY_BACKSPACE
	#define Buttons_QUIT ALLEGRO_KEY_ESCAPE
	
void msleep(unsigned char milisec)
{
}

void Init_video()
{
	short i;
	short windowWidth, windowHeight;

	ALLEGRO_MONITOR_INFO info;
	
	al_init();
	al_install_keyboard();
	al_init_image_addon();

	timer = al_create_timer(1.0/FPS_VIDEO);
	
	al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST);

	al_get_monitor_info(0, &info);
	windowWidth = info.x2 - info.x1;
	windowHeight = info.y2 - info.y1;
	
	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW | ALLEGRO_SWAP_METHOD);
	
	screen = al_create_display(windowWidth, windowHeight);
	buffer = al_create_bitmap(320, 240);

	/* 
	FFUUUUU, this doesn't work 
	float sx = windowWidth / 320;
	float sy = windowHeight / 240;
	*/
	
	if (windowWidth>windowHeight)
	{
		for(i=0;scaleH<windowHeight+1;i++)
		{
			/* 
			 * Replace 1.3333... with (4/3) and see the result...
			 * I have no idea why this does not work...
			*/
			scaleW = scaleW + 1.333333333;
			scaleH = scaleH + 1;
		}
		
		scaleH = windowHeight;
	}
	/* This is necessary for handling properly Vertical screens, not tested */
	else
	{
		for(i=0;scaleW<windowWidth+1;i++)
		{
			scaleW = scaleW + 1;
			scaleH = scaleH + 0.75;
		}
		
		scaleW = windowWidth;
	}
	
	scaleX = (windowWidth - scaleW) / 2;
	scaleY = (windowHeight - scaleH) / 2;

	event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(screen));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_flip_display();
    
    al_start_timer(timer);
}

void Load_Image(unsigned short a, const char* directory)
{
	al_destroy_bitmap(sprites_img[a]);
	
	#ifdef DEBUG
		fprintf(stderr, "Trying to load image %d (%s)...\n", a, directory);
	#endif
	
	if (sprites_img[a] != NULL)
	{
		sprites_img[a] = al_load_bitmap(directory);
	}
	
	#ifdef DEBUG
		fprintf(stderr, "Transparency for image...\n");
	#endif
	
	al_convert_mask_to_alpha(sprites_img[a], al_map_rgb(255, 0, 255));
	
	#ifdef DEBUG
		fprintf(stderr, "Loading image %d (%s) was successful\n", a, directory);
	#endif
	
	sprites_img_tocopy[a] = 0;

}

void Copy_Image(unsigned short a, unsigned short i)
{
	#ifdef DEBUG_CRAZY
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
		al_draw_bitmap(sprites_img[sprites_img_tocopy[a]], x, y, 0);
	}
	else
	{
		al_draw_bitmap(sprites_img[a], x, y, 0);
	}
}

void Put_sprite(unsigned short a, short x, short y, unsigned short w, unsigned short h, unsigned char f)
{
	#ifdef DEBUG_CRAZY
		fprintf(stderr, "Put sprite %d on screen and update its position\n X: %d \n Y: %d\n Frame: %d\n", a, x ,y, f);
	#endif
	
	if (sprites_img_tocopy[a] > 0)
	{
		al_draw_bitmap_region(sprites_img[sprites_img_tocopy[a]], f*w, 0, w, h, x, y, 0);
	}
	else
	{
		al_draw_bitmap_region(sprites_img[a], f*w, 0, w, h, x, y, 0);
	}

}

void Clear_screen()
{
	al_set_target_bitmap(buffer);
	al_clear_to_color(al_map_rgb(0, 0, 0));
}

void Faster_clearing(short x, short y, unsigned short w, unsigned short h)
{
	Clear_screen();
}

void Update_video()
{
	al_set_target_backbuffer(screen);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_scaled_bitmap(buffer, 0, 0, 320, 240, scaleX, scaleY, scaleW, scaleH, 0);
	al_flip_display();
}

void Faster_update(short x, short y, short w, short h)
{
	Update_video();
}


void Sync_refresh()
{
}

void Draw_Pixel(unsigned short x, unsigned short y, unsigned char R, unsigned char G, unsigned char B)
{
	al_draw_rectangle(x, y, x+1, y+1, al_map_rgb(R, G, B), 1);
}

void Controls()
{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = 1;
		}

		BUTTON.QUIT = 0;	
		BUTTON.LEFT = 0;	
		BUTTON.RIGHT = 0;	
		BUTTON.UP = 0;	
		BUTTON.DOWN = 0;	
		BUTTON.A = 0;	
		BUTTON.B = 0;
		BUTTON.C = 0;	
		BUTTON.D = 0;		
		BUTTON.START = 0;	
		BUTTON.SELECT = 0;

		if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(ev.keyboard.keycode)
			{
			case Buttons_QUIT:
				BUTTON.QUIT = 1;	
				break;
			case Buttons_LEFT:
				BUTTON.LEFT = 1;	
				break;
			case Buttons_RIGHT:	
				BUTTON.RIGHT = 1;	
				break;
			case Buttons_UP:
				BUTTON.UP = 1;	
				break;
			case Buttons_DOWN:
				BUTTON.DOWN = 1;	
				break;
			case Buttons_A:	
				BUTTON.A = 1;	
				break;
			case Buttons_B:
				BUTTON.B = 1;
				break;	
			case Buttons_C:
				BUTTON.C = 1;	
				break;
			case Buttons_D:
				BUTTON.D = 1;	
				break;	
			case Buttons_START:
				BUTTON.START = 1;	
				break;
			case Buttons_SELECT:
				BUTTON.SELECT = 1;	
				break;	
			}
		}

}

void Clear_Image(unsigned short a)
{
	if (sprites_img[a] != NULL)
	{
		al_destroy_bitmap(sprites_img[a]);
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
			al_destroy_bitmap(sprites_img[i]);
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
		
		al_install_audio();
		al_reserve_samples(MAX_SFX);
		al_init_acodec_addon();
	}
	
	void Clean_Music()
	{
		if (music != NULL)
		{
			al_destroy_sample(music);
		}
	}

	void Load_Music(const char* directory)
	{
		Clean_Music();
		music = al_load_sample(directory);
	}

	void Play_Music(char loop)
	{
		ALLEGRO_PLAYMODE aa;
		
		if (loop == -1)
		{
			aa = ALLEGRO_PLAYMODE_LOOP;
		}
		else
		{
			aa = ALLEGRO_PLAYMODE_ONCE;
		}
		
		al_play_sample(music, 1.0, 0.0, 1.0, aa, NULL);
	}

	void Load_SFX(unsigned char i, const char* directory)
	{
		#ifdef DEBUG
			fprintf(stderr, "Loading sound effect %d (%s) in memory\n", i, directory);
		#endif
		
		if (gfx_id[i] != NULL)
		{
			al_destroy_sample(gfx_id[i]);
		}
		gfx_id[i] = al_load_sample(directory);
	}

	void Play_SFX(unsigned char i)
	{
		#ifdef DEBUG
			fprintf(stderr, "Play sound effect %d loaded in memory\n", i);
		#endif
		
		al_play_sample(gfx_id[i], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	}

	void Unload_SFX()
	{
		short i;
		
		#ifdef DEBUG
			fprintf(stderr, "Free sound effect from memory\n");
		#endif
		
		for (i=0;i<MAX_SFX;i++)
		{ 
			if (gfx_id[i] != NULL)
			{
				al_destroy_sample(gfx_id[i]);
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
	
	#ifdef DEBUG
		fprintf(stderr, "Destroy Allegro buffer\n");
	#endif

	al_destroy_bitmap(buffer);
	
	
	#ifdef DEBUG
		fprintf(stderr, "Destroy Allegro screen\n");
	#endif
	
	al_destroy_display(screen);
	
	#ifdef DEBUG
		fprintf(stderr, "Screen destroyed\n");
	#endif
}


