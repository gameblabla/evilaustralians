/*   COPYRIGHT (C) 2014-2015 GAMEBLABLA   Licensed under the Apache License, Version 2.0 (the "License");   you may not use this file except in compliance with the License.   You may obtain a copy of the License at       http://www.apache.org/licenses/LICENSE-2.0   Unless required by applicable law or agreed to in writing, software   distributed under the License is distributed on an "AS IS" BASIS,   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   See the License for the specific language governing permissions and   limitations under the License.*/#include "System.h"#include "controlpad.h"#include "math.h"#include "graphics.h"#include "getvideoinfo.h"#include "displayutils.h"#include "timerutils.h"#include "event.h"#include "GestionAffichage.h"#include "GestionSprites.h"#define FPS_VIDEO 60#define MAX_IMAGE 512#ifdef SOUND_ENABLED#include <burger.h>#include "audio.h"#include "musicerror.h"#include "score.h"         #include "soundplayer.h"#include "soundfile.h"#define MAX_SFX 16Item AllSamples[MAX_SFX];			/* Items to sound samples */Word AllRates[MAX_SFX];uint32 MainTask;					/* My own task item */static TagArg SoundRateArgs[] = {	AF_TAG_SAMPLE_RATE,(void*)0,	/* Get the sample rate */	TAG_END,0		/* End of the list */};int32 sfx_data[MAX_SFX];#endifshort sprites_img[MAX_IMAGE];char sprites_state[MAX_IMAGE];Sprite * sprites_[MAX_IMAGE];short PAL_border_IMG;Sprite * PAL_border[2];char *sprites_img_directory[MAX_IMAGE];#include "INPUT.h"struct input BUTTON;unsigned short done = 0;char* game_name = "";unsigned short PAL_Y = 24;unsigned char isPAL = 0;/*  	3DO	CONTROLLER KEY MAPPINGS*/#define Buttons_UP ControlUp#define Buttons_LEFT ControlLeft#define Buttons_RIGHT ControlRight#define Buttons_DOWN ControlDown#define Buttons_A ControlA#define Buttons_B ControlB#define Buttons_C ControlC#define Buttons_D ControlLeftShift#define Buttons_START ControlStart#define Buttons_SELECT ControlRightShift#define Buttons_QUIT 0void msleep(unsigned char milisec){}void Init_video(){	affichageInitialisation();	imageFondChargement("System/donotremove.cel");	InitializeControlPads();	affichageRendu();		/* if 3DO is PAL */	if (Screen_format() > 0)	{		isPAL = 1;		PAL_border[0] = spriteChargement("System/donotremove_spr.cel", 0, 0, 0, 0, 0, FALSE, 0, 0, 0, 0);		PAL_border[1] = spriteChargement("System/donotremove_spr.cel", 0, 0, 0, 0, 0, FALSE, 0, 0, 0, 0);		listeAffichageAjout((Node *) PAL_border[0], TRUE, 1, 1);			listeAffichageAjout((Node *) PAL_border[1], TRUE, 1, 1);		}	else	{		PAL_Y = 0;		isPAL = 0;	}	}void Load_Image(unsigned short a, char* directory){		/* 		The 3DO only accepts CEL format files.		Sprites with Multiple frames must be especially		converted to CEL ANIM format where Width		and Height must be definied at convertion time.	*/		listeAffichageSuppression((Node *) sprites_[a]);	sprites_[a] = spriteChargement(directory, 0, 0, 0, 0, 0, FALSE, 0, 0, 0, 0);	sprites_img_directory[a] = directory;	sprites_state[a] = 0;}void Copy_Image(unsigned short a, unsigned char i){	sprites_[i] = spriteChargement(sprites_img_directory[a], 0, 0, 0, 0, 0, FALSE, 0, 0, 0, 0);	/*sprites_[a] = spriteCopie(sprites_[i]);	sprites_[a] = spriteClone(sprites_[i]);	sprites_state[i] = 0;*/}void Put_image(unsigned short a, short x, short y){	if (sprites_state[a] == 0)	{		listeAffichageAjout((Node *) sprites_[a], TRUE, 1, 1);			sprites_state[a] = 1;	}		spriteModificationPosition(sprites_[a], x, y+PAL_Y, FALSE);	}void Put_sprite(unsigned short a, short x, short y, unsigned short w, unsigned short h, unsigned char f){	/* 		The 3DO only accepts CEL format files.		Sprites with Multiple frames must be especially		converted to CEL ANIM format where Width		and Height are definied at convertion time.		w and h parameters are simply ignored here.	*/		if (sprites_state[a] == 0)	{		listeAffichageAjout((Node *) sprites_[a], TRUE, 1, 1);			sprites_state[a] = 1;	}		spriteModificationPosition(sprites_[a], x, y+PAL_Y, FALSE);	spriteConfigurationFrame(sprites_[a], f);}void Faster_clearing(short x, short y, unsigned short w, unsigned short h){	}void Clear_screen(){}void Update_video(){	/* Display PAL borders */	if (isPAL == 1)	{		spriteModificationPosition(PAL_border[0], 0,   0, FALSE);		spriteModificationPosition(PAL_border[1], 0, 240, FALSE);	}	affichageRendu();}void Faster_update(short x, short y, short w, short h){	Update_video();}void Sync_Refresh(){}void Draw_Pixel(unsigned short x, unsigned short y, unsigned char R, unsigned char G, unsigned char B){	fill_rectangle(x, y, 1, R, G, B);}void Draw_Rect(unsigned short x, unsigned short y, unsigned short width, unsigned char R, unsigned char G, unsigned char B){	fill_rectangle(x, y, width, R, G, B);}void Controls(){	uint32	gButtons;	DoControlPad(1, &gButtons, (ControlUp | ControlDown | ControlLeft | ControlRight));	BUTTON.UP 	= (gButtons & Buttons_UP)	 ? 1 : 0;	BUTTON.DOWN 	= (gButtons & Buttons_DOWN)	 ? 1 : 0;	BUTTON.LEFT 	= (gButtons & Buttons_LEFT)	 ? 1 : 0;	BUTTON.RIGHT 	= (gButtons & Buttons_RIGHT) ? 1 : 0;			BUTTON.A 	= (gButtons & Buttons_A) ? 1 : 0;	BUTTON.B 	= (gButtons & Buttons_B) ? 1 : 0;	BUTTON.C 	= (gButtons & Buttons_C) ? 1 : 0;	BUTTON.D 	= (gButtons & Buttons_D) ? 1 : 0;	BUTTON.START 	= (gButtons & Buttons_START) ? 1 : 0;	BUTTON.SELECT 	= (gButtons & Buttons_SELECT) ? 1 : 0;	}void Clear_Image(unsigned short a){	/* 		The image is still in memory but it will not be shown.		I need to find a proper way to delete images		other than loading a new and smaller one	*/	listeAffichageSuppression((Node *) sprites_[a]);}void Clear_Images(){	unsigned short i;	for (i=0;i<MAX_IMAGE+1;i++)	{		if (sprites_[i])		{			listeAffichageSuppression((Node *) sprites_[i]);		}	}}void Init_sound(){	OpenAudioFolio();	MainTask = KernelBase->kb_CurrentTask->t.n_Item;	}		void Clean_Music(){	PauseSong();}void Load_Music(const char* directory){}void Play_Music(char loop){}void Load_SFX(unsigned char i, const char* directory){}void Play_SFX(unsigned char i){}void Unload_SFX(){}void Clearing(){	Clear_Images();	Clean_Music();	Unload_SFX();}