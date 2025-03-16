#include "Bomb.h"
#include "Constants.h"

Bomb::Bomb(int x, int y) {
    rect = {x, y, BOMB_SIZE, BOMB_SIZE};
}

void Bomb::update() {
    rect.x -= BOMB_SPEED;
}
