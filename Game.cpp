#include <iostream>
#include <SDL_image.h>
#include "Game.h"

// Khởi tạo trò chơi, tải các tài nguyên cần thiết
Game::Game(SDL_Renderer* rend) : renderer(rend)
{
    // Khởi tạo các biến trạng thái
    running = true;
    gameStarted = false;
    isExploding = false;
    isPaused = false;
    isCountingDown = false;
    levelSelection = false;
    countdownTimer = 0;
    countdownNumber = 3;
    score = 0;
    frameCount = 0;
    rocketFrameCount = 0;
    explodeTimer = 0;
    pipeSpeed = PIPE_SPEED;
    bombSpeed = BOMB_SPEED;
    bombSpawnRate = 150;
    rocketFallSpeed = ROCKET_SPEED;
    gravity = 0.2f;
    jumpForce = -6.0f;
    shakeTimer = 0;
    shakeIntensity = 3;
    shakeOffsetX = 0;
    shakeOffsetY = 0;
    explosionFrame = 0;
    explosionFrameCount = 0;

    // Tạo đối tượng Bird và AudioManager
    bird = new Bird(renderer);
    audioManager = new AudioManager();

    // Tải texture nền
    SDL_Surface* bgSurface = IMG_Load("assets/background.png");
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    // Tải texture ống
    SDL_Surface* upperPipeSurface = IMG_Load("assets/upper_pipe.png");
    if (!upperPipeSurface) std::cout << "Failed to load upper pipe image! SDL_image Error: " << IMG_GetError() << std::endl;
    upperPipeTexture = SDL_CreateTextureFromSurface(renderer, upperPipeSurface);
    SDL_FreeSurface(upperPipeSurface);

    SDL_Surface* lowerPipeSurface = IMG_Load("assets/lower_pipe.png");
    if (!lowerPipeSurface) std::cout << "Failed to load lower pipe image! SDL_image Error: " << IMG_GetError() << std::endl;
    lowerPipeTexture = SDL_CreateTextureFromSurface(renderer, lowerPipeSurface);
    SDL_FreeSurface(lowerPipeSurface);

    // Tải texture bom
    SDL_Surface* bombSurface = IMG_Load("assets/bomb.png");
    bombTexture = SDL_CreateTextureFromSurface(renderer, bombSurface);
    SDL_FreeSurface(bombSurface);

    // Tải texture tên lửa
    SDL_Surface* rocketSurface = IMG_Load("assets/rocket.png");
    if (!rocketSurface) std::cout << "Failed to load rocket image! SDL_image Error: " << IMG_GetError() << std::endl;
    rocketTexture = SDL_CreateTextureFromSurface(renderer, rocketSurface);
    SDL_FreeSurface(rocketSurface);

    // Tải texture hiệu ứng nổ
    SDL_Surface* explosionSurface = IMG_Load("assets/explosion_sheet.png");
    if (!explosionSurface) std::cout << "Failed to load explosion image! SDL_image Error: " << IMG_GetError() << std::endl;
    explosionTexture = SDL_CreateTextureFromSurface(renderer, explosionSurface);
    SDL_FreeSurface(explosionSurface);
    explosionRect = {0, 0, 150, 150};
    explosionSrcRect = {0, 0, 100, 100};

    // Khởi tạo font chữ
    if (TTF_Init() == -1) std::cout << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
    font = TTF_OpenFont("assets/Arial.ttf", 24);
    if (!font) std::cout << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
    scoreTexture = nullptr;
    scoreRect = {10, 10, 0, 0};
    updateScoreTexture();

    // Tải texture nút bắt đầu
    SDL_Surface* startButtonSurface = IMG_Load("assets/start_button.png");
    startButtonTexture = SDL_CreateTextureFromSurface(renderer, startButtonSurface);
    SDL_FreeSurface(startButtonSurface);
    startButtonRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT * 2 / 3, 100, 50};

    // Tải texture tiêu đề
    SDL_Surface* flappyBirdTitleSurface = IMG_Load("assets/flappybird_title.png");
    if (!flappyBirdTitleSurface) std::cout << "Failed to load title image! SDL_image Error: " << IMG_GetError() << std::endl;
    flappyBirdTitleTexture = SDL_CreateTextureFromSurface(renderer, flappyBirdTitleSurface);
    flappyBirdTitleRect = {SCREEN_WIDTH / 2 - 350, SCREEN_HEIGHT / 3 - 50, 700, 200};
    SDL_FreeSurface(flappyBirdTitleSurface);

    // Tải texture bảng chọn cấp độ
    SDL_Surface* panelSurface = IMG_Load("assets/panel.png");
    panelTexture = SDL_CreateTextureFromSurface(renderer, panelSurface);
    SDL_FreeSurface(panelSurface);
    levelPanelRect = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 100, 300, 270};

    // Tải texture nút cấp độ
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

    // Khởi tạo điểm cao nhất
    currentLevel = 0;
    for (int i = 0; i < 3; i++) highScores[i] = 0;
    highScoreTexture = nullptr;
    highScoreRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 75, 0, 0};
    currentScoreTexture = nullptr;
    currentScoreRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 25, 0, 0};
    updateHighScoreTexture();

    // Tải texture nút chơi lại và thoát
    SDL_Surface* replaySurface = IMG_Load("assets/replay_button.png");
    replayButtonTexture = SDL_CreateTextureFromSurface(renderer, replaySurface);
    SDL_FreeSurface(replaySurface);
    replayButtonRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 25, 100, 50};

    SDL_Surface* exitSurface = IMG_Load("assets/exit_button.png");
    if (!exitSurface) std::cout << "Failed to load exit button image! SDL_image Error: " << IMG_GetError() << std::endl;
    exitButtonTexture = SDL_CreateTextureFromSurface(renderer, exitSurface);
    SDL_FreeSurface(exitSurface);
    exitButtonRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 95, 100, 50};
    gameOverPanelRect = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 100, 300, 280};
}

