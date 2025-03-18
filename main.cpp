
#include "AudioManager.h"
#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    // Khởi tạo SDL với video và audio
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Tạo cửa sổ
    SDL_Window* window = SDL_CreateWindow("AudioManager Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Tạo renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Tạo đối tượng AudioManager
    AudioManager audioManager;

    // Phát nhạc nền ngay khi chạy
    audioManager.playBackgroundMusic();

    // Vòng lặp chính
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false; // Thoát khi nhấn nút X trên cửa sổ
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_SPACE) {
                    audioManager.playJumpSound(); // Nhấn Space để phát âm thanh nhảy
                }
                if (event.key.keysym.sym == SDLK_e) {
                    audioManager.playExplosionSound(); // Nhấn E để phát âm thanh va chạm
                }
            }
        }

        // Xóa màn hình (màu đen)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    // Dọn dẹp tài nguyên
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
