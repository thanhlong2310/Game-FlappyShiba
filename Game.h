#ifndef GAME_H
#define GAME_H
#include <vector>
#include <SDL.h>
#include "Bird.h"
#include "Pipe.h"
#include "Bomb.h"
#include "AudioManager.h"
#include "Screen.h"
#include "Constants.h"

class Game
{
private:
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    Bird* bird;
    std::vector<Pipe*> pipes;
    std::vector<Bomb*> bombs;
    AudioManager* audioManager;
    Screen* startScreen;
    Screen* gameOverScreen;
    bool running;
    bool gameStarted;
    bool isExploding;
    int score;
    int frameCount;
    int explodeTimer;
    void spawnPipe();
    void spawnBomb();
    void checkCollisions();
    void updateScore();
public:
    Game(SDL_Renderer* renderer);
    ~Game();
    void run();
    void handleEvents();
    void update();
    void render();
};
#endif
