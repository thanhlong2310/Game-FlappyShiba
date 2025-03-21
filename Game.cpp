#include <iostream>
#include <SDL_image.h>
#include "Game.h"

Game::Game(SDL_Renderer* rend) : renderer(rend)
{
    bird = new Bird(renderer);
    audioManager = new AudioManager();
    startScreen = new Screen("assets/start.png", renderer);
    gameOverScreen = new Screen("assets/gameover.png", renderer);
    running = true;
    gameStarted = false;
    isExploding = false;
    score = 0;
    frameCount = 0;
    explodeTimer = 0;
}
Game::~Game()
{
    delete bird;
    for (auto pipe : pipes) delete pipe;
    for (auto bomb : bombs) delete bomb;
    delete audioManager;
    delete startScreen;
    delete gameOverScreen;
}
void Game::run()
{
    audioManager->playBackgroundMusic();
    while (running)
    {
        handleEvents();
        update();
        render();
        if(isExploding)
        {
            SDL_Delay(1000);
            running = false;
        }
    }
    audioManager->stopBackgroundMusic();
}
void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_SPACE)
            {
                if (!gameStarted) gameStarted = true;
                else
                {
                    bird->jump();
                    audioManager->playJumpSound();
                }
            }
        }
    }
}
void Game::update()
{
    if (!gameStarted || isExploding) return;
    bird->update();
    frameCount++;
    if (frameCount % 90 == 0) spawnPipe();
    if (frameCount % BOMB_SPAWN_RATE == 0) spawnBomb();
    for (auto it = pipes.begin(); it != pipes.end();)
    {
        (*it)->update();
        if ((*it)->upperRect.x < -100)
        {
            delete *it;
            it = pipes.erase(it);
        }
        else ++it;
    }
    for (auto it = bombs.begin(); it != bombs.end();)
    {
        (*it)->update();
        if ((*it)->rect.x < -BOMB_SIZE)
        {
            delete *it;
            it = bombs.erase(it);
        }
        else ++it;
    }
    checkCollisions();
    updateScore();
}
void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    if (!gameStarted)
    {
        startScreen->render(renderer);
    }
    else
    {
        SDL_Surface* bgSurface = IMG_Load("assets/background.png");
        SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
        SDL_RenderCopy(renderer, bgTexture, nullptr, nullptr);
        SDL_FreeSurface(bgSurface);
        SDL_DestroyTexture(bgTexture);
        bird->render(renderer);
        for (auto pipe : pipes) pipe->render(renderer);
        for (auto bomb : bombs) bomb->render(renderer);
    }
    if (!running && !isExploding)
    {
        gameOverScreen->render(renderer);
    }
    SDL_RenderPresent(renderer);
}
void Game::spawnPipe()
{
    pipes.push_back(new Pipe(SCREEN_WIDTH, renderer));
}
void Game::spawnBomb()
{
    int y = rand() % (SCREEN_HEIGHT - BOMB_SIZE);
    bombs.push_back(new Bomb(SCREEN_WIDTH, y, renderer));
}
void Game::checkCollisions()
{
    SDL_Rect birdRect = bird->rect;
    for (auto pipe : pipes)
    {
        if (SDL_HasIntersection(&birdRect, &pipe->upperRect) || SDL_HasIntersection(&birdRect, &pipe->lowerRect))
        {
            audioManager->playExplosionSound();
            isExploding = true;
            return;
        }
    }
    for (auto bomb : bombs)
    {
        if (SDL_HasIntersection(&birdRect, &bomb->rect))
        {
            audioManager->playExplosionSound();
            isExploding = true;
            return;
        }
    }
    if (birdRect.y < 0 || birdRect.y + birdRect.h > SCREEN_HEIGHT)
    {
        audioManager->playExplosionSound();
        isExploding = true;
        return;
    }
}
void Game::updateScore()
{
    for (auto pipe : pipes)
    {
        if (!pipe->passed && pipe->upperRect.x + pipe->upperRect.w < bird->rect.x)
        {
            pipe->passed = true;
            score++;
            std::cout << "Score: " << score << std::endl;
        }
    }
}
