#include <SDL_image.h>
#include <iostream>
#include <cstdlib>
#include "Pipe.h"
#include "Constants.h"

Pipe::Pipe(int x, SDL_Renderer* renderer)
{
    int height = rand() % (SCEEN_HEIGHT - PIPE_GAP - 100) + 50;
    upperRect = {x, 0, 100, height};
    lowerRect = {x, height + PIPE_GAP, 100, SCREEN_HEIGHT - height - PIPE_GAP};
    passed = false;
    SDL_Surface* surface = IMG_Load("assets/pipe.png");
    if (!surface)
    {
        std::cout << "Failed to load pipe image! SDL_image Error: " << IMG_GetError() << std::endl;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}
void Pipe::update()
{
    upperRect.x -= PIPE_SPEED;
    lowerRect.x -= PIPE_SPEED;
}
void Pipe::render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, texture, nullptr, &upperRect);
    SDL_RenderCopy(renderer, texture, nullptr, &lowerRect);
}
