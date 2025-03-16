#include "Bird.h"
#include "Constants.h"

Bird::Bird() {
    rect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, 50, 50};
    velocity = 0;
}

void Bird::update() {
    velocity += GRAVITY;
    rect.y += velocity;
}

void Bird::jump() {
    velocity = JUMP_FORCE;
}
