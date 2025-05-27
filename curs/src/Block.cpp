#include "Block.h"
#include "Bonus.h"

Block::Block(const sf::Vector2f& pos, const sf::Vector2f& size, int health)
    : GameObject(pos, size),
      health(health),
      indestructible(false),
      color(sf::Color::White) {
    shape.setFillColor(color);
}

void Block::update(float deltaTime) {
    if (!isActive) return;
    // Обновление состояния блока
}

void Block::draw(sf::RenderWindow& window) {
    if (isActive) {
        window.draw(shape);
    }
}

void Block::handleCollision(GameObject& other) {
    if (!isActive) return;
    
    if (checkCollision(other)) {
        onHit();
    }
}

void Block::onHit() {
    if (indestructible) return;

    health--;
    if (health <= 0) {
        onDestroy();
        isActive = false;
    }
    else {
        // Изменяем цвет блока в зависимости от оставшегося здоровья
        float intensity = static_cast<float>(health) / 3.0f; // Предполагаем максимум 3 здоровья
        color = sf::Color(255, static_cast<sf::Uint8>(255 * intensity), static_cast<sf::Uint8>(255 * intensity));
        shape.setFillColor(color);
    }
}

void Block::onDestroy() {
    // Базовая реализация - ничего не делает
    // Дочерние классы могут переопределить это поведение
}

std::unique_ptr<Bonus> Block::releaseBonus() {
    if (bonus) {
        return std::move(bonus);
    }
    return nullptr;
}

void Block::setColor(const sf::Color& newColor) {
    color = newColor;
    shape.setFillColor(color);
} 