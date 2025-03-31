#include <iostream>
#include <SDL_image.h>
#include "Rocket.h"
#include "Constants.h"

Rocket::Rocket(int x, SDL_Texture* tex, SDL_Renderer* renderer)
{
    rect = {x, 0, ROCKET_SIZE, ROCKET_SIZE};
    texture = tex;
}
void Rocket::update(int verticalSpeed, int horizontalSpeed)
{
    rect.y += verticalSpeed;
    rect.x -= horizontalSpeed;
}
void Rocket::render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}
