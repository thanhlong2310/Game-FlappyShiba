#include <iostream>
#include <SDL_image.h>
#include "Bird.h"
#include "Constants.h"

Bird::Bird(SDL_Renderer* renderer)
{
    rect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, 50, 50};
    velocity = 0.0f;
    angle = 0.0;
    this->gravity = gravity;
    this->jumpForce = jumpForce;
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
    velocity += gravity;
    rect.y += velocity;
    if (velocity < 0)
    {
        angle = BIRD_ANGLE_UP;
    }
    else if (velocity > 0)
    {
        angle = BIRD_ANGLE_DOWN;
    }
}
void Bird::jump()
{
    velocity = jumpForce;
    angle = BIRD_ANGLE_UP;
}
void Bird::render(SDL_Renderer* renderer)
{
    SDL_RenderCopyEx(renderer, texture, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);
}
