#ifndef BIRD_H
#define BIRD_H

#include <SDL.h>

class Bird {
public:
    SDL_Rect rect;
    int velocity;

    Bird();
    void update();
    void jump();
};

#endif