// Giải phóng tài nguyên khi hủy đối tượng
Game::~Game()
{
    delete bird;
    for (auto pipe : pipes) delete pipe;
    for (auto bomb : bombs) delete bomb;
    for (auto rocket : rockets) delete rocket;
    delete audioManager;
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(upperPipeTexture);
    SDL_DestroyTexture(lowerPipeTexture);
    SDL_DestroyTexture(bombTexture);
    SDL_DestroyTexture(rocketTexture);
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

// Chạy vòng lặp chính của trò chơi
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
        if (isExploding) explodeTimer++;
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
    }
    audioManager->stopBackgroundMusic();
}

// Xử lý sự kiện đầu vào từ người dùng
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
                    startLevel(1, 0.2f, -6.0f, PIPE_SPEED, BOMB_SPEED, 150, ROCKET_SPEED);
                }
                else if (x >= level2ButtonRect.x && x <= level2ButtonRect.x + level2ButtonRect.w && y >= level2ButtonRect.y && y <= level2ButtonRect.y + level2ButtonRect.h)
                {
                    startLevel(2, 0.2f * 1.5f, -6.0f * 1.5f, PIPE_SPEED * 1.5f, BOMB_SPEED * 1.5f, 90, ROCKET_SPEED * 1.5f);
                }
                else if (x >= levelAsianButtonRect.x && x <= levelAsianButtonRect.x + levelAsianButtonRect.w && y >= levelAsianButtonRect.y && y <= levelAsianButtonRect.y + levelAsianButtonRect.h)
                {
                    startLevel(3, 0.2f * 3, -6.0f * 3, PIPE_SPEED * 3, BOMB_SPEED * 3, 60, ROCKET_SPEED * 3);
                }
            }
            else if (isExploding && explodeTimer >= EXPLOSION_FRAMES * EXPLOSION_FRAME_DURATION)
            {
                if (x >= replayButtonRect.x && x <= replayButtonRect.x + replayButtonRect.w && y >= replayButtonRect.y && y <= replayButtonRect.y + replayButtonRect.h)
                {
                    resetGame();
                }
                else if (x >= exitButtonRect.x && x <= exitButtonRect.x + exitButtonRect.w && y >= exitButtonRect.y && y <= exitButtonRect.y + exitButtonRect.h)
                {
                    gameStarted = false;
                    levelSelection = true;
                    isExploding = false;
                    explodeTimer = 0;
                    score = 0;
                    frameCount = 0;
                    rocketFrameCount = 0;
                    shakeTimer = 0;
                    bird->rect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, 50, 50};
                    bird->velocity = 0;
                    bird->angle = 0;
                    for (auto pipe : pipes) delete pipe;
                    pipes.clear();
                    for (auto bomb : bombs) delete bomb;
                    bombs.clear();
                    for (auto rocket : rockets) delete rocket;
                    rockets.clear();
                    updateScoreTexture();
                    updateHighScoreTexture();
                    updateCurrentScoreTexture();
                }
            }
        }
        else if (event.type == SDL_KEYDOWN && gameStarted && !levelSelection)
        {
            if (event.key.keysym.sym == SDLK_SPACE)
            {
                if (!isCountingDown && !isPaused && !isExploding)
                {
                    bird->jump();
                    audioManager->playJumpSound();
                }
            }
            else if (event.key.keysym.sym == SDLK_p)
            {
                if (!isCountingDown && !isExploding && explodeTimer < EXPLOSION_FRAMES * EXPLOSION_FRAME_DURATION + 1000)
                {
                    isPaused = !isPaused;
                    if (isPaused) audioManager->stopBackgroundMusic();
                    else audioManager->playBackgroundMusic();
                }
            }
        }
    }
}

