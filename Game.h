#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <vector>
#include "Bird.h"
#include "Pipe.h"
#include "Bomb.h"
#include "AudioManager.h"
#include "Screen.h"

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Bird bird;
    std::vector<Pipe> pipes;
    std::vector<Bomb> bombs;
    AudioManager audio;
    Screen screen;
    int pipeTimer;
    int bombTimer;
    int score;
    bool running;
    bool inStartScreen;
    bool inGameOverScreen;

    bool initSDL();
    void handleEvents();
    void update();
    void render();
    void clean();

public:
    Game();
    ~Game();
    void run();
};

#endif
