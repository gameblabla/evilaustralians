/**
 * FlappyBird-N64 - audio.h
 *
 * Copyright 2017, Christopher Bonhage
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __FLAPPY_AUDIO_H
#define __FLAPPY_AUDIO_H

#include "system.h"

/* Audio definitions */
#define FREQUENCY_11KHZ 11025
#define FREQUENCY_44KHZ 44100

#define MONO_PCM_SAMPLE_SIZE sizeof(int16_t)
#define STEREO_PCM_SAMPLE_SIZE (MONO_PCM_SAMPLE_SIZE << 1)

/* Sound FX definitions */

#define SFX_NUM_CHANNELS    8
#define SFX_NUM_SOUNDS      10

typedef enum sfx_sounds
{
    SFX_DIE,
    SFX_HIT,
    SFX_POINT,
    SFX_SWOOSH,
    SFX_WING
} sfx_sounds_t;

typedef struct
{
    uint16_t sample_rate;
    uint8_t channels;
    uint32_t frames;
    uint32_t samples;
    int16_t *data;
} pcm_sound_t;

typedef struct
{
    uint32_t cursor;
	uint8_t looping;
    pcm_sound_t *sfx;
} sfx_channel_t;

typedef struct
{
    // Setup state
    uint16_t sample_rate;
    uint32_t frames;
    int16_t *buffer;
    pcm_sound_t *sfx_cache[SFX_NUM_SOUNDS];
    // Playback state
    sfx_channel_t channels[SFX_NUM_CHANNELS];
} audio_t;

/* Audio functions */

audio_t *audio_setup(const uint16_t sample_rate, const uint8_t buffers, const char* sfx_files[]);

void audio_load_pcm(audio_t *audio, uint8_t number, const char* sfx_files[]);

void audio_free_pcm(audio_t *audio, uint8_t number);

void audio_free(audio_t *audio);

void audio_tick(audio_t *audio);

void audio_play_sfx(audio_t *audio, const sfx_sounds_t sfx_sound);

void audio_play_music(audio_t *audio, const sfx_sounds_t sfx_sound, const uint8_t looping);

pcm_sound_t *read_dfs_pcm_sound(const char *file, uint16_t sample_rate, uint8_t channels);

#endif