// Cập nhật trạng thái trò chơi
void Game::update()
{
    if (shakeTimer > 0)
    {
        shakeTimer--;
        shakeOffsetX = (rand() % (2 * shakeIntensity + 1)) - shakeIntensity;
        shakeOffsetY = (rand() % (2 * shakeIntensity + 1)) - shakeIntensity;
    }
    else
    {
        shakeOffsetX = 0;
        shakeOffsetY = 0;
    }
    if (isCountingDown)
    {
        countdownTimer++;
        if (countdownTimer >= 60)
        {
            countdownNumber--;
            countdownTimer = 0;
            if (countdownNumber <= 0) isCountingDown = false;
        }
        return;
    }
    if (!gameStarted || isExploding || isPaused) return;
    bird->update();
    frameCount++;
    rocketFrameCount++;
    if (frameCount % 90 == 0) spawnPipe();
    if (frameCount % bombSpawnRate == 0) spawnBomb();
    if (rocketFrameCount >= ROCKET_SPAWN_INTERVAL)
    {
        spawnRocket();
        rocketFrameCount = 0;
    }
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
    for (auto it = rockets.begin(); it != rockets.end();)
    {
        (*it)->update(rocketFallSpeed, pipeSpeed);
        if ((*it)->rect.y > SCREEN_HEIGHT || (*it)->rect.x < -ROCKET_SIZE)
        {
            delete *it;
            it = rockets.erase(it);
        }
        else ++it;
    }
    checkCollisions();
    updateScore();
}

