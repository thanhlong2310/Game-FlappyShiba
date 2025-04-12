#include <iostream>
#include <SDL_image.h>
#include "Rocket.h"
#include "Constants.h"

// Khởi tạo đối tượng Rocket với tọa độ và texture
Rocket::Rocket(int x, SDL_Texture* tex, SDL_Renderer* renderer)
{
    rect = {x, 0, ROCKET_SIZE, ROCKET_SIZE};
    texture = tex;

    int offsetX = (100 - 54) / 2;
    int offsetY = (100 - 88) / 2;
    collisionRect = {x + offsetX, offsetY, 54, 88};
    collisionRect.x += collisionRect.w * 0.1;
    collisionRect.y += collisionRect.h * 0.1;
    collisionRect.w *= 0.8;
    collisionRect.h *= 0.8;
}

// Cập nhật vị trí của Rocket, di chuyển theo chiều dọc và ngang
void Rocket::update(int fallSpeed, int moveSpeed)
{
    rect.y += fallSpeed; // Tốc độ rơi xuống
    rect.x -= moveSpeed; // Tốc độ di chuyển sang trái
    int offsetX = (100 - 54) / 2;
    int offsetY = (100 - 88) / 2;
    collisionRect.x = rect.x + offsetX + (54 * 0.1);
    collisionRect.y = rect.y + offsetY + (88 * 0.1);
    collisionRect.w = 54 * 0.8;
    collisionRect.h = 88 * 0.8;
}

// Vẽ Rocket lên màn hình
void Rocket::render(SDL_Renderer* renderer, int shakeOffsetX, int shakeOffsetY)
{
    SDL_Rect rectAdjusted = {rect.x + shakeOffsetX, rect.y + shakeOffsetY, rect.w, rect.h};
    SDL_RenderCopy(renderer, texture, nullptr, &rectAdjusted);
}
