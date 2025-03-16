#include "Screen.h"
#include <SDL_image.h>
#include <iostream>

Screen::Screen() : startTexture(nullptr), gameOverTexture(nullptr) {}

Screen::~Screen() {
    SDL_DestroyTexture(startTexture);
    SDL_DestroyTexture(gameOverTexture);
}

bool Screen::loadTextures(SDL_Renderer* renderer) {
    SDL_Surface* startSurface = IMG_Load("assets/start.png");
    if (!startSurface) {
        std::cout << "Failed to load start image! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }
    startTexture = SDL_CreateTextureFromSurface(renderer, startSurface);
    SDL_FreeSurface(startSurface);

    SDL_Surface* gameOverSurface = IMG_Load("assets/gameover.png");
    if (!gameOverSurface) {
        std::cout << "Failed to load gameover image! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }
    gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
    SDL_FreeSurface(gameOverSurface);

    return true;
}

void Screen::renderStartScreen(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, startTexture, nullptr, nullptr);
}

void Screen::renderGameOverScreen(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, gameOverTexture, nullptr, nullptr);
}
