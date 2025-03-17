#ifndef BIRD_H
#define BIRD_H
#include <SDL.h>

class Bird
{
public:
    SDL_Rect rect;
    int velocity;
    SDL_Texture* texture;
    Bird(SDL_Renderer* renderer);
    void update();
    void jump();
    void render(SDL_Renderer* renderer);
};
#endif
