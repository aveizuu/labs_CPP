#pragma once
#include <SFML/Graphics.hpp>

class GameObject {
protected:
    sf::Vector2f position;
    sf::Vector2f size;
    sf::RectangleShape shape;
    bool isActive;

public:
    GameObject(const sf::Vector2f& pos, const sf::Vector2f& size);
    virtual ~GameObject() = default; // Для удаления объектов

    // Виртуальные методы, которые должны быть реализованы в дочерних классах
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void handleCollision(GameObject& other) = 0;

    // Геттеры и сеттеры
    const sf::Vector2f& getPosition() const { return position; }
    void setPosition(const sf::Vector2f& pos);
    const sf::Vector2f& getSize() const { return size; }
    bool getIsActive() const { return isActive; }
    void setIsActive(bool active) { isActive = active; }

    // Метод для проверки столкновений
    bool checkCollision(const GameObject& other) const;
}; 