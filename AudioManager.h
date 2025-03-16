#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <SDL_mixer.h>

class AudioManager {
private:
    Mix_Music* backgroundMusic;
    Mix_Chunk* explosionSound;

public:
    AudioManager();
    ~AudioManager();
    bool init();
    void playBackgroundMusic();
    void playExplosionSound();
    void stopBackgroundMusic();
};

#endif
