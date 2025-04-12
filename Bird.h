#ifndef BIRD_H
#define BIRD_H
#include <SDL.h>

// Lớp đại diện cho shiba
class Bird
{
public:
    SDL_Rect rect;        // Vùng hiển thị
    SDL_Rect collisionRect; // Vùng va chạm
    float velocity;       // Vận tốc rơi của chim
    SDL_Texture* texture; // Texture hình ảnh của chim
    double angle;         // Góc xoay của chim
    float gravity;        // Trọng lực tác động lên chim
    float jumpForce;      // Lực nhảy của chim

    Bird(SDL_Renderer* renderer);        // Hàm khởi tạo
    void update();                       // Cập nhật trạng thái của chim
    void jump();                         // Thực hiện hành động nhảy
    void render(SDL_Renderer* renderer); // Vẽ chim lên màn hình
};
#endif
