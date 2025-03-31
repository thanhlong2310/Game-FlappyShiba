#ifndef ROCKET_H
#define ROCKET_H
#include <SDL.h>

class Rocket
{
public:
    SDL_Rect rect;
    SDL_Texture* texture;
    Rocket(int x, SDL_Texture* tex, SDL_Renderer* renderer);
    void update(int verticalSpeed, int horizontalSpeed);
    void render(SDL_Renderer* renderer);
};
#endif
