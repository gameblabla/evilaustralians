/** @file src/audio/dsp.h DSP definitions. */

#ifndef DMA_H
#define DMA_H

typedef struct tagS
{
  unsigned char* data;
  unsigned long size;
} SOUND;

/* STE/TT/Falcon DMA Sound is able to play at :
 * 50066Hz 25033Hz 12517Hz 6258Hz
 * (6258Hz is not available on falcon) */
#define DMASOUND_FREQ 12517
#define DMASOUND_BUFFER_SIZE	64*1024

#define Sound_Play(x,b) DMA_Play(x,b)
extern void Sound_Load(unsigned char* path);
extern void DMA_Play(unsigned char loop);
extern void DMA_Stop(void);
extern unsigned char DMA_GetStatus(void);
extern bool DMA_Init(void);
extern void DMA_Uninit(void);

#endif /* DMA_H */
