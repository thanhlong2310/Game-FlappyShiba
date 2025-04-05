#ifndef CONSTANTS_H
#define CONSTANTS_H

// Các hằng số cố định trong game
const int SCREEN_WIDTH = 800;                  // Chiều rộng màn hình
const int SCREEN_HEIGHT = 600;                 // Chiều cao màn hình
const int PIPE_SPEED = 3;                      // Tốc độ di chuyển của ống
const int PIPE_GAP = 200;                      // Khoảng cách giữa hai ống trên và dưới
const int BOMB_SPEED = 5;                      // Tốc độ di chuyển của bom
const int BOMB_SIZE = 50;                      // Kích thước của bom
const double BIRD_ANGLE_UP = -20.0;            // Góc nghiêng lên khi nhảy
const double BIRD_ANGLE_DOWN = 20.0;           // Góc nghiêng xuống khi rơi
static const int EXPLOSION_FRAMES = 4;         // Số khung hình của hiệu ứng nổ
static const int EXPLOSION_FRAME_DURATION = 5; // Thời gian mỗi khung hình nổ
const int ROCKET_SPEED = 4;                    // Tốc độ di chuyển của tên lửa
const int ROCKET_SIZE = 100;                   // Kích thước của tên lửa
const int ROCKET_SPAWN_INTERVAL = 600;         // Khoảng thời gian sinh ra tên lửa mới
#endif
