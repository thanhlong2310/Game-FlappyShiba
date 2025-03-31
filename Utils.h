#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        std::cout << "Failed to load image: " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

Mix_Chunk* loadSound(const char* path) {
    Mix_Chunk* sound = Mix_LoadWAV(path);
    if (!sound) {
        std::cout << "Failed to load sound: " << path << "! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
    return sound;
}

Mix_Music* loadMusic(const char* path) {
    Mix_Music* music = Mix_LoadMUS(path);
    if (!music) {
        std::cout << "Failed to load music: " << path << "! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
    return music;
}

#endif
