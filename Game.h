#ifndef GAME_H
#define GAME_H
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Bird.h"
#include "Pipe.h"
#include "Bomb.h"
#include "Rocket.h"
#include "AudioManager.h"
#include "Constants.h"

// Lớp chính điều khiển toàn bộ trò chơi
class Game
{
private:
    // Các biến liên quan đến renderer và texture
    SDL_Renderer* renderer;             // Renderer chính của game
    SDL_Texture* backgroundTexture;     // Texture nền
    SDL_Texture* upperPipeTexture;      // Texture ống trên
    SDL_Texture* lowerPipeTexture;      // Texture ống dưới
    SDL_Texture* bombTexture;           // Texture bom
    SDL_Texture* rocketTexture;         // Texture tên lửa
    SDL_Texture* explosionTexture;      // Texture hiệu ứng nổ
    SDL_Texture* scoreTexture;          // Texture điểm số
    SDL_Texture* startButtonTexture;    // Texture nút bắt đầu
    SDL_Texture* flappyBirdTitleTexture;// Texture tiêu đề game
    SDL_Texture* panelTexture;          // Texture bảng chọn cấp độ
    SDL_Texture* level1ButtonTexture;   // Texture nút cấp độ 1
    SDL_Texture* level2ButtonTexture;   // Texture nút cấp độ 2
    SDL_Texture* levelAsianButtonTexture;  // Texture nút cấp độ Asian
    SDL_Texture* highScoreTexture;      // Texture điểm cao nhất
    SDL_Texture* currentScoreTexture;   // Texture điểm hiện tại
    SDL_Texture* replayButtonTexture;   // Texture nút chơi lại
    SDL_Texture* exitButtonTexture;     // Texture nút thoát

    // Các đối tượng trong game
    Bird* bird;                         // Đối tượng chim
    std::vector<Pipe*> pipes;           // Danh sách các cặp ống
    std::vector<Bomb*> bombs;           // Danh sách các quả bom
    std::vector<Rocket*> rockets;       // Danh sách các tên lửa
    AudioManager* audioManager;         // Đối tượng quản lý âm thanh

    // Trạng thái game
    bool running;                       // Trạng thái chạy của game
    bool gameStarted;                   // Game đã bắt đầu chưa
    bool isExploding;                   // Đang trong trạng thái nổ
    bool isPaused;                      // Game đang tạm dừng
    bool isCountingDown;                // Đang đếm ngược để bắt đầu
    bool levelSelection;                // Đang ở màn hình chọn cấp độ

    // Các biến thời gian và đếm
    int countdownTimer;                 // Thời gian đếm ngược
    int countdownNumber;                // Số đếm ngược hiện tại
    int score;                          // Điểm số hiện tại
    int frameCount;                     // Đếm số khung hình
    int rocketFrameCount;               // Đếm khung hình để sinh tên lửa
    int explodeTimer;                   // Thời gian hiệu ứng nổ

    // Các biến tốc độ và vật lý
    int pipeSpeed;                      // Tốc độ di chuyển của ống
    int bombSpeed;                      // Tốc độ di chuyển của bom
    int bombSpawnRate;                  // Tần suất sinh bom
    int rocketFallSpeed;                // Tốc độ rơi của tên lửa
    float gravity;                      // Trọng lực
    float jumpForce;                    // Lực nhảy

    // Biến lưu cấp độ và điểm cao nhất
    int currentLevel;                   // Cấp độ hiện tại
    int highScores[3];                  // Mảng lưu điểm cao nhất cho từng cấp độ

    // Font và các hình chữ nhật
    TTF_Font* font;                     // Font chữ
    SDL_Rect scoreRect;                 // Vị trí hiển thị điểm số
    SDL_Rect explosionRect;             // Vị trí hiệu ứng nổ
    SDL_Rect explosionSrcRect;          // Vùng nguồn của hiệu ứng nổ
    SDL_Rect startButtonRect;           // Vị trí nút bắt đầu
    SDL_Rect flappyBirdTitleRect;       // Vị trí tiêu đề game
    SDL_Rect levelPanelRect;            // Vị trí bảng chọn cấp độ
    SDL_Rect level1ButtonRect;          // Vị trí nút cấp độ 1
    SDL_Rect level2ButtonRect;          // Vị trí nút cấp độ 2
    SDL_Rect levelAsianButtonRect;      // Vị trí nút cấp độ Asian
    SDL_Rect highScoreRect;             // Vị trí hiển thị điểm cao nhất
    SDL_Rect currentScoreRect;          // Vị trí hiển thị điểm hiện tại
    SDL_Rect replayButtonRect;          // Vị trí nút chơi lại
    SDL_Rect gameOverPanelRect;         // Vị trí bảng kết thúc game
    SDL_Rect exitButtonRect;            // Vị trí nút thoát

    // Hiệu ứng nổ và rung màn hình
    int explosionFrame;                 // Khung hình hiện tại của hiệu ứng nổ
    int explosionFrameCount;            // Đếm khung hình cho hiệu ứng nổ
    int shakeTimer;                     // Thời gian rung màn hình
    int shakeIntensity;                 // Độ mạnh của rung
    int shakeOffsetX;                   // Độ lệch X khi rung
    int shakeOffsetY;                   // Độ lệch Y khi rung

    // Các hàm private hỗ trợ
    void updateScoreTexture();          // Cập nhật texture điểm số
    void spawnPipe();                   // Sinh ra cặp ống mới
    void spawnBomb();                   // Sinh ra bom mới
    void spawnRocket();                 // Sinh ra tên lửa mới
    void checkCollisions();             // Kiểm tra va chạm
    void handleCollision();             // Xử lý khi va chạm xảy ra
    void updateScore();                 // Cập nhật điểm số
    void updateHighScoreTexture();      // Cập nhật texture điểm cao nhất
    void updateCurrentScoreTexture();   // Cập nhật texture điểm hiện tại
    void startLevel(int level, float grav, float jump, int pSpeed, int bSpeed, int bSpawnRate, int rSpeed);  // Bắt đầu cấp độ mới
    void resetGame();                   // Đặt lại game về trạng thái ban đầu

public:
    Game(SDL_Renderer* renderer);       // Hàm khởi tạo game
    ~Game();                            // Hàm hủy để giải phóng tài nguyên
    void run();                         // Chạy vòng lặp chính của game
    void handleEvents();                // Xử lý sự kiện đầu vào
    void update();                      // Cập nhật trạng thái game
    void render();                      // Vẽ game lên màn hình
};

#endif
