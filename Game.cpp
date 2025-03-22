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
    SDL_Surface* bgSurface = IMG_Load("assets/background.png");
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);
    SDL_Surface* pipeSurface = IMG_Load("assets/pipe.png");
    pipeTexture = SDL_CreateTextureFromSurface(renderer, pipeSurface);
    SDL_FreeSurface(pipeSurface);
    SDL_Surface* bombSurface = IMG_Load("assets/bomb.png");
    bombTexture = SDL_CreateTextureFromSurface(renderer, bombSurface);
    SDL_FreeSurface(bombSurface);
}
Game::~Game()
{
    delete bird;
    for (auto pipe : pipes) delete pipe;
    for (auto bomb : bombs) delete bomb;
    delete audioManager;
    delete startScreen;
    delete gameOverScreen;
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(pipeTexture);
    SDL_DestroyTexture(bombTexture);
}
void Game::run()
{
    audioManager->playBackgroundMusic();
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;
    while (running)
    {
        frameStart = SDL_GetTicks();
        handleEvents();
        update();
        render();
        if(isExploding)
        {
            explodeTimer += frameTime;
            if (explodeTimer >= 1000)
            {
                running = false;
            }
        }
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
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
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
        bird->render(renderer);
        for (auto pipe : pipes) pipe->render(renderer);
        for (auto bomb : bombs) bomb->render(renderer);
    }
    if (isExploding)
    {
        gameOverScreen->render(renderer);
    }
    SDL_RenderPresent(renderer);
}
void Game::spawnPipe()
{
    pipes.push_back(new Pipe(SCREEN_WIDTH, pipeTexture, renderer));
}
void Game::spawnBomb()
{
    int y = rand() % (SCREEN_HEIGHT - BOMB_SIZE);
    bombs.push_back(new Bomb(SCREEN_WIDTH, y, bombTexture, renderer));
}
void Game::checkCollisions()
{
    SDL_Rect birdRect = bird->rect;
    birdRect.x += 17;
    birdRect.y += 17;
    birdRect.w -= 34;
    birdRect.h -= 34;
    for (auto pipe : pipes)
    {
        if (pipe->upperRect.x > SCREEN_WIDTH || pipe->upperRect.x + pipe->upperRect.w < 0) continue;
        SDL_Rect upperRect = pipe->upperRect;
        upperRect.x += 35;
        upperRect.w -= 70;
        SDL_Rect lowerRect = pipe->lowerRect;
        lowerRect.x += 35;
        lowerRect.w -= 70;
        if (SDL_HasIntersection(&birdRect, &pipe->upperRect) || SDL_HasIntersection(&birdRect, &pipe->lowerRect))
        {
            audioManager->playExplosionSound();
            isExploding = true;
            return;
        }
    }
    for (auto bomb : bombs)
    {
        if (bomb->rect.x > SCREEN_WIDTH || bomb->rect.x + bomb->rect.w < 0) continue;
        SDL_Rect bombRect = bomb->rect;
        bombRect.x += 17;
        bombRect.y += 17;
        bombRect.w -= 34;
        bombRect.h -= 34;
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
