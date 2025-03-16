#include "Pipe.h"
#include "Constants.h"

Pipe::Pipe(int x) {
    int height = rand() % (SCREEN_HEIGHT - PIPE_GAP - 100) + 50;
    upperRect = {x, 0, 100, height};
    lowerRect = {x, height + PIPE_GAP, 100, SCREEN_HEIGHT};
    passed = false;
}

void Pipe::update() {
    upperRect.x -= PIPE_SPEED;
    lowerRect.x -= PIPE_SPEED;
}
