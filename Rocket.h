#ifndef ROCKET_H
#define ROCKET_H
#include <SDL.h>

// Lớp đại diện cho tên lửa
class Rocket
{
public:
    SDL_Rect rect;        // Vùng hiển thị
    SDL_Rect collisionRect; // Vùng va chạm
    SDL_Texture* texture; // Texture hình ảnh của tên lửa

    Rocket(int x, SDL_Texture* tex, SDL_Renderer* renderer); // Hàm khởi tạo
    void update(int fallSpeed, int moveSpeed); // Cập nhật vị trí của tên lửa
    void render(SDL_Renderer* renderer, int shakeOffsetX, int shakeOffsetY); // Vẽ tên lửa lên màn hình
};
#endif
