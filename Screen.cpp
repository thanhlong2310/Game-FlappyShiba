#include <iostream>
#include <SDL_image.h>
#include "Screen.h"
#include "Constants.h"

Screen::Screen(const char* imagePath, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(imagePath);
    if(!surface)
    {
        std::cout << "Failed to load screen image! SDL_image Error: " << IMG_GetError() << std::endl;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
}
Screen::~Screen()
{
    SDL_DestroyTexture(texture);
}
void Screen::render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}