// Vẽ các thành phần lên màn hình
void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_Rect bgRect = {shakeOffsetX, shakeOffsetY, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, &bgRect);

    if (!gameStarted && !levelSelection)
    {
        SDL_Rect titleRect = {flappyBirdTitleRect.x + shakeOffsetX, flappyBirdTitleRect.y + shakeOffsetY, flappyBirdTitleRect.w, flappyBirdTitleRect.h};
        SDL_RenderCopy(renderer, flappyBirdTitleTexture, nullptr, &titleRect);
        SDL_Rect buttonRect = {startButtonRect.x + shakeOffsetX, startButtonRect.y + shakeOffsetY, startButtonRect.w, startButtonRect.h};
        SDL_RenderCopy(renderer, startButtonTexture, nullptr, &buttonRect);
    }
    else if (levelSelection)
    {
        SDL_Rect panelRect = {levelPanelRect.x + shakeOffsetX, levelPanelRect.y + shakeOffsetY, levelPanelRect.w, levelPanelRect.h};
        SDL_RenderCopy(renderer, panelTexture, nullptr, &panelRect);
        SDL_Rect level1Rect = {level1ButtonRect.x + shakeOffsetX, level1ButtonRect.y + shakeOffsetY, level1ButtonRect.w, level1ButtonRect.h};
        SDL_RenderCopy(renderer, level1ButtonTexture, nullptr, &level1Rect);
        SDL_Rect level2Rect = {level2ButtonRect.x + shakeOffsetX, level2ButtonRect.y + shakeOffsetY, level2ButtonRect.w, level2ButtonRect.h};
        SDL_RenderCopy(renderer, level2ButtonTexture, nullptr, &level2Rect);
        SDL_Rect levelAsianRect = {levelAsianButtonRect.x + shakeOffsetX, levelAsianButtonRect.y + shakeOffsetY, levelAsianButtonRect.w, levelAsianButtonRect.h};
        SDL_RenderCopy(renderer, levelAsianButtonTexture, nullptr, &levelAsianRect);
    }
    else
    {
        if (!isExploding) bird->render(renderer);
        for (auto pipe : pipes) pipe->render(renderer, shakeOffsetX, shakeOffsetY);
        for (auto bomb : bombs) bomb->render(renderer, shakeOffsetX, shakeOffsetY);
        for (auto rocket : rockets) rocket->render(renderer, shakeOffsetX, shakeOffsetY);
        SDL_Rect scoreRectAdjusted = {scoreRect.x + shakeOffsetX, scoreRect.y + shakeOffsetY, scoreRect.w, scoreRect.h};
        SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRectAdjusted);

        if (isPaused)
        {
            const char* pauseText = "Paused";
            SDL_Surface* pauseSurface = TTF_RenderText_Solid(font, pauseText, {255, 255, 255, 255});
            if (!pauseSurface)
            {
                std::cout << "Failed to render pause text! TTF_Error: " << TTF_GetError() << std::endl;
                return;
            }
            SDL_Texture* pauseTexture = SDL_CreateTextureFromSurface(renderer, pauseSurface);
            SDL_Rect pauseRect = {SCREEN_WIDTH / 2 - pauseSurface->w / 2 + shakeOffsetX, SCREEN_HEIGHT / 2 - pauseSurface->h / 2 + shakeOffsetY, pauseSurface->w, pauseSurface->h};
            SDL_RenderCopy(renderer, pauseTexture, nullptr, &pauseRect);
            SDL_FreeSurface(pauseSurface);
            SDL_DestroyTexture(pauseTexture);
        }
        if (isCountingDown && countdownNumber > 0)
        {
            std::string countdownText = std::to_string(countdownNumber);
            SDL_Surface* countdownSurface = TTF_RenderText_Solid(font, countdownText.c_str(), {255, 255, 255, 255});
            if (!countdownSurface)
            {
                std::cout << "Failed to render countdown text! TTF_Error: " << TTF_GetError() << std::endl;
                return;
            }
            SDL_Texture* countdownTexture = SDL_CreateTextureFromSurface(renderer, countdownSurface);
            SDL_Rect countdownRect = {SCREEN_WIDTH / 2 - countdownSurface->w / 2 + shakeOffsetX, SCREEN_HEIGHT / 2 - countdownSurface->h / 2 + shakeOffsetY, countdownSurface->w, countdownSurface->h};
            SDL_RenderCopy(renderer, countdownTexture, nullptr, &countdownRect);
            SDL_FreeSurface(countdownSurface);
            SDL_DestroyTexture(countdownTexture);
        }
        if (isExploding && explodeTimer < EXPLOSION_FRAMES * EXPLOSION_FRAME_DURATION)
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
                SDL_Rect explosionRectAdjusted = {explosionRect.x + shakeOffsetX, explosionRect.y + shakeOffsetY, explosionRect.w, explosionRect.h};
                SDL_RenderCopy(renderer, explosionTexture, &explosionSrcRect, &explosionRectAdjusted);
            }
        }
        else if (isExploding)
        {
            SDL_Rect panelRect = {gameOverPanelRect.x, gameOverPanelRect.y, gameOverPanelRect.w, gameOverPanelRect.h};
            SDL_RenderCopy(renderer, panelTexture, nullptr, &panelRect);
            SDL_Rect highScoreRectAdjusted = {highScoreRect.x, highScoreRect.y, highScoreRect.w, highScoreRect.h};
            SDL_Rect currentScoreRectAdjusted = {currentScoreRect.x, currentScoreRect.y, currentScoreRect.w, currentScoreRect.h};
            SDL_Rect replayRect = {replayButtonRect.x, replayButtonRect.y, replayButtonRect.w, replayButtonRect.h};
            SDL_Rect exitRect = {exitButtonRect.x, exitButtonRect.y, exitButtonRect.w, exitButtonRect.h};
            SDL_RenderCopy(renderer, highScoreTexture, nullptr, &highScoreRectAdjusted);
            SDL_RenderCopy(renderer, currentScoreTexture, nullptr, &currentScoreRectAdjusted);
            SDL_RenderCopy(renderer, replayButtonTexture, nullptr, &replayRect);
            SDL_RenderCopy(renderer, exitButtonTexture, nullptr, &exitRect);
        }
    }
    SDL_RenderPresent(renderer);
}

