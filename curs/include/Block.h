#pragma once
#include "GameObject.h"
#include <memory>

class Bonus; // Предварительное объявление

class Block : public GameObject {
protected:
    int health;
    bool indestructible;
    std::unique_ptr<Bonus> bonus;
    sf::Color color;

public:
    Block(const sf::Vector2f& pos, const sf::Vector2f& size, int health = 1);
    virtual ~Block() = default;

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    void handleCollision(GameObject& other) override;

    // Виртуальные методы для специфического поведения блоков
    virtual void onHit();
    virtual void onDestroy();
    virtual bool hasBonus() const { return bonus != nullptr; }
    virtual std::unique_ptr<Bonus> releaseBonus();

    // Геттеры
    int getHealth() const { return health; }
    bool isIndestructible() const { return indestructible; }
    const sf::Color& getColor() const { return color; }

protected:
    void setColor(const sf::Color& newColor);
    void setIndestructible(bool ind) { indestructible = ind; }
};

// Неразрушаемый блок
class IndestructibleBlock : public Block {
public:
    IndestructibleBlock(const sf::Vector2f& pos, const sf::Vector2f& size)
        : Block(pos, size, 1) {
        setIndestructible(true);
        setColor(sf::Color(100, 100, 100)); // Серый цвет
    }

    void onHit() override {
        // Просто отскакивает, не разрушается
    }
};

// Блок со скоростью
class SpeedBlock : public Block {
    float speedIncrease;
public:
    SpeedBlock(const sf::Vector2f& pos, const sf::Vector2f& size, float speedInc = 60.f)
        : Block(pos, size, 1), speedIncrease(speedInc) {
        setColor(sf::Color::Red);
    }

    void onHit() override {
        Block::onHit();
    }

    float getSpeedIncrease() const { return speedIncrease; }
};

// Блок с бонусом
class BonusBlock : public Block {
public:
    BonusBlock(const sf::Vector2f& pos, const sf::Vector2f& size, std::unique_ptr<Bonus> b)
        : Block(pos, size, 1) {
        bonus = std::move(b);
        setColor(sf::Color::Yellow);
    }

    void onDestroy() override {
        // При разрушении бонус начинает падать
    }
};

// Блок с здоровьем
class HealthBlock : public Block {
public:
    HealthBlock(const sf::Vector2f& pos, const sf::Vector2f& size, int health = 3)
        : Block(pos, size, health) {
        setColor(sf::Color(0, 200, 255));
    }
}; 