#pragma once
#include "GameObject.h"

class Ball : public GameObject {
private:
    sf::CircleShape shape; // Свой shape для круга
    sf::Vector2f velocity;
    float speed;
    bool isSticky;
    bool hasBottomShield;
    bool bottomShieldUsed;
    bool isStuckToPaddle = false;
    sf::Vector2f stuckOffset; // смещение относительно Paddle

public:
    Ball(const sf::Vector2f& pos, float radius);
    
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    void handleCollision(GameObject& other) override;

    // Методы для управления мячом
    void setVelocity(const sf::Vector2f& vel);
    void setSpeed(float newSpeed);
    void setSticky(bool sticky);
    void setBottomShield(bool shield);
    void randomizeTrajectory();
    
    // Геттеры
    const sf::Vector2f& getVelocity() const { return velocity; }
    float getSpeed() const { return speed; }
    bool getIsSticky() const { return isSticky; }
    bool getHasBottomShield() const { return hasBottomShield; }
    bool getBottomShieldUsed() const { return bottomShieldUsed; }
    float getRadius() const { return shape.getRadius(); }
    bool isStuck() const { return isStuckToPaddle; }

    void handleWallCollision(const sf::Vector2u& windowSize);

    void stickToPaddle(const sf::Vector2f& paddlePos, float paddleWidth); //
    void releaseFromPaddle();

private:
    void handlePaddleCollision(const GameObject& paddle);
    void handleBlockCollision(GameObject& block);
}; 