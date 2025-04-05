#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#include <SDL_mixer.h>

// Lớp quản lý âm thanh trong game
class AudioManager
{
public:
    Mix_Music* backgroundMusic; // Nhạc nền
    Mix_Chunk* jumpSound;       // Âm thanh khi nhảy
    Mix_Chunk* explosionSound;  // Âm thanh khi va chạm nổ

    AudioManager();             // Hàm khởi tạo âm thanh
    ~AudioManager();            // Hàm hủy để giải phóng tài nguyên
    void playBackgroundMusic(); // Phát nhạc nền
    void stopBackgroundMusic(); // Dừng nhạc nền
    void playJumpSound();       // Phát âm thanh nhảy
    void playExplosionSound();  // Phát âm thanh nổ
};
#endif
