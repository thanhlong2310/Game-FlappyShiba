#ifndef BOMB_H
#define BOMB_H
#include <SDL.h>

// Lớp đại diện cho quả bom trong game
class Bomb
{
public:
    SDL_Rect rect;        // Vùng hiển thị
    SDL_Rect collisionRect; // Vùng va chạm
    SDL_Texture* texture; // Texture hình ảnh của bom

    Bomb(int x, int y, SDL_Texture* tex, SDL_Renderer* renderer); // Hàm khởi tạo
    void update(int speed);              // Cập nhật vị trí của bom
    void render(SDL_Renderer* renderer); // Vẽ bom lên màn hình
};
#endif
