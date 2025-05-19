#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
class Game;

class Bonus {
public:
    Bonus(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size, float speed, const sf::Color& color);
    virtual ~Bonus() = default;
    virtual void draw();
    virtual bool isDead() const;
    virtual void invokeBonusAction() = 0;
    virtual sf::Vector2f getPosition() const;
    virtual sf::Vector2f getSize() const;
protected:
    void _move();
    Game& _parent;
    sf::RectangleShape _shape;
    float _speed;
    bool _isDead;
}; 