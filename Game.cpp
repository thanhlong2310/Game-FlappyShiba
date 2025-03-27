#include <iostream>
#include <SDL_image.h>
#include "Game.h"
#include "Constants.h"

Game::Game(SDL_Renderer* rend) : renderer(rend)
{
    bird = new Bird(renderer);
    audioManager = new AudioManager();
    startScreen = new Screen("assets/start.png", renderer);
    gameOverScreen = new Screen("assets/gameover.png", renderer);
    running = true;
    gameStarted = false;
    isExploding = false;
    isPaused = false;
    score = 0;
    frameCount = 0;
    explodeTimer = 0;
    pipeSpeed = PIPE_SPEED;
    bombSpeed = BOMB_SPEED;
    bombSpawnRate = 150;
    SDL_Surface* bgSurface = IMG_Load("assets/background.png");
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);
    SDL_Surface* pipeSurface = IMG_Load("assets/pipe.png");
    pipeTexture = SDL_CreateTextureFromSurface(renderer, pipeSurface);
    SDL_FreeSurface(pipeSurface);
    SDL_Surface* bombSurface = IMG_Load("assets/bomb.png");
    bombTexture = SDL_CreateTextureFromSurface(renderer, bombSurface);
    SDL_FreeSurface(bombSurface);
    SDL_Surface* explosionSurface = IMG_Load("assets/explosion_sheet.png");
    if (!explosionSurface)
    {
        std::cout << "Failed to load explosion image! SDL_image Error: " << IMG_GetError() << std::endl;
    }
    explosionTexture = SDL_CreateTextureFromSurface(renderer, explosionSurface);
    SDL_FreeSurface(explosionSurface);
    explosionRect = {0, 0, 150, 150};
    explosionSrcRect = {0, 0, 100, 100};
    explosionFrame = 0;
    explosionFrameCount = 0;
    if (TTF_Init() == -1)
    {
        std::cout << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
    }
    font = TTF_OpenFont("assets/Arial.ttf", 24);
    if (!font)
    {
        std::cout << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
    }
    scoreTexture = nullptr;
    scoreRect = {10, 10, 0, 0};
    updateScoreTexture();
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
    SDL_DestroyTexture(explosionTexture);
    SDL_DestroyTexture(scoreTexture);
    TTF_CloseFont(font);
    TTF_Quit();
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
            if (explodeTimer >= EXPLOSION_FRAMES * EXPLOSION_FRAME_DURATION + 1000)
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
                else if (!isPaused)
                {
                    bird->jump();
                    audioManager->playJumpSound();
                }
            }
            else if (event.key.keysym.sym == SDLK_p)
            {
                if (gameStarted && !isExploding&& explodeTimer < EXPLOSION_FRAMES * EXPLOSION_FRAME_DURATION + 1000)
                {
                    isPaused = !isPaused;
                    if (isPaused)
                    {
                        audioManager->stopBackgroundMusic();
                    }
                    else
                    {
                        audioManager->playBackgroundMusic();
                    }
                }
            }
        }
    }
}
void Game::update()
{
    if (!gameStarted || isExploding || isPaused) return;
    bird->update();
    frameCount++;
    if (frameCount % 90 == 0) spawnPipe();
    if (frameCount % bombSpawnRate == 0) spawnBomb();
    for (auto it = pipes.begin(); it != pipes.end();)
    {
        (*it)->update(pipeSpeed);
        if ((*it)->upperRect.x < -100)
        {
            delete *it;
            it = pipes.erase(it);
        }
        else ++it;
    }
    for (auto it = bombs.begin(); it != bombs.end();)
    {
        (*it)->update(bombSpeed);
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
        if(!isExploding)
        {
            bird->render(renderer);
        }
        for (auto pipe : pipes) pipe->render(renderer);
        for (auto bomb : bombs) bomb->render(renderer);
        SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);
        if (isPaused)
        {
            SDL_Surface* pauseSurface = TTF_RenderText_Solid(font, "Paused", {255, 255, 255, 255});
            SDL_Texture* pauseTexture = SDL_CreateTextureFromSurface(renderer, pauseSurface);
            SDL_Rect pauseRect = {SCREEN_WIDTH / 2 - pauseSurface->w / 2, SCREEN_HEIGHT / 2 - pauseSurface->h / 2, pauseSurface->w, pauseSurface->h};
            SDL_RenderCopy(renderer, pauseTexture, nullptr, &pauseRect);
            SDL_FreeSurface(pauseSurface);
            SDL_DestroyTexture(pauseTexture);
        }
    }
    if (isExploding)
    {
        if(explodeTimer < EXPLOSION_FRAMES * EXPLOSION_FRAME_DURATION)
        {
            explosionFrameCount += explodeTimer - (explosionFrame * EXPLOSION_FRAME_DURATION);
            if (explosionFrameCount >= EXPLOSION_FRAME_DURATION)
            {
                explosionFrame++;
                explosionFrameCount = 0;
            }
            if (explosionFrame < EXPLOSION_FRAMES)
            {
                explosionSrcRect.x = explosionFrame * explosionSrcRect.w;
                SDL_RenderCopy(renderer, explosionTexture, &explosionSrcRect, &explosionRect);
            }
        }
        if (explodeTimer >= EXPLOSION_FRAMES * EXPLOSION_FRAME_DURATION)
        {
            gameOverScreen->render(renderer);
        }
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
    birdRect.x += 10;
    birdRect.y += 10;
    birdRect.w -= 20;
    birdRect.h -= 20;
    for (auto pipe : pipes)
    {
        if (pipe->upperRect.x > SCREEN_WIDTH || pipe->upperRect.x + pipe->upperRect.w < 0) continue;
        SDL_Rect upperRect = pipe->upperRect;
        upperRect.x += 20;
        upperRect.w -= 40;
        SDL_Rect lowerRect = pipe->lowerRect;
        lowerRect.x += 20;
        lowerRect.w -= 40;
        if (SDL_HasIntersection(&birdRect, &pipe->upperRect) || SDL_HasIntersection(&birdRect, &pipe->lowerRect))
        {
            explosionRect.x = birdRect.x - (explosionRect.w / 2);
            explosionRect.y = birdRect.y - (explosionRect.h / 2);
            audioManager->playExplosionSound();
            isExploding = true;
            explosionFrame = 0;
            explosionFrameCount = 0;
            return;
        }
    }
    for (auto it = bombs.begin(); it != bombs.end();)
    {
        if ((*it)->rect.x > SCREEN_WIDTH || (*it)->rect.x + (*it)->rect.w < 0)
        {
            delete *it;
            it = bombs.erase(it);
            continue;
        }
        SDL_Rect bombRect = (*it)->rect;
        bombRect.x += 10;
        bombRect.y += 10;
        bombRect.w -= 20;
        bombRect.h -= 20;
        if (SDL_HasIntersection(&birdRect, &(*it)->rect))
        {
            explosionRect.x = birdRect.x - (explosionRect.w / 2);
            explosionRect.y = birdRect.y - (explosionRect.h / 2);
            audioManager->playExplosionSound();
            isExploding = true;
            explosionFrame = 0;
            explosionFrameCount = 0;
            delete *it;
            it = bombs.erase(it);
            return;
        }
        else
        {
            ++it;
        }
    }
    if (birdRect.y < 0 || birdRect.y + birdRect.h > SCREEN_HEIGHT)
    {
        explosionRect.x = birdRect.x - (explosionRect.w / 2);
        explosionRect.y = birdRect.y - (explosionRect.h / 2);
        audioManager->playExplosionSound();
        isExploding = true;
        explosionFrame = 0;
        explosionFrameCount = 0;
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
            updateScoreTexture();
            if (score % 10 == 0)
            {
                pipeSpeed += 1;
                bombSpeed += 1;
                bombSpawnRate -= 30;
                if (bombSpawnRate < 30) bombSpawnRate = 30;
            }
        }
    }
}
void Game::updateScoreTexture()
{
    if (scoreTexture) SDL_DestroyTexture(scoreTexture);
    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText.c_str(), {255, 255, 255, 255});
    if (!surface)
    {
        std::cout << "Failed to create score surface! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }
    scoreTexture = SDL_CreateTextureFromSurface(renderer, surface);
    scoreRect.w = surface->w;
    scoreRect.h = surface->h;
    SDL_FreeSurface(surface);
}
