#include <SDL_image.h>
#include <iostream>
#include <cstdlib>
#include "Pipe.h"
#include "Constants.h"

Pipe::Pipe(int x, SDL_Texture* tex, SDL_Renderer* renderer)
{
    int height = rand() % (SCREEN_HEIGHT - PIPE_GAP - 100) + 50;
    upperRect = {x, 0, 100, height};
    lowerRect = {x, height + PIPE_GAP, 100, SCREEN_HEIGHT - height - PIPE_GAP};
    passed = false;
    texture = tex;
}
void Pipe::update()
{
    upperRect.x -= PIPE_SPEED;
    lowerRect.x -= PIPE_SPEED;
}
void Pipe::render(SDL_Renderer* renderer)
{
    SDL_RenderCopyEx(renderer, texture, nullptr, &upperRect, 0.0, nullptr, SDL_FLIP_VERTICAL);
    SDL_RenderCopy(renderer, texture, nullptr, &lowerRect);
}
