#include <audioengine.h>
#include <iostream>

irrklang::ISoundEngine* audioEngine = audioInit();
irrklang::ISound * music = NULL;
irrklang::ik_u32 songPos = 0;
Song * prevSongRef = NULL;
Song * curSongRef = NULL;
std::vector<Song*> songsPlayed = {};
unsigned int shuffleQueueIndex = 0;
std::vector<Song*> shuffleQueue = {};
bool shuffle = false;

irrklang::ISoundEffectControl * fx = 0;

using namespace irrklang;

//ISoundStopEventReceiver * soundFinishedCallback() {
////    songPos = 0;
////    playAudioFile(engine, path);

//    std::cout << songPos << std::endl;
//    std::cout << "Song Over" << std::endl;
//    return NULL;
//}


class SoundStopEventReceiver : public ISoundStopEventReceiver {
public:
    virtual void OnSoundStopped(ISound * sound, E_STOP_EVENT_CAUSE reason, void * userData) override
    {
        // Only act if song naturally finished
        if (reason == irrklang::E_STOP_EVENT_CAUSE::ESEC_SOUND_FINISHED_PLAYING) {
            playAudioFile(audioEngine, curSongRef->path.c_str());
        }

//        else if (reason == irrklang::E_STOP_EVENT_CAUSE::ESEC_SOUND_STOPPED_BY_USER) {

//        }
    }
};

SoundStopEventReceiver soundFinishedCallback;

ISoundEngine* audioInit() {
    ISoundEngine* engine = createIrrKlangDevice();

    if (!engine) {
        std::cout << "AUDIO FAILED" << std::endl;
        return NULL;
    }

    return engine;
}

ISound * playAudioFile(ISoundEngine* engine, const char * path) {
    music = engine->play3D(path, vec3df(0,0,0), false, false, true, ESM_AUTO_DETECT, true);
//    music->setSoundStopEventReceiver(, 0);
    music->setSoundStopEventReceiver(&soundFinishedCallback);
    music->setPlayPosition(songPos);
    addReverb();
}

void addReverb() {
    fx = music->getSoundEffectControl();
    if (!fx->isWavesReverbSoundEffectEnabled())
        fx->enableWavesReverbSoundEffect();
}
