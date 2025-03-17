#include <iostream>
#include <SDL_image.h>
#include <Bomb.h>
#include <Constants.h>

Bomb::Bomb(int x, int y, SDL_Renderer* renderer)
{
    rect = {x, y, BOMB_SIZE, BOMB_SIZE};
    SDL_Surface* surface = IMG_Load("assets/bomb.png");
    if (!surface)
    {
        std::cout << "Failed to load bomb image! SDL_image Error: " << IMG_GetError() << std::endl;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}
void Bomb::update()
{
    rect.x -= BOMB_SPEED;
}
void Bomb::render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}
