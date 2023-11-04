#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H
#include <irrKlang.h>



irrklang::ISoundEngine* audioInit();
irrklang::ISound * playAudioFile(irrklang::ISoundEngine* engine, const char * path);

#endif // AUDIOENGINE_H
