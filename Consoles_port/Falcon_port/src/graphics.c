// FalconSystem v5 Demo Example
#include "graphics.h"
#define DV_SCREEN_WIDTH = 160;
#define DV_SCREEN_HEIGHT = 120;

void Load_BMP(unsigned char* path, BITMAP *b, const unsigned char frame_width)
{
	int fp;
	fp = Fopen(path, 1);
	if (b->data) 
	{
		Mfree(b->data);
	}
	
	Fseek(0, fp,  SEEK_SET);
	Fread(fp, 2, &b->width);
	Fseek(2, fp, SEEK_SET);
	Fread(fp, 2, &b->height);
	Fseek(4, fp,  SEEK_SET);
	
	b->tsize = (b->width*b->height)*2;
	b->data = (unsigned char*) Mxalloc(b->tsize, MX_STRAM);

	Fread(fp, b->tsize, b->data);
	b->db_width = b->width * 2;
	b->dv_width = b->width / 2;
	
	if (frame_width > 0) 
		b->frame_width = frame_width;
	else 
		b->frame_width = b->width;
	
	Fclose(fp);
}

void Free_BMP(BITMAP b)
{
	if (b.data) Mfree(b.data);
}

/*
DrawSprite supports clipping
*/
/*
void DrawSprite(BITMAP* b, u16* dst, short x, unsigned short y)
{
	u16* to;
    u16* from = (u16*)b->data;
    short s = 0, h = 0;
    unsigned short total, j;
    
    if (x + b->width > RL_SCREEN_WIDTH)
    {
		if (x+b->width >= RL_SCREEN_WIDTH+b->width) return;
		s = (x + b->width) - RL_SCREEN_WIDTH;
	}
	
	if (x < 0)
	{
		s = h = -x;
		if (x <= -b->width) return;
		x = 0;
	}
	
	to = (u16*)dst + x + y * RL_SCREEN_WIDTH;
		
	if (y + b->height > RL_SCREEN_HEIGHT)
		j = (b->height - ((y + b->height) - RL_SCREEN_HEIGHT));	
    else
		j = b->height;	
	
	if (y+b->height >= RL_SCREEN_HEIGHT+b->height) return;
	
	total = (b->width-s)*2;
	
    while(j--)
    {
        FastCopy16Skip(from+h, to, (b->width-s)*2, 0xf81f);
        to += RL_SCREEN_WIDTH;
        from += b->width;
    }
}*/

void DrawSprite_frame(BITMAP* b, u16* dst, short x, unsigned short y, unsigned char frame)
{
    u16* from = (u16*)b->data + (b->frame_width*frame);
    u16* to;
    short s = 0, h = 0;
    unsigned short total, j;
    
    if (x + b->frame_width > RL_SCREEN_WIDTH)
    {
		if (x+b->frame_width >= RL_SCREEN_WIDTH+b->frame_width) return;
		s = (x + b->frame_width) - RL_SCREEN_WIDTH;
	}
	
	if (x < 0)
	{
		s = h = -x;
		if (x <= -b->frame_width) return;
		x = 0;
	}
	
	total = (b->frame_width-s)*2;
	to = (u16*)dst + x + y * RL_SCREEN_WIDTH;
		
	if (y + b->height > RL_SCREEN_HEIGHT)
		j = (b->height - ((y + b->height) - RL_SCREEN_HEIGHT));	
    else
		j = b->height;	
	
	if (y+b->height >= RL_SCREEN_HEIGHT+b->height) return;
	
    while(j--)
    {
        FastCopy16Skip(from+h, to, total, 0xf81f);
        to += RL_SCREEN_WIDTH;
        from += b->width;
    }
}

/*
DrawSprite_NoChecks does not support clipping
*/
/*void DrawSprite_NoChecks(BITMAP* b, u16* dst, unsigned short x, unsigned short y)
{
    u16* from = (u16*)b->data;
    u16* to = (u16*)dst + x + y * RL_SCREEN_WIDTH;
    unsigned short j, h;
    
    j = b->height;
    h = b->db_width;
    
    while(j--)
    {
        FastCopy16Skip(from, to, h, 0xf81f);
        to += RL_SCREEN_WIDTH;
        from += b->width;
    }
}*/

