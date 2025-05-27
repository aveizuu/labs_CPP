#include "GameObject.h"

GameObject::GameObject(const sf::Vector2f& pos, const sf::Vector2f& size)
    : position(pos), size(size), isActive(true) {
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(sf::Color::White);
}

void GameObject::setPosition(const sf::Vector2f& pos) {
    position = pos;
    shape.setPosition(position);
}

bool GameObject::checkCollision(const GameObject& other) const {
    // Проверка пересечения прямоугольников
    return (position.x < other.position.x + other.size.x &&
            position.x + size.x > other.position.x &&
            position.y < other.position.y + other.size.y &&
            position.y + size.y > other.position.y);
} 