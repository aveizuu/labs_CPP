#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Ball.h"
#include "Paddle.h"
#include "Block.h"
#include "Bonus.h"

class Game {
private:
    sf::RenderWindow window;
    sf::Clock clock;
    sf::Font font;
    sf::Text scoreText;
    sf::Text livesText;

    std::unique_ptr<Ball> ball;
    std::unique_ptr<Paddle> paddle;
    std::vector<std::unique_ptr<Block>> blocks;
    std::vector<std::unique_ptr<Bonus>> activeBonuses;

    int score;
    int lives;
    bool isGameOver;
    bool isPaused;

    // Константы игры
    static constexpr float BALL_RADIUS = 10.f;
    static constexpr float PADDLE_WIDTH = 100.f;
    static constexpr float PADDLE_HEIGHT = 20.f;
    static constexpr float BLOCK_WIDTH = 60.f;
    static constexpr float BLOCK_HEIGHT = 30.f;
    static constexpr int BLOCKS_PER_ROW = 10;
    static constexpr int BLOCK_ROWS = 5;
    static constexpr float BLOCK_SPACING = 10.f;

public:
    Game();
    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();
    void initialize();
    void createBlocks();
    void checkCollisions();
    void handleBonusCollisions();
    void spawnBonus(std::unique_ptr<Bonus> bonus);
    void resetBall();
    void gameOver();
    void drawUI();
}; 