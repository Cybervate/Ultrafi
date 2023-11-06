#include <audioengine.h>
#include <iostream>
#include "mainwindow.h"

using namespace irrklang;

ISoundEngine* audioInit() {
    ISoundEngine* engine = createIrrKlangDevice();

    if (!engine) {
        std::cout << "AUDIO FAILED" << std::endl;
        return NULL;
    }

    return engine;
}

ISound * playAudioFile(ISoundEngine* engine, const char * path) {
    ISound* music = engine->play3D(path, vec3df(0,0,0), true, false, true);
    return music;
}
