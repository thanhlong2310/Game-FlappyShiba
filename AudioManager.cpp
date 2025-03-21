#include <iostream>
#include "AudioManager.h"

AudioManager::AudioManager()
{
    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
    {
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
    backgroundMusic = Mix_LoadMUS("assets/background.wav");
    if(!backgroundMusic)
    {
        std::cout << "Failed to load background music! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
    jumpSound = Mix_LoadWAV("assets/jump.wav");
    if (!jumpSound) {
        std::cout << "Failed to load jump sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
    explosionSound = Mix_LoadWAV("assets/explosion.wav");
    if (!explosionSound)
    {
        std::cout << "Failed to load explosion sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
}
AudioManager::~AudioManager()
{
    Mix_FreeMusic(backgroundMusic);
    Mix_FreeChunk(jumpSound);
    Mix_FreeChunk(explosionSound);
    Mix_CloseAudio();
}
void AudioManager::playBackgroundMusic()
{
    if (backgroundMusic)
    {
        Mix_PlayMusic(backgroundMusic, -1);
    }
}
void AudioManager::stopBackgroundMusic()
{
    Mix_HaltMusic();
}
void AudioManager::playJumpSound()
{
    if (jumpSound)
    {
        Mix_PlayChannel(-1, jumpSound, 0);
    }
}
void AudioManager::playExplosionSound()
{
    if (explosionSound)
    {
        Mix_PlayChannel(-1, explosionSound, 0);
    }
}
