#include "Game.h"
#include "Constants.h"
#include <SDL_image.h>
#include <iostream>

Game::Game() : window(nullptr), renderer(nullptr), pipeTimer(0), bombTimer(0),
              score(0), running(false), inStartScreen(true), inGameOverScreen(false) {}

Game::~Game() {
    clean();
}

bool Game::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return false;
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return false;

    window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                             SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;

    if (!audio.init() || !screen.loadTextures(renderer)) return false;

    return true;
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            running = false;
        }
        else if (e.type == SDL_KEYDOWN) {
            if (inStartScreen && e.key.keysym.sym == SDLK_SPACE) {
                inStartScreen = false;
                audio.playBackgroundMusic();
            }
            else if (inGameOverScreen && e.key.keysym.sym == SDLK_SPACE) {
                // Reset game
                bird = Bird();
                pipes.clear();
                bombs.clear();
                score = 0;
                pipeTimer = 0;
                bombTimer = 0;
                inGameOverScreen = false;
                audio.playBackgroundMusic();
            }
            else if (!inStartScreen && !inGameOverScreen && e.key.keysym.sym == SDLK_SPACE) {
                bird.jump();
            }
        }
    }
}

void Game::update() {
    if (inStartScreen || inGameOverScreen) return;

    bird.update();

    pipeTimer++;
    if (pipeTimer > 100) {
        pipes.push_back(Pipe(SCREEN_WIDTH));
        pipeTimer = 0;
    }

    bombTimer++;
    if (bombTimer > BOMB_SPAWN_RATE) {
        int y = rand() % (SCREEN_HEIGHT - BOMB_SIZE);
        bombs.push_back(Bomb(SCREEN_WIDTH, y));
        bombTimer = 0;
    }

    for (auto it = pipes.begin(); it != pipes.end();) {
        it->update();
        if (SDL_HasIntersection(&bird.rect, &it->upperRect) ||
            SDL_HasIntersection(&bird.rect, &it->lowerRect)) {
            audio.stopBackgroundMusic();
            audio.playExplosionSound();
            inGameOverScreen = true;
        }
        if (!it->passed && it->upperRect.x + it->upperRect.w < bird.rect.x) {
            it->passed = true;
            score++;
        }
        if (it->upperRect.x < -it->upperRect.w) {
            it = pipes.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = bombs.begin(); it != bombs.end();) {
        it->update();
        if (SDL_HasIntersection(&bird.rect, &it->rect)) {
            audio.stopBackgroundMusic();
            audio.playExplosionSound();
            inGameOverScreen = true;
        }
        if (it->rect.x < -it->rect.w) {
            it = bombs.erase(it);
        } else {
            ++it;
        }
    }

    if (bird.rect.y < 0 || bird.rect.y > SCREEN_HEIGHT) {
        audio.stopBackgroundMusic();
        audio.playExplosionSound();
        inGameOverScreen = true;
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
    SDL_RenderClear(renderer);

    if (inStartScreen) {
        screen.renderStartScreen(renderer);
    }
    else if (inGameOverScreen) {
        screen.renderGameOverScreen(renderer);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (const auto& pipe : pipes) {
            SDL_RenderFillRect(renderer, &pipe.upperRect);
            SDL_RenderFillRect(renderer, &pipe.lowerRect);
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Bom màu đỏ
        for (const auto& bomb : bombs) {
            SDL_RenderFillRect(renderer, &bomb.rect);
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &bird.rect);
    }

    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void Game::run() {
    if (!initSDL()) return;

    running = true;
    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(16);
    }

    std::cout << "Final Score: " << score << std::endl;
    clean();
}
