#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H
#include <irrKlang.h>
#include "metastructures.h"

irrklang::ISoundEngine* audioInit();
irrklang::ISound * playAudioFile(irrklang::ISoundEngine* engine, const char * path);

extern Song * prevSongRef;
extern Song * curSongRef;
extern std::vector<Song*> songsPlayed;
extern unsigned int shuffleQueueIndex;
extern std::vector<Song*> shuffleQueue;

extern bool shuffle;

extern irrklang::ISoundEngine* audioEngine;
extern irrklang::ISound * music;
extern irrklang::ik_u32 songPos;

void addReverb();

#endif // AUDIOENGINE_H
