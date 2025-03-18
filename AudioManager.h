#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#include <SDL_mixer.h>

class AudioManager
{
public:
    Mix_Music* backgroundMusic;
    Mix_Chunk* jumpSound;
    Mix_Chunk* explosionSound;
    AudioManager();
    ~AudioManager();
    void playBackgroundMusic();
    void stopBackgroundMusic();
    void playJumpSound();
    void playExplosionSound();
};
#endif
