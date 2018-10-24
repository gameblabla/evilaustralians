#/!bin/sh
ffmpeg -i "auss.wav" -f s16be -acodec pcm_s16be auss.raw
ffmpeg -i "batt.wav" -f s16be -acodec pcm_s16be batt.raw
ffmpeg -i "end.wav" -f s16be -acodec pcm_s16be end.raw
ffmpeg -i "hero.wav" -f s16be -acodec pcm_s16be hero.raw
ffmpeg -i "inst.wav" -f s16be -acodec pcm_s16be inst.raw
ffmpeg -i "win.wav" -f s16be -acodec pcm_s16be win.raw
ffmpeg -i "hit.wav" -f s16be -acodec pcm_s16be hit.raw
ffmpeg -i "hit2.wav" -f s16be -acodec pcm_s16be hit2.raw
ffmpeg -i "shoot.wav" -f s16be -acodec pcm_s16be shoot.raw
ffmpeg -i "shoot_enemy.wav" -f s16be -acodec pcm_s16be shoot_enemy.raw
