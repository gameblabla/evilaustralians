/*
 * This should be used for developers who wants 
 * to port it to other platforms and not by end-users !
*/

/*
 * Add here all the headers
 * for graphics handling and controls.
 * For example :
 #include <SDL/SDL.h>
*/

#define FPS_VIDEO 60
#define MAX_IMAGE 512

#ifdef SOUND_ENABLED
	/*
	 * Add here the headers for loading sounds and music.
	 * Maybe we need variables for storing the sound
	 * data as well !
	 * For example :
		 #include <SDL/SDL_mixer.h>
		 #define MAX_SFX 16
		 int music;
		 int gfx_id[32];
	*/
#endif

#ifdef IMAGE_CODEC_ENABLED
	/*
	 * Add here the headers to support additional images formats.
	 * For example :
	 #include <SDL/SDL_image.h>
	*/
#endif

/*
 * What's below should always be there. 
 * Default framerate is 60 but you can change it (FPS_VIDEO)
 * if the console you are porting to only supports
 * 50 frames per second for example.
 * Or maybe it's slow to handle that framerate.
 * 
 * Game_name should be used for displaying the game's name.
 * Useful for PCs but useless for consoles.
 * 
 * If done = 1 then the program should quit.
*/

#include "INPUT.h"
struct input BUTTON;

int done = 0;
const float real_FPS = 1000/FPS_VIDEO;
char* game_name = "";


/*
	* Generally, we need one array for storing the actual data
	* We also need (in general) a variable to store the context. (aka surface or screen)
	* 
	* For example (SDL 1.2) :
	SDL_Surface *sprites_img[MAX_IMAGE];
	SDL_Surface *screen;
*/

/*	
	* The following defines are used for
	* mapping controls.
	* They work well for controllers and keyboards.
	*
	* For example :
	* 
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
*/

void msleep(unsigned char milisec)
{
	/*
	 * If you need anything for keeping 
	 * in sync with the framerate, put it here.
	 * For example :
	 * SDL_Delay(millisec);
	 * 
	*/
}

void Init_video(char* argv[])
{
	/*
	 * Put everything here for video initialisation and controls.
	 * For sound, see Init_sound.
	 * For example :
	  SDL_Init( SDL_INIT_VIDEO );
	  screen = SDL_SetVideoMode(320, 240, 16, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
	*/
}

void Load_Image(unsigned short a, const char* directory)
{
	/*
		* Put everything here for loading images from a directory.
		* Also, make sure to free the image/surface before you load a new one in memory !
		* For example (very unoptimised on purpose) :
		* 
		SDL_FreeSurface(sprites_img[a]);
		sprites_img[a] = SDL_LoadBMP(directory);
	*/
}


void Put_image(unsigned short a, short x, short y)
{
	
	/*
		* Put the image in question on screen.
		* It takes the id of the images you should and also takes
		* two coordinates : X (horizontal) and Y (vertical).
		* The Allegro5 port gives you a good example of this.
		* 
		* For example: (Allegro5)
		* 
		al_draw_bitmap(sprites_img[a], x, y, 0);
	*/
}

void Put_sprite(unsigned short a, short x, short y, unsigned short w, unsigned short h, unsigned char f)
{
	/*
		* Same here except you need to show a sprite with multiple frames here.
		* All sprite tables must be horizontal.
		* Again, the Allegro5 port is a good example on how to do this.
		* For example: (Allegro5)
		* 
		al_draw_bitmap_region(sprites_img[a], f*w, 0, w, h, x, y, 0);
	*/
}

void Clear_screen()
{
	/*
	 * Clear everything on screen.
	 * Sometimes, it is needed and sometimes it's not.
	 * Make sure to compare it to the other ports,
	 * they have to behave (almost) exactly !
	 * For example (SDL 1.2) :
	 SDL_FillRect(screen, 0, 0);
	*/
}


void Update_video()
{
	/*
		Update everything on screen and keep it in sync with the framerate.
		For example : (SDL 1.2) 

		Uint32 start;
		start = SDL_GetTicks();
		SDL_Flip(screen);
		if(real_FPS > SDL_GetTicks()-start) msleep(real_FPS-(SDL_GetTicks()-start));

		This is where the msleep function comes in action !
		Remember, compare it to the other ports !
	*/
}

void Faster_update(short x, short y, short w, short h)
{
}

void Faster_clearing(short x, short y, unsigned short w, unsigned short h)
{
}


void Sync_refresh()
{
/*
 * Same as Update_video except you don't refresh the screen.
 * Example :
	Uint32 start;
	start = SDL_GetTicks();
	if(real_FPS > SDL_GetTicks()-start) msleep(real_FPS-(SDL_GetTicks()-start));
*/
}

