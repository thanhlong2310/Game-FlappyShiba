#ifndef BOMB_H
#define BOMB_H

#include <SDL.h>

class Bomb {
public:
    SDL_Rect rect;

    Bomb(int x, int y);
    void update();
};

#endif
