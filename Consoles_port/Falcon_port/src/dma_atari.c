/** @file src/audio/dsp_atari.c Atari DMA Sound implementation of the DSP. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tos.h>
#include "types.h"
#include "dsp.h"

extern void set_dma_sound(void * buffer, unsigned int len);
extern void stop_dma_sound(void);	/* needs to be called in supervisor mode */
extern unsigned int get_dma_status(void);	/* needs to be called in supervisor mode */

static unsigned char *s_stRamBuffer;
static unsigned int s_stRamBufferSize;

void Sound_Load(const unsigned char* path, SOUND* s)
{
	FILE* fp;
	fp = fopen(path, "rb");
	fseek(fp, 0L, SEEK_END);
	s->size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	s->data = (unsigned char *)Mxalloc(s->size, MX_STRAM);
	fread(s->data, 1, s->size, fp);
	fclose(fp);
}

void Sound_free(SOUND s)
{
	if (s.data != NULL) Mfree(s.data);
}

void DMA_Stop(void)
{
	Supexec(stop_dma_sound);
}

void DMA_Uninit(void)
{
	DMA_Stop();
	if (s_stRamBuffer != NULL) {
		Mfree(s_stRamBuffer);
		s_stRamBuffer = NULL;
	}
	s_stRamBufferSize = 0;
}

bool DMA_Init(void) 
{
	/* allocate ST RAM buffer for audio */
	s_stRamBufferSize = DMASOUND_BUFFER_SIZE;
	s_stRamBuffer = (unsigned char *)Mxalloc(s_stRamBufferSize, MX_STRAM);
	if(s_stRamBuffer == NULL) {
		printf("Failed to allocate %u bytes of ST RAM for DMA sound.\n", s_stRamBufferSize);
		s_stRamBufferSize = 0;
		return false;
	}
	return true;
}

void DMA_Play(const unsigned char *data, const unsigned long size)
{
	set_dma_sound(data, size);
}

/**
 * Should return 2 if playing sound, 0 if not
 */
unsigned char DMA_GetStatus(void)
{
	unsigned char status = (unsigned char)Supexec(get_dma_status);
	return (status != 0) ? 2 : 0;
}
