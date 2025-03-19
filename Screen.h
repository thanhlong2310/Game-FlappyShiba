#ifndef SCREEN_H
#define SCREEN_H
#include <SDL.h>

class Screen
{
private:
    SDL_Texture* texture;
    SDL_Rect rect;
public:
    Screen(const char* imagePath, SDL_Renderer* renderer);
    ~Screen();
    void render(SDL_Renderer* renderer);
};
#endif
