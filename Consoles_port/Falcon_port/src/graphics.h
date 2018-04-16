#ifndef GRAPH_H_
#define GRAPH_H_

#include <tos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "falcsys.h"

#define RL_SCREEN_WIDTH FS_ScreenWidth 
#define RL_SCREEN_HEIGHT FS_ScreenHeight

#define Draw_BMP(a,b) FastCopy32(a, scr32, b);
#define Draw_BMP_Big(a,b) VFastCopy32(a, scr32, b);

typedef struct tagBITMAP
{
	unsigned short width;
	unsigned short height;
	unsigned short dv_width;
	unsigned short db_width;
	unsigned long tsize;
	unsigned char *data;
	unsigned char frame_width;
} BITMAP;

void Load_BMP(unsigned char* path, BITMAP *b, const unsigned char frame_width);
void Free_BMP(BITMAP b);

void DrawSprite(BITMAP* b, u16* dst, short x, unsigned short y);
void DrawSprite_frame(BITMAP* b, u16* dst, short x, unsigned short y, unsigned char frame);

void DrawSprite_NoChecks(BITMAP* b, u16* dst, unsigned short x, unsigned short y);
void DrawSprite_NoChecks_frame(BITMAP* b, u16* dst, unsigned short x, unsigned short y, unsigned char frame);

void Notrans_DrawSprite(BITMAP* b, u16* dst, short x, unsigned short y);
void Notrans_DrawSprite_frame(BITMAP* b, u16* dst, short x, unsigned short y, unsigned char frame);
void Notrans_DrawSprite_noheightcheck(BITMAP* b, u16* dst, short x, unsigned short y);

void Notrans_DrawSprite_NoChecks(BITMAP* b, u16* dst, unsigned short x, unsigned short y);
void Notrans_DrawSprite_NoChecks_frame(BITMAP* b, u16* scr, unsigned short x, unsigned short y, unsigned char frame );

void Notrans_DrawSprite_NoChecks_scroll(BITMAP* b, u16* scr, unsigned short scroll );

#endif
