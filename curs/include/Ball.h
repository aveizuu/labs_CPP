#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <functional>
#include <chrono>
class Game;

class Ball {
public:
    Ball(Game& parent, const sf::Vector2f& velocity, float radius, int fallScoreIncrease = -15, const sf::Color& color = sf::Color::Red);
    void draw();
    sf::Vector2f getPosition() const;
    float getRadius() const;
    sf::Vector2f getVelocity() const;
    void setPosition(const sf::Vector2f& pos);
    void setVelocity(const sf::Vector2f& velocity);
    bool isDead() const;
    void boostUp(float multiplier, float timeMs);
    bool isBoosted() const;
    void kill();
    Game& getParent() { return _parent; }
private:
    void _move();
    void _update();
    void _handleWindowCollision();
    void _handleBoost();
    Game& _parent;
    sf::CircleShape _shape;
    sf::Vector2f _velocity;
    int _fallScoreIncrease;
    float _velocityNormDefault;
    float _lastBoostDuration;
    sf::Color _colorDefault;
    bool _boosted;
    std::chrono::time_point<std::chrono::system_clock> _lastBoostTimePoint;
    float _boostMultiplier = 1.0f;
    float _boostEndTime = 0.0f;
    sf::Color _boostColor = sf::Color(100, 200, 255);
    bool _isDead = false;
}; 