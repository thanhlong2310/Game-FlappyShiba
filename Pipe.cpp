#include <SDL_image.h>
#include <iostream>
#include <cstdlib>
#include "Pipe.h"
#include "Constants.h"

// Khởi tạo cặp ống với vị trí và texture
Pipe::Pipe(int x, SDL_Texture* upperTex, SDL_Texture* lowerTex, SDL_Renderer* renderer)
{
    const int PIPE_WIDTH = 100;
    int gapY = rand() % (SCREEN_HEIGHT - PIPE_GAP - 100) + 50;
    upperRect = {x, 0, PIPE_WIDTH, gapY};
    lowerRect = {x, gapY + PIPE_GAP, PIPE_WIDTH, SCREEN_HEIGHT - (gapY + PIPE_GAP)};
    passed = false;
    upperTexture = upperTex;
    lowerTexture = lowerTex;
}

// Cập nhật vị trí của cặp ống, di chuyển sang trái
void Pipe::update(int speed)
{
    upperRect.x -= speed;
    lowerRect.x -= speed;
}

// Vẽ cặp ống lên màn hình
void Pipe::render(SDL_Renderer* renderer, int shakeOffsetX, int shakeOffsetY)
{
    const int PIPE_HEIGHT = 300;
    SDL_Rect upperSrcRect = {0, PIPE_HEIGHT - upperRect.h, upperRect.w, upperRect.h};
    SDL_Rect lowerSrcRect = {0, 0, lowerRect.w, lowerRect.h};
    SDL_Rect upperRectAdjusted = {upperRect.x + shakeOffsetX, upperRect.y + shakeOffsetY, upperRect.w, upperRect.h};
    SDL_Rect lowerRectAdjusted = {lowerRect.x + shakeOffsetX, lowerRect.y + shakeOffsetY, lowerRect.w, lowerRect.h};
    SDL_RenderCopy(renderer, upperTexture, &upperSrcRect, &upperRectAdjusted);
    SDL_RenderCopy(renderer, lowerTexture, &lowerSrcRect, &lowerRectAdjusted);
}
