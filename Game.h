#ifndef GAME_H
#define GAME_H
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
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
    SDL_Texture* pipeTexture;
    SDL_Texture* bombTexture;
    Bird* bird;
    std::vector<Pipe*> pipes;
    std::vector<Bomb*> bombs;
    AudioManager* audioManager;
    Screen* startScreen;
    Screen* gameOverScreen;
    bool running;
    bool gameStarted;
    bool isExploding;
    bool isPaused;
    int score;
    int frameCount;
    int explodeTimer;
    TTF_Font* font;
    SDL_Texture* scoreTexture;
    SDL_Rect scoreRect;
    SDL_Texture* explosionTexture;
    SDL_Rect explosionRect;
    SDL_Rect explosionSrcRect;
    int explosionFrame;
    int explosionFrameCount;
    int pipeSpeed;
    int bombSpeed;
    static const int EXPLOSION_FRAMES = 4;
    static const int EXPLOSION_FRAME_DURATION = 10;
    void updateScoreTexture();
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
    int getPipeSpeed() const { return pipeSpeed; }
    int getBombSpeed() const { return bombSpeed; }
};
#endif
