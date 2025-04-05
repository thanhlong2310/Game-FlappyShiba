#include <iostream>
#include <SDL_image.h>
#include "Bird.h"
#include "Constants.h"

// Khởi tạo đối tượng Bird với renderer, tải hình ảnh và đặt giá trị ban đầu
Bird::Bird(SDL_Renderer* renderer)
{
    rect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, 50, 50};
    collisionRect = rect;
    collisionRect.x += rect.w * 0.1;
    collisionRect.y += rect.h * 0.1;
    collisionRect.w *= 0.8;
    collisionRect.h *= 0.8;

    velocity = 0.0f;
    angle = 0.0;
    gravity = 0.2f;
    jumpForce = -6.0f;

    SDL_Surface* surface = IMG_Load("assets/shiba.png");
    if(!surface)
    {
        std::cout << "Failed to load shiba image! SDL_image Error: " << IMG_GetError() << std::endl;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

// Cập nhật vị trí và góc của Bird dựa trên vận tốc và trọng lực
void Bird::update()
{
    velocity += gravity;
    rect.y += velocity;
    if (velocity < 0)
    {
        angle = BIRD_ANGLE_UP;
    }
    else if (velocity > 0)
    {
        angle = BIRD_ANGLE_DOWN;
    }

    collisionRect.x = rect.x + (rect.w * 0.1);
    collisionRect.y = rect.y + (rect.h * 0.1);
    collisionRect.w = rect.w * 0.8;
    collisionRect.h = rect.h * 0.8;
}

// Thực hiện hành động nhảy, đặt lại vận tốc và góc
void Bird::jump()
{
    velocity = jumpForce;
    angle = BIRD_ANGLE_UP;
}

// Vẽ Bird lên màn hình với góc xoay phù hợp
void Bird::render(SDL_Renderer* renderer)
{
    SDL_RenderCopyEx(renderer, texture, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);
}
