#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
class Game;

class Racket {
public:
    Racket(Game& parent, float y, float speed, const sf::Vector2f& size, const sf::Color& color = sf::Color::Black);
    void draw();
    void handleEvent(const sf::Event& event);
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
    void setPosition(const sf::Vector2f& pos);
    void setSize(const sf::Vector2f& size);
private:
    void _move();
    void _update();
    void _handleWindowCollision();
    Game& _parent;
    sf::RectangleShape _shape;
    float _speedDefault;
    float _speed;
}; 