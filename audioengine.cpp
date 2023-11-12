#include <audioengine.h>
#include <iostream>
#include <QTimer>
#include <QIODevice>
#include <QAudioBuffer>

#include <stk/Stk.h>
#include <stk/FreeVerb.h>

#include <stdlib.h>



Song * prevSongRef = NULL;
Song * curSongRef = NULL;
unsigned int shuffleQueueIndex = 0;
std::vector<Song*> shuffleQueue = {};
bool shuffle = false;

QMediaPlayer * mediaPlayer = new QMediaPlayer();
QAudioOutput * audioOutput = new QAudioOutput();

stk::FreeVerb freeVerb;



void qAudioInit() {
    mediaPlayer->setAudioOutput(audioOutput);
    audioOutput->setVolume(1);
}

void qPlayFile (std::string path) {
    mediaPlayer->setSource(QUrl::fromLocalFile(path.c_str()));
    mediaPlayer->play();
}

void setEq(float b1, float b2, float b3, float b4, float b5, float b6, float b7, float b8, float b9, float b10) {

}

void addReverb() {

}