void DrawSprite_NoChecks_frame(BITMAP* b, u16* dst, unsigned short x, unsigned short y, unsigned char frame)
{
    u16* from = (u16*)b->data;
    u16* to = (u16*)dst + x + y * RL_SCREEN_WIDTH;
    unsigned short j, h;
    
    j = b->height;
    h = (b->frame_width)*2;
    
    while(j--)
    {
        FastCopy16Skip(from+(b->frame_width*frame), to, h, 0xf81f);
        to += RL_SCREEN_WIDTH;
        from += b->width;
    }
}

void Notrans_DrawSprite(BITMAP* b, u16* dst, short x, unsigned short y)
{
    u16* from;
    u16* to;
    short s = 0, h = 0;
    unsigned short total, j;
    
    if (x + b->width > RL_SCREEN_WIDTH)
    {
		if (x+b->width >= RL_SCREEN_WIDTH+b->width) return;
		s = (x + b->width) - RL_SCREEN_WIDTH;
	}
	
	if (x < 0)
	{
		s = h = -x;
		if (x <= -b->width) return;
		x = 0;
	}
	
	if (y+b->height >= RL_SCREEN_HEIGHT+b->height) return;
	
	from = (u16*)b->data + h;
	to = (u16*)dst + x + y * RL_SCREEN_WIDTH;
		
	if (y + b->height > RL_SCREEN_HEIGHT)
		j = (b->height - ((y + b->height) - RL_SCREEN_HEIGHT));	
    else
		j = b->height;	
	
	total = (b->width-s)*2;
	
    while(j--)
    {
        FastCopy16(from, to, total);
        to += RL_SCREEN_WIDTH;
        from += b->width;
    }
}

/*
void Notrans_DrawSprite_frame(BITMAP* b, u16* dst, short x, unsigned short y, unsigned char frame)
{
    u16* from = (u16*)b->data + (b->frame_width*frame);
    u16* to;
    short s = 0, h = 0;
    unsigned short total, j;
    
    if (x + b->frame_width > RL_SCREEN_WIDTH)
    {
		if (x+b->frame_width >= RL_SCREEN_WIDTH+b->frame_width) return;
		s = (x + b->frame_width) - RL_SCREEN_WIDTH;
	}
	
	if (x < 0)
	{
		s = h = -x;
		if (x <= -b->frame_width) return;
		x = 0;
	}
	
	total = (b->frame_width-s)*2;
	to = (u16*)dst + x + y * RL_SCREEN_WIDTH;
		
	if (y + b->height > RL_SCREEN_HEIGHT)
		j = (b->height - ((y + b->height) - RL_SCREEN_HEIGHT));	
    else
		j = b->height;	
	
	if (y+b->height >= RL_SCREEN_HEIGHT+b->height) return;
	
    while(j--)
    {
        VFastCopy16(from+h, to, total);
        to += RL_SCREEN_WIDTH;
        from += b->width;
    }
}

void Notrans_DrawSprite_NoChecks(BITMAP* b, u16* scr, unsigned short x, unsigned short y)
{
	u16* dst = (u16*)scr + (x + y * RL_SCREEN_WIDTH);
	u16* src = (u16*)b->data;
	u16 row = b->height;
   
	while (row--)
	{
		VFastCopy16(src, dst, b->db_width);
		dst += (RL_SCREEN_WIDTH);
		src += b->width;
	}
}

void Notrans_DrawSprite_NoChecks_frame(BITMAP* b, u16* scr, unsigned short x, unsigned short y, unsigned char frame )
{
	u16* dst = (u16*)scr + (x + y * RL_SCREEN_WIDTH);
	u16* src = (u16*)b->data;
	u16 row = b->height;
   
	while (row--)
	{
		VFastCopy16(src+(b->frame_width*frame), dst, (b->frame_width)*2);
		dst += (RL_SCREEN_WIDTH);
		src += b->width;
	}
}*/

void Notrans_DrawSprite_NoChecks_scroll(BITMAP* b, u16* scr, unsigned short scroll )
{
	u16* dst = (u16*)scr;
	u16* src = (u16*)b->data;
	unsigned char row = b->height;
	
	while (row--)
	{
		VFastCopy128(src+scroll, dst);
		dst += (RL_SCREEN_WIDTH);
		src += b->width;
	}
}
