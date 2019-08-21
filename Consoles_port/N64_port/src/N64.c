/*The MIT License (MIT)Copyright (c) 2016 GameblablaPermission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:*/#define FPS_VIDEO 60#define MAX_IMAGE 24#define MAX_SND 16#include <stdio.h>#include <malloc.h>#include <string.h>#include <time.h>#include <stdint.h>#include <libdragon.h>#include "INPUT.h"struct controller_data keys;static display_context_t disp = 0;struct input BUTTON;const float real_FPS = 1000/FPS_VIDEO;unsigned short done = 0;char* game_name;sprite_t *sprites[MAX_IMAGE];void Clear_Image(unsigned short a);sprite_t* Load_Picture(const char* path){	int fp;		fp = dfs_open(path);    sprite_t *temp__ = malloc( dfs_size( fp ) );    dfs_read( temp__, 1, dfs_size( fp ), fp );    dfs_close( fp );    return temp__;}void msleep(unsigned char milisec){}void Init_video(char* argv[]){    /* enable interrupts (on the CPU) */    init_interrupts();    /* Initialize peripherals */    display_init( RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE );    dfs_init( DFS_DEFAULT_LOCATION );    rdp_init();    controller_init();    timer_init();        audio_init(11025,2);}void Load_Image(unsigned short a, const char* directory){	if (!sprites[a]) sprites[a] = Load_Picture(directory);	else	{		Clear_Image(a);		sprites[a] = Load_Picture(directory);	}}void Put_image(unsigned short a, short x, short y){	graphics_draw_sprite(disp, x, y, sprites[a]);}void Put_image_trans(unsigned short a, short x, short y){	graphics_draw_sprite_trans(disp, x, y, sprites[a]);}void Put_sprite(unsigned short a, short x, short y, unsigned short w, unsigned short h, unsigned char f){	if (x < 320-w || x > -w) graphics_draw_sprite_stride(disp, x, y, sprites[a], f);}void Put_sprite_trans(unsigned short a, short x, short y, unsigned short w, unsigned short h, unsigned char f){	if (x < 320-w || x > -w) graphics_draw_sprite_trans_stride(disp, x, y, sprites[a], f);}void Put_sprite_hardware(unsigned short a, short x, short y, unsigned short w, unsigned short h, unsigned char f){	/*rdp_sync( SYNC_PIPE );	rdp_load_texture_stride( 0, 0, MIRROR_DISABLED, sprites[a], f );	rdp_draw_sprite( 0, x, y );*/}void Clear_screen(){}void Faster_clearing(short x, short y, unsigned short w, unsigned short h){}void Update_video(){	//rdp_detach_display();	display_show(disp);	//MikMod_Update();		controller_scan();	keys = get_keys_held();}void Faster_update(short x, short y, short w, short h){	Update_video();}void Sync_refresh(){}void Draw_Pixel(unsigned short x, unsigned short y, unsigned char R, unsigned char G, unsigned char B){}void Draw_Rect(unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned char R, unsigned char G, unsigned char B){}void Controls(){	disp = 0;		while( !(disp = display_lock()) )	{	}	BUTTON.UP = (keys.c[0].up || (keys.c[0].y < -64)) ? 1 : 0;	BUTTON.DOWN = (keys.c[0].down || (keys.c[0].y > 64)) ? 1 : 0;	BUTTON.LEFT = (keys.c[0].left || (keys.c[0].x < -64)) ? 1 : 0;	BUTTON.RIGHT = (keys.c[0].right || (keys.c[0].x > 64)) ? 1 : 0;			BUTTON.A = (keys.c[0].A) ? 1 : 0;	BUTTON.B = (keys.c[0].B) ? 1 : 0;	BUTTON.C = (0) ? 1 : 0;	BUTTON.D = (0) ? 1 : 0;	BUTTON.START = (keys.c[0].start) ? 1 : 0;	/*rdp_sync( SYNC_PIPE );    rdp_set_default_clipping();	rdp_enable_texture_copy();	rdp_attach_display( disp );*/}void Clear_Image(unsigned short a){	if (sprites[a]) free(sprites[a]);}void Clear_Images(){	unsigned int i;	for(i=0;i<MAX_IMAGE;i++)	{		if (sprites[i]) free(sprites[i]);	}}void Init_sound(){	}		void Clean_Music(){}void Load_Music(char* directory){}void Play_Music(char loop){}void Load_SFX(unsigned char i, char* directory){}void Play_SFX(unsigned char i){}void Unload_SFX(){}
void Clearing(){	Clear_Images();}