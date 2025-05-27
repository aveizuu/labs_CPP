#include "Paddle.h"

Paddle::Paddle(const sf::Vector2f& pos, const sf::Vector2f& size)
    : GameObject(pos, size),
      moveSpeed(15.f),
      originalWidth(size.x),
      maxWidth(size.x * 2.0f) {
    shape.setFillColor(sf::Color::White);
}

void Paddle::update(float deltaTime) {
    if (!isActive) return;
    // Обновление позиции и других параметров каретки
}

void Paddle::draw(sf::RenderWindow& window) {
    if (isActive) {
        window.draw(shape);
    }
}

void Paddle::handleCollision(GameObject& other) {
    if (!isActive) return;
    // Обработка столкновений с другими объектами
}

void Paddle::moveLeft(float deltaTime) {
    if (!isActive) return;
    position.x -= moveSpeed * deltaTime;
    if (position.x < 0) {
        position.x = 0;
    }
    shape.setPosition(position);
}

void Paddle::moveRight(float deltaTime, float windowWidth) {
    if (!isActive) return;
    position.x += moveSpeed * deltaTime;
    if (position.x + size.x > windowWidth) {
        position.x = windowWidth - size.x;
    }
    shape.setPosition(position);
}

void Paddle::setWidth(float width) {
    if (width <= maxWidth) {
        size.x = width;
        shape.setSize(size);
        // Центрируем каретку относительно её текущей позиции
        position.x += (originalWidth - width) / 2;
        shape.setPosition(position);
    }
}

void Paddle::resetWidth() {
    setWidth(originalWidth);
} 