#include "AudioManager.h"
#include <iostream>

AudioManager::AudioManager() : backgroundMusic(nullptr), explosionSound(nullptr) {}

AudioManager::~AudioManager() {
    Mix_FreeMusic(backgroundMusic);
    Mix_FreeChunk(explosionSound);
    Mix_Quit();
}

bool AudioManager::init() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    backgroundMusic = Mix_LoadMUS("assets/background.wav");
    if (!backgroundMusic) {
        std::cout << "Failed to load background music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    explosionSound = Mix_LoadWAV("assets/explosion.wav");
    if (!explosionSound) {
        std::cout << "Failed to load explosion sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    return true;
}

void AudioManager::playBackgroundMusic() {
    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(backgroundMusic, -1); // -1 để lặp vô hạn
    }
}

void AudioManager::playExplosionSound() {
    Mix_PlayChannel(-1, explosionSound, 0);
}

void AudioManager::stopBackgroundMusic() {
    Mix_HaltMusic();
}
