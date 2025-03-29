#ifndef PIPE_H
#define PIPE_H
#include <SDL.h>

class Pipe
{
public:
    SDL_Rect upperRect;
    SDL_Rect lowerRect;
    bool passed;
    SDL_Texture* upperTexture;
    SDL_Texture* lowerTexture;
    Pipe(int x, SDL_Texture* upperTex, SDL_Texture* lowerTex, SDL_Renderer* renderer);
    void update(int speed);
    void render(SDL_Renderer* renderer);
};
#endif
