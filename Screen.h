#ifndef SCREEN_H
#define SCREEN_H
#include <SDL.h>

// Lớp đại diện cho nền trong game
class Screen
{
private:
    SDL_Texture* texture; // Texture của màn hình
    SDL_Rect rect;        // Hình chữ nhật đại diện vị trí và kích thước

public:
    Screen(const char* imagePath, SDL_Renderer* renderer); // Hàm khởi tạo với đường dẫn ảnh
    ~Screen();                                             // Hàm hủy để giải phóng tài nguyên
    void render(SDL_Renderer* renderer);                   // Vẽ màn hình lên renderer
};
#endif
