#include <iostream>
#include <SDL_image.h>
#include "Bomb.h"
#include "Constants.h"

Bomb::Bomb(int x, int y, SDL_Texture* tex, SDL_Renderer* renderer)
{
    rect = {x, y, BOMB_SIZE, BOMB_SIZE};
    texture = tex;
}
void Bomb::update()
{
    rect.x -= BOMB_SPEED;
}
void Bomb::render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}
