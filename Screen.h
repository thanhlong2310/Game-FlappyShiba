#ifndef SCREEN_H
#define SCREEN_H

#include <SDL.h>

class Screen {
private:
    SDL_Texture* startTexture;
    SDL_Texture* gameOverTexture;

public:
    Screen();
    ~Screen();
    bool loadTextures(SDL_Renderer* renderer);
    void renderStartScreen(SDL_Renderer* renderer);
    void renderGameOverScreen(SDL_Renderer* renderer);
};

#endif