void Draw_Pixel(unsigned short x, unsigned short y, unsigned char R, unsigned char G, unsigned char B)
{
/*
 * Draw a pixel on screen. (with colors in RGB format)
 * Example (SDL2) :
	int color = makecol(R,G,B);
	rectfill(screen, x, y, x+1, x+1, color);
*/
}

void Controls()
{
	/*
	 * Input detection should be done here.
	 * Have a look at the SDL 1.2 port for more details.
	 * 
	 * Below is the content of the Controls function
	 * taken from the PSP port
	 */
	
/*
		SceCtrlData pad;
		sceCtrlPeekBufferPositive(&pad,1);
		
		BUTTON.UP 		= (pad.Buttons & Buttons_UP)	 ? 1 : 0;
		BUTTON.DOWN 	= (pad.Buttons & Buttons_DOWN)	 ? 1 : 0;
		BUTTON.LEFT 	= (pad.Buttons & Buttons_LEFT)	 ? 1 : 0;
		BUTTON.RIGHT 	= (pad.Buttons & Buttons_RIGHT)  ? 1 : 0;
		
		BUTTON.A 		= (pad.Buttons & Buttons_A) ? 1 : 0;
		BUTTON.B 		= (pad.Buttons & Buttons_B) ? 1 : 0;
		BUTTON.C 		= (pad.Buttons & Buttons_C) ? 1 : 0;
		BUTTON.D 		= (pad.Buttons & Buttons_D) ? 1 : 0;
		BUTTON.START 	= (pad.Buttons & Buttons_START) ? 1 : 0;
		BUTTON.SELECT 	= (pad.Buttons & Buttons_SELECT) ? 1 : 0;
*/
}

void Clear_Image(unsigned short a)
{
	/*
	 * Clear an image in question.
	 * Make sure it's not NULL before you clear it !
	 * For example : (SDL 1.2)

		if (sprites_img[a] != NULL)
		{
			SDL_FreeSurface(sprites_img[a]);
		}
	 */
}

void Clear_Images()
{
	/*
	 * Same here except you put it in a loop.
	 * For example : (SDL 1.2)

		short i;

		for (i=0;i<MAX_IMAGE+1;i++)
		{
			if (sprites_img[i] != NULL)
			{
				SDL_FreeSurface(sprites_img[i]);
			}
		}
	 */
}


#ifdef SOUND_ENABLED

		void Init_sound()
		{
			/*
			 * Sound initialisation comes here.
			 * This include allocating the channels,
			 * setting the frequency, opening the sound device...
			 * For example (SDL_mixer)
				Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024);
				Mix_AllocateChannels(MAX_SFX);
			 */
		}
		
		void Clean_Music()
		{
			/*
			 * Clear music in memory.
			 * For example (SDL_mixer)
				Mix_HaltMusic();
				Mix_FreeMusic(music);
			 */
		}

		void Load_Music(const char* directory)
		{
			/*
			 * Loading music in memory.
			 * Have a look at the dreamcast port on
			 * how you should handle multiple formats with multiple libraries.
			 * For example (SDL_mixer)
				Clean_Music();
				music = Mix_LoadMUS(directory);
			 */
		}

		void Play_Music(char loop)
		{
			/*
			 * Play music, looping or not.
			 * For example (SDL_mixer)
				if (loop == 1)
				{
					Mix_PlayMusic(music, -1);
				}
				else
				{
					Mix_PlayMusic(music, 0);
				}
			 */
		}

		void Load_SFX(unsigned char i, const char* directory)
		{
			/*
			 * Loading a new sound effect from a directory.
			 * For example (SDL_mixer)
				Mix_FreeChunk(gfx_id[i]);
				gfx_id[i] = NULL;
				gfx_id[i] = Mix_LoadWAV(directory);
			 */
		}

		void Play_SFX(unsigned char i)
		{
			/*
			 * Play the sound effect.
			 * For example (SDL_mixer)
				Mix_PlayChannel(-1, gfx_id[i], 0);
			 */
		}

		void Unload_SFX()
		{
			/*
			 * Clears all the sound effects.
			 * For example (SDL_mixer)
				short i;
				for (i=0;i<MAX_SFX;i++) 
				{
					Mix_FreeChunk(gfx_id[i]);
					gfx_id[i]=NULL;
				}
			 */
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
	/*
		Clearing everything in memory.
		For example :
		
		#ifdef SOUND_ENABLED
			Clean_Music();
		#endif
		
		Clear_Images();
		
		SDL_FreeSurface(screen);
		
		#ifdef SOUND_ENABLED
			Unload_SFX(); 
			Mix_CloseAudio();
		#endif
		
		return 0;
	*/
}

