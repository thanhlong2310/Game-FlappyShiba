#ifndef PIPE_H
#define PIPE_H
#include <SDL.h>

class Pipe
{
public:
    SDL_Rect upperRect;
    SDL_Rect lowerRect;
    bool passed;
    SDL_Texture* texture;
    Pipe(int x, SDL_Renderer* renderer);
    void update();
    void render(SDL_Renderer* renderer);
};
#endif
