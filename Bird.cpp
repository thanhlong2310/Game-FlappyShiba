#include <iostream>
#include <SDL_image.h>
#include "Bird.h"
#include "Constants.h"

Bird::Bird(SDL_Renderer* renderer)
{
    rect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, 50, 50};
    velocity = 0.0f;
    SDL_Surface* surface = IMG_Load("assets/shiba.png");
    if(!surface)
    {
        std::cout << "Failed to load shiba image! SDL_image Error: " << IMG_GetError() << std::endl;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}
void Bird::update()
{
    velocity += GRAVITY;
    rect.y += velocity;
}
void Bird::jump()
{
    velocity = JUMP_FORCE;
}
void Bird::render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

