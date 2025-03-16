#ifndef PIPE_H
#define PIPE_H

#include <SDL.h>

class Pipe {
public:
    SDL_Rect upperRect;
    SDL_Rect lowerRect;
    bool passed;

    Pipe(int x);
    void update();
};

#endif
