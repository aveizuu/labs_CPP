#include "Game.h"
#include <random>
#include <iostream>

Game::Game()
    : window(sf::VideoMode(800, 600), "Arkanoid"),
      score(0),
      lives(3),
      isGameOver(false),
      isPaused(false) {
    initialize();
}

void Game::initialize() {
    // Загрузка шрифта
    if (!font.loadFromFile("Unbounded-Regular.ttf")) {
        std::cerr << "Error loading font" << std::endl;
    }

    // Настройка текста
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    livesText.setFont(font);
    livesText.setCharacterSize(20);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(window.getSize().x - 100, 10);

    // Создание каретки
    float paddleX = (window.getSize().x - PADDLE_WIDTH) / 2;
    float paddleY = window.getSize().y - PADDLE_HEIGHT - 20;
    paddle = std::make_unique<Paddle>(
        sf::Vector2f(paddleX, paddleY),
        sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT)
    );

    // Создание мяча
    float ballX = paddleX + PADDLE_WIDTH / 2;
    float ballY = paddleY - BALL_RADIUS * 2;
    ball = std::make_unique<Ball>(sf::Vector2f(ballX, ballY), BALL_RADIUS);

    // Создание блоков
    createBlocks();
}

void Game::createBlocks() {
    float startX = (window.getSize().x - (BLOCKS_PER_ROW * (BLOCK_WIDTH + BLOCK_SPACING))) / 2;
    float startY = 50.f;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> bonusChance(0.0f, 1.0f);

    for (int row = 0; row < BLOCK_ROWS; ++row) {
        for (int col = 0; col < BLOCKS_PER_ROW; ++col) {
            float x = startX + col * (BLOCK_WIDTH + BLOCK_SPACING);
            float y = startY + row * (BLOCK_HEIGHT + BLOCK_SPACING);
            sf::Vector2f pos(x, y);
            sf::Vector2f size(BLOCK_WIDTH, BLOCK_HEIGHT);

            // Создаем разные типы блоков в зависимости от ряда
            if (row == 0) {
                // Первый ряд - неразрушаемые блоки
                blocks.push_back(std::make_unique<IndestructibleBlock>(pos, size));
            }
            else if (row == 1) {
                // Второй ряд - блоки со скоростью
                blocks.push_back(std::make_unique<SpeedBlock>(pos, size));
            }
            else if (row == 2) {
                // Третий ряд - блоки с бонусами
                if (bonusChance(gen) < 4.0f) { // 30% шанс бонуса
                    std::unique_ptr<Bonus> bonus;
                    float bonusType = bonusChance(gen);
                    if (bonusType < 0.2f) {
                        bonus = std::make_unique<PaddleSizeBonus>(pos);
                    }
                    else if (bonusType < 0.4f) {
                        bonus = std::make_unique<BallSpeedBonus>(pos);
                    }
                    else if (bonusType < 0.6f) {
                        bonus = std::make_unique<StickyPaddleBonus>(pos);
                    }
                    else if (bonusType < 0.8f) {
                        bonus = std::make_unique<BottomShieldBonus>(pos);
                    }
                    else {
                        bonus = std::make_unique<RandomTrajectoryBonus>(pos);
                    }
                    blocks.push_back(std::make_unique<BonusBlock>(pos, size, std::move(bonus)));
                }
                else {
                    blocks.push_back(std::make_unique<HealthBlock>(pos, size, 2));
                }
            }
            else {
                 //Остальные ряды - обычные блоки с разным здоровьем
                blocks.push_back(std::make_unique<HealthBlock>(pos, size, row - 1));
            }
        }
    }
}

void Game::run() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        
        processEvents();
        if (!isPaused && !isGameOver) {
            update(deltaTime);
        }
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                isPaused = !isPaused;
            }
            else if (event.key.code == sf::Keyboard::Space && isGameOver) {
                // Перезапуск игры
                score = 0;
                lives = 3;
                isGameOver = false;
                blocks.clear();
                activeBonuses.clear();
                initialize();
            }
            // Отпустить мяч, если он прилип к Paddle
            else if (event.key.code == sf::Keyboard::Space && ball->isStuck()) {
                ball->releaseFromPaddle();
            }
        }
    }

    // Управление кареткой
    if (!isPaused && !isGameOver) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            paddle->moveLeft(1.f/60.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            paddle->moveRight(1.f/60.f, window.getSize().x);
        }
    }
}

