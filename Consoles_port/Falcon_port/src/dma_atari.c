/** @file src/audio/dsp_atari.c Atari DMA Sound implementation of the DSP. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tos.h>
#include "types.h"
#include "dma.h"

extern void set_dma_sound(void * buffer, unsigned int len);
extern void set_dma_sound_noloop(void * buffer, unsigned int len);
extern void stop_dma_sound(void);	/* needs to be called in supervisor mode */
//extern unsigned int get_dma_status(void);	/* needs to be called in supervisor mode */

static unsigned char *s_stRamBuffer;
static unsigned int s_stRamBufferSize;

static SOUND snd;

static void Sound_free()
{
	if (snd.data) Mfree(snd.data);
}

void Sound_Load(unsigned char* path)
{
	int fp;
	unsigned short	handle;
	
	DMA_Stop();
	Sound_free();

	fp = Fopen(path, 1);
	Fseek(0, fp,  SEEK_END);
	handle = fp & 0xFFFF;
	snd.size = Fseek(0, handle, 2);
	Fseek(0, handle, 0);
	snd.data = (unsigned char *)Mxalloc(snd.size, MX_STRAM);
	Fread(handle, snd.size, snd.data);
	Fclose(handle);
}


void DMA_Stop(void)
{
	Supexec(stop_dma_sound);
}

void DMA_Uninit(void)
{
	DMA_Stop();
	if (s_stRamBuffer != NULL) 
	{
		Mfree(s_stRamBuffer);
		s_stRamBuffer = NULL;
	}
	s_stRamBufferSize = 0;
	Sound_free();

}

bool DMA_Init(void) 
{
	/* allocate ST RAM buffer for audio */
	s_stRamBufferSize = DMASOUND_BUFFER_SIZE;
	s_stRamBuffer = (unsigned char *)Mxalloc(s_stRamBufferSize, MX_STRAM);
	if(s_stRamBuffer == NULL) {
		s_stRamBufferSize = 0;
		return false;
	}
	return true;
}

void DMA_Play(unsigned char loop)
{
	if (loop == 1)
		set_dma_sound(snd.data, snd.size);
	else
		set_dma_sound_noloop(snd.data, snd.size);
}

/**
 * Should return 2 if playing sound, 0 if not
 */
/*unsigned char DMA_GetStatus(void)
{
	unsigned char status = (unsigned char)Supexec(get_dma_status);
	return (status != 0) ? 2 : 0;
}*/
