#include <iostream>
#include <SDL_image.h>
#include "Game.h"

Game::Game(SDL_Renderer* rend) : renderer(rend)
{
    gravity = 0.2f;
    jumpForce = -6.0f;
    bird = new Bird(renderer);
    audioManager = new AudioManager();
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
    SDL_Surface* upperPipeSurface = IMG_Load("assets/upper_pipe.png");
    if (!upperPipeSurface)
    {
        std::cout << "Failed to load upper pipe image! SDL_image Error: " << IMG_GetError() << std::endl;
    }
    upperPipeTexture = SDL_CreateTextureFromSurface(renderer, upperPipeSurface);
    SDL_FreeSurface(upperPipeSurface);
    SDL_Surface* lowerPipeSurface = IMG_Load("assets/lower_pipe.png");
    if (!lowerPipeSurface)
    {
        std::cout << "Failed to load lower pipe image! SDL_image Error: " << IMG_GetError() << std::endl;
    }
    lowerPipeTexture = SDL_CreateTextureFromSurface(renderer, lowerPipeSurface);
    SDL_FreeSurface(lowerPipeSurface);
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
    SDL_Surface* startButtonSurface = IMG_Load("assets/start_button.png");
    startButtonTexture = SDL_CreateTextureFromSurface(renderer, startButtonSurface);
    SDL_FreeSurface(startButtonSurface);
    startButtonRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT * 2 / 3, 100, 50};
    SDL_Surface* flappyBirdTitleSurface = IMG_Load("assets/flappybird_title.png");
    if (!flappyBirdTitleSurface)
    {
        std::cout << "Failed to load flappy bird title image! SDL_image Error: " << IMG_GetError() << std::endl;
    }
    flappyBirdTitleTexture = SDL_CreateTextureFromSurface(renderer, flappyBirdTitleSurface);
    flappyBirdTitleRect = {SCREEN_WIDTH / 2 - 350, SCREEN_HEIGHT / 3 - 50, 700, 200};
    SDL_FreeSurface(flappyBirdTitleSurface);
    levelSelection = false;
    SDL_Surface* panelSurface = IMG_Load("assets/panel.png");
    panelTexture = SDL_CreateTextureFromSurface(renderer, panelSurface);
    SDL_FreeSurface(panelSurface);
    levelPanelRect = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 100, 300, 270};
    SDL_Surface* level1Surface = IMG_Load("assets/level1_button.png");
    level1ButtonTexture = SDL_CreateTextureFromSurface(renderer, level1Surface);
    SDL_FreeSurface(level1Surface);
    level1ButtonRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 75, 100, 50};
    SDL_Surface* level2Surface = IMG_Load("assets/level2_button.png");
    level2ButtonTexture = SDL_CreateTextureFromSurface(renderer, level2Surface);
    SDL_FreeSurface(level2Surface);
    level2ButtonRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 10, 100, 50};
    SDL_Surface* levelAsianSurface = IMG_Load("assets/levelasian_button.png");
    levelAsianButtonTexture = SDL_CreateTextureFromSurface(renderer, levelAsianSurface);
    SDL_FreeSurface(levelAsianSurface);
    levelAsianButtonRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 95, 100, 50};
    currentLevel = 0;
    for (int i = 0; i < 3; i++)
    {
        highScores[i] = 0;
    }
    highScoreTexture = nullptr;
    highScoreRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 75, 0, 0};
    currentScoreTexture = nullptr;
    currentScoreRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 25, 0, 0};
    updateHighScoreTexture();
    updateHighScoreTexture();
    SDL_Surface* replaySurface = IMG_Load("assets/replay_button.png");
    replayButtonTexture = SDL_CreateTextureFromSurface(renderer, replaySurface);
    SDL_FreeSurface(replaySurface);
    replayButtonRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 25, 100, 50};
    SDL_Surface* exitSurface = IMG_Load("assets/exit_button.png");
    if (!exitSurface)
    {
        std::cout << "Failed to load exit button image! SDL_image Error: " << IMG_GetError() << std::endl;
    }
    exitButtonTexture = SDL_CreateTextureFromSurface(renderer, exitSurface);
    SDL_FreeSurface(exitSurface);
    exitButtonRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 95, 100, 50};
    gameOverPanelRect = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 100, 300, 280};
}
Game::~Game()
{
    delete bird;
    for (auto pipe : pipes) delete pipe;
    for (auto bomb : bombs) delete bomb;
    delete audioManager;
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(upperPipeTexture);
    SDL_DestroyTexture(lowerPipeTexture);
    SDL_DestroyTexture(bombTexture);
    SDL_DestroyTexture(explosionTexture);
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(startButtonTexture);
    SDL_DestroyTexture(flappyBirdTitleTexture);
    SDL_DestroyTexture(panelTexture);
    SDL_DestroyTexture(level1ButtonTexture);
    SDL_DestroyTexture(level2ButtonTexture);
    SDL_DestroyTexture(levelAsianButtonTexture);
    SDL_DestroyTexture(highScoreTexture);
    SDL_DestroyTexture(currentScoreTexture);
    SDL_DestroyTexture(replayButtonTexture);
    SDL_DestroyTexture(exitButtonTexture);
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
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            int x = event.button.x;
            int y = event.button.y;
            if (!gameStarted && !levelSelection)
            {
                if (x >= startButtonRect.x && x <= startButtonRect.x + startButtonRect.w && y >= startButtonRect.y && y <= startButtonRect.y + startButtonRect.h)
                {
                    levelSelection = true;
                }
            }
            else if (levelSelection)
            {
                if (x >= level1ButtonRect.x && x <= level1ButtonRect.x + level1ButtonRect.w && y >= level1ButtonRect.y && y <= level1ButtonRect.y + level1ButtonRect.h)
                {
                    currentLevel = 1;
                    gameStarted = true;
                    levelSelection = false;
                    gravity = 0.2f;
                    jumpForce = -6.0f;
                    pipeSpeed = PIPE_SPEED;
                    bombSpeed = BOMB_SPEED;
                    bombSpawnRate = 150;
                    bird->gravity = gravity;
                    bird->jumpForce = jumpForce;
                }
                else if (x >= level2ButtonRect.x && x <= level2ButtonRect.x + level2ButtonRect.w && y >= level2ButtonRect.y && y <= level2ButtonRect.y + level2ButtonRect.h)
                {
                    currentLevel = 2;
                    gameStarted = true;
                    levelSelection = false;
                    gravity = 0.2f * 1.5;
                    jumpForce = -6.0f * 1.5;
                    pipeSpeed = PIPE_SPEED * 1.5;
                    bombSpeed = BOMB_SPEED * 1.5;
                    bombSpawnRate = 90;
                    bird->gravity = gravity;
                    bird->jumpForce = jumpForce;
                }
                else if (x >= levelAsianButtonRect.x && x <= levelAsianButtonRect.x + levelAsianButtonRect.w && y >= levelAsianButtonRect.y && y <= levelAsianButtonRect.y + levelAsianButtonRect.h)
                {
                    currentLevel = 3;
                    gameStarted = true;
                    levelSelection = false;
                    gravity = 0.2f * 3;
                    jumpForce = -6.0f * 3;
                    pipeSpeed = PIPE_SPEED * 3;
                    bombSpeed = BOMB_SPEED * 3;
                    bombSpawnRate = 10;
                    bird->gravity = gravity;
                    bird->jumpForce = jumpForce;
                }
            }
            else if (isExploding && explodeTimer >= EXPLOSION_FRAMES * EXPLOSION_FRAME_DURATION)
            {
                if (x >= replayButtonRect.x && x <= replayButtonRect.x + replayButtonRect.w && y >= replayButtonRect.y && y <= replayButtonRect.y + replayButtonRect.h)
                {
                    score = 0;
                    frameCount = 0;
                    explodeTimer = 0;
                    isExploding = false;
                    if (currentLevel == 1)
                    {
                        gravity = 0.2f;
                        jumpForce = -6.0f;
                        pipeSpeed = PIPE_SPEED;
                        bombSpeed = BOMB_SPEED;
                        bombSpawnRate = 150;
                    }
                    else if (currentLevel == 2)
                    {
                        gravity = 0.2f * 1.5;
                        jumpForce = -6.0f * 1.5;
                        pipeSpeed = PIPE_SPEED * 1.5;
                        bombSpeed = BOMB_SPEED * 1.5;
                        bombSpawnRate = 90;
                    }
                    else if (currentLevel == 3)
                    {
                        gravity = 0.2f * 3;
                        jumpForce = -6.0f * 3;
                        pipeSpeed = PIPE_SPEED * 3;
                        bombSpeed = BOMB_SPEED * 3;
                        bombSpawnRate = 60;
                    }
                    bird->gravity = gravity;
                    bird->jumpForce = jumpForce;
                    bird->rect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, 50, 50};
                    bird->velocity = 0;
                    bird->angle = 0;
                    for (auto pipe : pipes) delete pipe;
                    pipes.clear();
                    for (auto bomb : bombs) delete bomb;
                    bombs.clear();
                    updateScoreTexture();
                    updateHighScoreTexture();
                    updateCurrentScoreTexture();
                }
                else if (x >= exitButtonRect.x && x <= exitButtonRect.x + exitButtonRect.w && y >= exitButtonRect.y && y <= exitButtonRect.y + exitButtonRect.h)
                {
                    gameStarted = false;
                    levelSelection = true;
                    isExploding = false;
                    explodeTimer = 0;
                    score = 0;
                    frameCount = 0;
                    bird->rect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, 50, 50};
                    bird->velocity = 0;
                    bird->angle = 0;
                    for (auto pipe : pipes) delete pipe;
                    pipes.clear();
                    for (auto bomb : bombs) delete bomb;
                    bombs.clear();
                    updateScoreTexture();
                    updateHighScoreTexture();
                    updateCurrentScoreTexture();
                }
            }
        }
        if (event.type == SDL_KEYDOWN && gameStarted && !levelSelection)
        {
            if (event.key.keysym.sym == SDLK_SPACE)
            {
                if (!isPaused)
                {
                    bird->jump();
                    audioManager->playJumpSound();
                }
            }
            else if (event.key.keysym.sym == SDLK_p)
            {
                if (!isExploding && explodeTimer < EXPLOSION_FRAMES * EXPLOSION_FRAME_DURATION + 1000)
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
    if (!gameStarted && !levelSelection)
    {
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
        SDL_RenderCopy(renderer, flappyBirdTitleTexture, nullptr, &flappyBirdTitleRect);
        SDL_RenderCopy(renderer, startButtonTexture, nullptr, &startButtonRect);
    }
    else if (levelSelection)
    {
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
        SDL_RenderCopy(renderer, panelTexture, nullptr, &levelPanelRect);
        SDL_RenderCopy(renderer, level1ButtonTexture, nullptr, &level1ButtonRect);
        SDL_RenderCopy(renderer, level2ButtonTexture, nullptr, &level2ButtonRect);
        SDL_RenderCopy(renderer, levelAsianButtonTexture, nullptr, &levelAsianButtonRect);
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
        else
        {
            SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
            SDL_RenderCopy(renderer, panelTexture, nullptr, &gameOverPanelRect);
            SDL_RenderCopy(renderer, highScoreTexture, nullptr, &highScoreRect);
            SDL_RenderCopy(renderer, currentScoreTexture, nullptr, &currentScoreRect);
            SDL_RenderCopy(renderer, replayButtonTexture, nullptr, &replayButtonRect);
            SDL_RenderCopy(renderer, exitButtonTexture, nullptr, &exitButtonRect);
        }
    }
    SDL_RenderPresent(renderer);
}
void Game::spawnPipe()
{
    pipes.push_back(new Pipe(SCREEN_WIDTH, upperPipeTexture, lowerPipeTexture, renderer));
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
            if (score > highScores[currentLevel - 1])
            {
                highScores[currentLevel - 1] = score;
            }
            updateHighScoreTexture();
            updateCurrentScoreTexture();
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
            if (score > highScores[currentLevel - 1])
            {
                highScores[currentLevel - 1] = score;
            }
            updateHighScoreTexture();
            updateCurrentScoreTexture();
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
        if (score > highScores[currentLevel - 1])
        {
            highScores[currentLevel - 1] = score;
        }
        updateHighScoreTexture();
        updateCurrentScoreTexture();
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
void Game::updateHighScoreTexture()
{
    if (highScoreTexture)
    {
        SDL_DestroyTexture(highScoreTexture);
    }
    std::string highScoreText = "High Score: " + std::to_string(highScores[currentLevel - 1]);
    SDL_Surface* highScoreSurface = TTF_RenderText_Solid(font, highScoreText.c_str(), {255, 255, 255, 255});
    highScoreTexture = SDL_CreateTextureFromSurface(renderer, highScoreSurface);
    highScoreRect.w = highScoreSurface->w;
    highScoreRect.h = highScoreSurface->h;
    SDL_FreeSurface(highScoreSurface);
}
void Game::updateCurrentScoreTexture()
{
    if (currentScoreTexture) SDL_DestroyTexture(currentScoreTexture);
    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText.c_str(), {255, 255, 255, 255});
    if (!surface)
    {
        std::cout << "Failed to create current score surface! TTF_Error: " << TTF_GetError() << std::endl;
        currentScoreTexture = nullptr;
        return;
    }
    currentScoreTexture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!currentScoreTexture)
    {
        std::cout << "Failed to create current score texture! SDL_Error: " << SDL_GetError() << std::endl;
    }
    currentScoreRect.w = surface->w;
    currentScoreRect.h = surface->h;
    SDL_FreeSurface(surface);
}
