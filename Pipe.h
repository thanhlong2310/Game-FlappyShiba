#ifndef PIPE_H
#define PIPE_H
#include <SDL.h>

// Lớp đại diện cho cặp ống trong game
class Pipe
{
public:
    SDL_Rect upperRect;        // Hình chữ nhật ống trên
    SDL_Rect lowerRect;        // Hình chữ nhật ống dưới
    bool passed;               // Biến kiểm tra chim đã vượt qua chưa
    SDL_Texture* upperTexture; // Texture ống trên
    SDL_Texture* lowerTexture; // Texture ống dưới

    Pipe(int x, SDL_Texture* upperTex, SDL_Texture* lowerTex, SDL_Renderer* renderer); // Hàm khởi tạo
    void update(int speed);              // Cập nhật vị trí của ống
    void render(SDL_Renderer* renderer, int shakeOffsetX, int shakeOffsetY); // Vẽ ống lên màn hình
};
#endif
