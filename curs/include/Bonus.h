#pragma once
#include "GameObject.h"
#include "Paddle.h"
#include "Ball.h"

class Bonus : public GameObject {
protected:
    float fallSpeed;
    sf::Color color;

public:
    Bonus(const sf::Vector2f& pos, const sf::Vector2f& size)
        : GameObject(pos, size), fallSpeed(200.f), color(sf::Color::Green) {
        shape.setFillColor(color);
    }

    void update(float deltaTime) override {
        if (!isActive) return;
        position.y += fallSpeed * deltaTime;
        shape.setPosition(position);
    }

    void draw(sf::RenderWindow& window) override {
        if (isActive) {
            window.draw(shape);
        }
    }

    void handleCollision(GameObject& other) override {
        // Реализация в дочерних классах
    }

    virtual void apply(Paddle& paddle, Ball& ball) = 0;
};

// Бонус изменения размера каретки
class PaddleSizeBonus : public Bonus {
    float sizeMultiplier;
public:
    PaddleSizeBonus(const sf::Vector2f& pos, float mult = 1.5f)
        : Bonus(pos, sf::Vector2f(30, 15)), sizeMultiplier(mult) {
        color = sf::Color::Magenta;
        shape.setFillColor(color);
    }

    void apply(Paddle& paddle, Ball&) override {
        paddle.setWidth(paddle.getOriginalWidth() * sizeMultiplier);
    }
};

// Бонус изменения скорости мяча
class BallSpeedBonus : public Bonus {
    float speedMultiplier;
public:
    BallSpeedBonus(const sf::Vector2f& pos, float mult = 1.3f)
        : Bonus(pos, sf::Vector2f(30, 15)), speedMultiplier(mult) {
        color = sf::Color::Red;
        shape.setFillColor(color);
    }

    void apply(Paddle&, Ball& ball) override {
        ball.setSpeed(ball.getSpeed() * speedMultiplier);
    }
};

// Бонус прилипания мяча к каретке
class StickyPaddleBonus : public Bonus {
public:
    StickyPaddleBonus(const sf::Vector2f& pos)
        : Bonus(pos, sf::Vector2f(30, 15)) {
        color = sf::Color::Blue;
        shape.setFillColor(color);
    }

    void apply(Paddle&, Ball& ball) override {
        ball.setSticky(true);
    }
};

// Бонус временного дна
class BottomShieldBonus : public Bonus {
public:
    BottomShieldBonus(const sf::Vector2f& pos)
        : Bonus(pos, sf::Vector2f(30, 15)) {
        color = sf::Color::Cyan;
        shape.setFillColor(color);
    }

    void apply(Paddle&, Ball& ball) override {
        ball.setBottomShield(true);
    }
};

// Бонус случайной траектории
class RandomTrajectoryBonus : public Bonus {
public:
    RandomTrajectoryBonus(const sf::Vector2f& pos)
        : Bonus(pos, sf::Vector2f(30, 15)) {
        color = sf::Color::Yellow;
        shape.setFillColor(color);
    }

    void apply(Paddle&, Ball& ball) override {
        ball.randomizeTrajectory();
    }
}; 