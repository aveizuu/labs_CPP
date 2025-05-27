#pragma once
#include "GameObject.h"

class Paddle : public GameObject {
private:
    float moveSpeed;
    float originalWidth;
    float maxWidth;

public:
    Paddle(const sf::Vector2f& pos, const sf::Vector2f& size);
    
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    void handleCollision(GameObject& other) override;

    // Методы для управления кареткой
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime, float windowWidth);
    void setWidth(float width);
    void resetWidth();
    
    // Геттеры
    float getMoveSpeed() const { return moveSpeed; }
    float getOriginalWidth() const { return originalWidth; }
}; 