void Game::update(float deltaTime) {
    // Если мяч прилип к Paddle, двигаем его вместе с Paddle
    if (ball->isStuck() && paddle) {
        ball->stickToPaddle(paddle->getPosition(), paddle->getSize().x);
    }
    // Обновление мяча
    ball->update(deltaTime);

    // Обновление бонусов
    for (auto& bonus : activeBonuses) {
        bonus->update(deltaTime);
    }

    // Проверка столкновений
    checkCollisions();
    handleBonusCollisions();

    // Проверка проигрыша
    if (!ball->getIsActive()) {
        lives--;
        if (lives <= 0) {
            gameOver();
        }
        else {
            resetBall();
        }
    }

    // Обновление UI
    scoreText.setString("Score: " + std::to_string(score));
    livesText.setString("Lives: " + std::to_string(lives));
}

void Game::checkCollisions() {
    // Столкновение мяча со стенами
    ball->handleWallCollision(window.getSize());

    // Столкновение мяча с кареткой
    if (ball->checkCollision(*paddle)) {
        ball->handleCollision(*paddle);
    }

    // Столкновение мяча с блоками
    for (auto& block : blocks) {
        if (block->getIsActive() && ball->checkCollision(*block)) {
            ball->handleCollision(*block);
            block->handleCollision(*ball);
            
            // Проверяем, есть ли бонус
            if (auto bonus = block->releaseBonus()) {
                spawnBonus(std::move(bonus));
            }

            // SpeedBlock: увеличиваем скорость мяча
            SpeedBlock* speedBlock = dynamic_cast<SpeedBlock*>(block.get());
            if (speedBlock && !block->getIsActive()) {
                ball->setSpeed(ball->getSpeed() + speedBlock->getSpeedIncrease());
            }

            // Увеличиваем счет
            if (!block->isIndestructible()) {
                score++;
            }
        }
    }
}

void Game::handleBonusCollisions() {
    // Проверка столкновений бонусов с кареткой
    for (auto it = activeBonuses.begin(); it != activeBonuses.end();) {
        if ((*it)->checkCollision(*paddle)) {
            (*it)->apply(*paddle, *ball);
            it = activeBonuses.erase(it);
        }
        else if ((*it)->getPosition().y > window.getSize().y) {
            it = activeBonuses.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Game::spawnBonus(std::unique_ptr<Bonus> bonus) {
    if (bonus) {
        activeBonuses.push_back(std::move(bonus));
    }
}

void Game::resetBall() {
    float ballX = paddle->getPosition().x + paddle->getSize().x / 2;
    float ballY = paddle->getPosition().y - BALL_RADIUS * 2;
    ball = std::make_unique<Ball>(sf::Vector2f(ballX, ballY), BALL_RADIUS);
}

void Game::gameOver() {
    isGameOver = true;
}

void Game::render() {
    window.clear(sf::Color::Black);

    // Отрисовка всех игровых объектов
    if (ball) ball->draw(window);
    if (paddle) paddle->draw(window);
    
    for (const auto& block : blocks) {
        if (block->getIsActive()) {
            block->draw(window);
        }
    }

    for (const auto& bonus : activeBonuses) {
        bonus->draw(window);
    }

    // Отрисовка UI
    drawUI();

    window.display();
}

void Game::drawUI() {
    window.draw(scoreText);
    window.draw(livesText);

    if (isGameOver) {
        sf::Text gameOverText;
        gameOverText.setFont(font);
        gameOverText.setString("Game Over! Press SPACE to restart");
        gameOverText.setCharacterSize(30);
        gameOverText.setFillColor(sf::Color::White);
        gameOverText.setPosition(
            (window.getSize().x - gameOverText.getLocalBounds().width) / 2,
            (window.getSize().y - gameOverText.getLocalBounds().height) / 2
        );
        window.draw(gameOverText);
    }

    if (isPaused) {
        sf::Text pauseText;
        pauseText.setFont(font);
        pauseText.setString("PAUSED");
        pauseText.setCharacterSize(30);
        pauseText.setFillColor(sf::Color::White);
        pauseText.setPosition(
            (window.getSize().x - pauseText.getLocalBounds().width) / 2,
            (window.getSize().y - pauseText.getLocalBounds().height) / 2
        );
        window.draw(pauseText);
    }
} 