// Sinh ra cặp ống mới
void Game::spawnPipe()
{
    pipes.push_back(new Pipe(SCREEN_WIDTH, upperPipeTexture, lowerPipeTexture, renderer));
}

// Sinh ra bom mới
void Game::spawnBomb()
{
    int y = rand() % (SCREEN_HEIGHT - BOMB_SIZE);
    bombs.push_back(new Bomb(SCREEN_WIDTH, y, bombTexture, renderer));
}

// Sinh ra tên lửa mới
void Game::spawnRocket()
{
    int x = rand() % (SCREEN_WIDTH - ROCKET_SIZE);
    rockets.push_back(new Rocket(x, rocketTexture, renderer));
}

// Kiểm tra va chạm giữa Bird và các vật thể
void Game::checkCollisions()
{
    SDL_Rect birdRect = bird->collisionRect;

    for (auto pipe : pipes)
    {
        if (pipe->upperRect.x > SCREEN_WIDTH || pipe->upperRect.x + pipe->upperRect.w < 0) continue;
        if (SDL_HasIntersection(&birdRect, &pipe->upperRect) || SDL_HasIntersection(&birdRect, &pipe->lowerRect))
        {
            handleCollision();
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
        SDL_Rect bombRect = (*it)->collisionRect;
        if (SDL_HasIntersection(&birdRect, &bombRect))
        {
            handleCollision();
            delete *it;
            bombs.erase(it);
            return;
        }
        else ++it;
    }
    for (auto it = rockets.begin(); it != rockets.end();)
    {
        if ((*it)->rect.y > SCREEN_HEIGHT || (*it)->rect.x < -100)
        {
            delete *it;
            it = rockets.erase(it);
            continue;
        }
        SDL_Rect rocketRect = (*it)->collisionRect;
        if (SDL_HasIntersection(&birdRect, &rocketRect))
        {
            handleCollision();
            delete *it;
            rockets.erase(it);
            return;
        }
        else ++it;
    }
    if (birdRect.y < 0 || birdRect.y + birdRect.h > SCREEN_HEIGHT)
    {
        handleCollision();
    }
}

// Xử lý khi có va chạm xảy ra
void Game::handleCollision()
{
    explosionRect.x = bird->rect.x - (explosionRect.w / 2);
    explosionRect.y = bird->rect.y - (explosionRect.h / 2);
    audioManager->playExplosionSound();
    isExploding = true;
    explosionFrame = 0;
    explosionFrameCount = 0;
    shakeTimer = 15;
    if (score > highScores[currentLevel - 1]) highScores[currentLevel - 1] = score;
    updateHighScoreTexture();
    updateCurrentScoreTexture();
}

// Cập nhật điểm số
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

// Cập nhật texture điểm số
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

// Cập nhật texture điểm cao nhất
void Game::updateHighScoreTexture()
{
    if (highScoreTexture) SDL_DestroyTexture(highScoreTexture);
    std::string highScoreText = "High Score: " + std::to_string(highScores[currentLevel - 1]);
    SDL_Surface* highScoreSurface = TTF_RenderText_Solid(font, highScoreText.c_str(), {255, 255, 255, 255});
    if (!highScoreSurface)
    {
        std::cout << "Failed to create high score surface! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }
    highScoreTexture = SDL_CreateTextureFromSurface(renderer, highScoreSurface);
    highScoreRect.w = highScoreSurface->w;
    highScoreRect.h = highScoreSurface->h;
    SDL_FreeSurface(highScoreSurface);
}

// Cập nhật texture điểm hiện tại
void Game::updateCurrentScoreTexture()
{
    if (currentScoreTexture) SDL_DestroyTexture(currentScoreTexture);
    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText.c_str(), {255, 255, 255, 255});
    if (!surface)
    {
        std::cout << "Failed to create current score surface! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }
    currentScoreTexture = SDL_CreateTextureFromSurface(renderer, surface);
    currentScoreRect.w = surface->w;
    currentScoreRect.h = surface->h;
    SDL_FreeSurface(surface);
}

// Bắt đầu một cấp độ mới
void Game::startLevel(int level, float grav, float jump, int pSpeed, int bSpeed, int bSpawnRate, int rSpeed)
{
    currentLevel = level;
    gameStarted = true;
    levelSelection = false;
    isCountingDown = true;
    countdownTimer = 0;
    countdownNumber = 3;
    gravity = grav;
    jumpForce = jump;
    pipeSpeed = pSpeed;
    bombSpeed = bSpeed;
    bombSpawnRate = bSpawnRate;
    rocketFallSpeed = rSpeed;
    bird->gravity = gravity;
    bird->jumpForce = jumpForce;
    bird->rect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, 50, 50};
    bird->velocity = 0;
    bird->angle = 0;
    for (auto pipe : pipes) delete pipe;
    pipes.clear();
    for (auto bomb : bombs) delete bomb;
    bombs.clear();
    for (auto rocket : rockets) delete rocket;
    rockets.clear();
}

// Đặt lại game về trạng thái ban đầu
void Game::resetGame()
{
    score = 0;
    frameCount = 0;
    rocketFrameCount = 0;
    explodeTimer = 0;
    isExploding = false;
    shakeTimer = 0;
    isCountingDown = true;
    countdownTimer = 0;
    countdownNumber = 3;
    if (currentLevel == 1)
    {
        gravity = 0.2f;
        jumpForce = -6.0f;
        pipeSpeed = PIPE_SPEED;
        bombSpeed = BOMB_SPEED;
        bombSpawnRate = 150;
        rocketFallSpeed = ROCKET_SPEED;
    }
    else if (currentLevel == 2)
    {
        gravity = 0.2f * 1.5f;
        jumpForce = -6.0f * 1.5f;
        pipeSpeed = PIPE_SPEED * 1.5f;
        bombSpeed = BOMB_SPEED * 1.5f;
        bombSpawnRate = 90;
        rocketFallSpeed = ROCKET_SPEED * 1.5f;
    }
    else if (currentLevel == 3)
    {
        gravity = 0.2f * 3;
        jumpForce = -6.0f * 3;
        pipeSpeed = PIPE_SPEED * 3;
        bombSpeed = BOMB_SPEED * 3;
        bombSpawnRate = 60;
        rocketFallSpeed = ROCKET_SPEED * 3;
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
    for (auto rocket : rockets) delete rocket;
    rockets.clear();
    updateScoreTexture();
    updateHighScoreTexture();
    updateCurrentScoreTexture();
}
