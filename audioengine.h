#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H
#include "metastructures.h"

#include <QAudioOutput>
#include <QMediaPlayer>

extern QMediaPlayer * mediaPlayer;
extern QAudioOutput * audioOutput;

extern Song * prevSongRef;
extern Song * curSongRef;
extern unsigned int shuffleQueueIndex;
extern std::vector<Song*> shuffleQueue;

extern bool shuffle;

void addReverb();

void qPlayFile (std::string path);
void qAudioInit();

void setEq(float b1, float b2, float b3, float b4, float b5, float b6, float b7, float b8, float b9, float b10);

#endif // AUDIOENGINE_H
