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
    int bombSpawnRate;
    float gravity;
    float jumpForce;
    SDL_Texture* startButtonTexture;
    SDL_Rect startButtonRect;
    bool levelSelection;
    SDL_Texture* panelTexture;
    SDL_Rect levelPanelRect;
    SDL_Texture* level1ButtonTexture;
    SDL_Rect level1ButtonRect;
    SDL_Texture* levelAsianButtonTexture;
    SDL_Rect levelAsianButtonRect;
    int currentLevel;
    int highScore;
    SDL_Texture* highScoreTexture;
    SDL_Rect highScoreRect;
    SDL_Texture* currentScoreTexture;
    SDL_Rect currentScoreRect;
    SDL_Texture* replayButtonTexture;
    SDL_Rect replayButtonRect;
    SDL_Rect gameOverPanelRect;
    void updateScoreTexture();
    void spawnPipe();
    void spawnBomb();
    void checkCollisions();
    void updateScore();
    void updateHighScoreTexture();
    void updateCurrentScoreTexture();
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
