#include <iostream>
#include <SDL_image.h>
#include "Bomb.h"
#include "Constants.h"

// Khởi tạo đối tượng Bomb với tọa độ và texture
Bomb::Bomb(int x, int y, SDL_Texture* tex, SDL_Renderer* renderer)
{
    rect = {x, y, BOMB_SIZE, BOMB_SIZE};
    texture = tex;

    int offsetX = (50 - 36) / 2;
    int offsetY = (50 - 46) / 2;
    collisionRect = {x + offsetX, y + offsetY, 36, 46};
    collisionRect.x += collisionRect.w * 0.1;
    collisionRect.y += collisionRect.h * 0.1;
    collisionRect.w *= 0.8;
    collisionRect.h *= 0.8;
}

// Cập nhật vị trí của Bomb, di chuyển sang trái với tốc độ cho trước
void Bomb::update(int speed)
{
    rect.x -= speed;
    int offsetX = (50 - 36) / 2;
    int offsetY = (50 - 46) / 2;
    collisionRect.x = rect.x + offsetX + (36 * 0.1);
    collisionRect.y = rect.y + offsetY + (46 * 0.1);
    collisionRect.w = 36 * 0.8;
    collisionRect.h = 46 * 0.8;
}

// Vẽ Bomb lên màn hình
void Bomb::render(SDL_Renderer* renderer, int shakeOffsetX, int shakeOffsetY)
{
    SDL_Rect rectAdjusted = {rect.x + shakeOffsetX, rect.y + shakeOffsetY, rect.w, rect.h};
    SDL_RenderCopy(renderer, texture, nullptr, &rectAdjusted);
}
