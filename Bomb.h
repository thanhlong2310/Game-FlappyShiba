#ifndef BOMB_H
#define BOMB_H
#include <SDL.h>

class Bomb
{
public:
    SDL_Rect rect;
    SDL_Texture* texture;
    Bomb(int x, int y, SDL_Texture* tex, SDL_Renderer* renderer);
    void update();
    void render(SDL_Renderer* renderer);
};
#endif
