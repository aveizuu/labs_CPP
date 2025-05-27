#include "Ball.h"
#include "Paddle.h"
#include <cmath>
#include <random>

Ball::Ball(const sf::Vector2f& pos, float radius)
    : GameObject(pos, sf::Vector2f(radius * 2, radius * 2)),
      shape(radius),
      velocity(100.f, -300.f), // Начальная скорость вверх
      speed(300.f),
      isSticky(false),
      hasBottomShield(false),
      bottomShieldUsed(false),
      isStuckToPaddle(false),
      stuckOffset(0.f, 0.f) {
    shape.setFillColor(sf::Color::White);
    shape.setOrigin(radius, radius);
    shape.setPosition(pos);
}

void Ball::update(float deltaTime) {
    if (!isActive) return;
    if (isStuckToPaddle) {
        // позиция мяча будет обновляться извне (Game::update)
        shape.setPosition(position);
        return;
    }
    // Обновление позиции
    position += velocity * deltaTime;
    shape.setPosition(position);
}

void Ball::draw(sf::RenderWindow& window) {
    if (isActive) {
        window.draw(shape);
    }
}

void Ball::handleCollision(GameObject& other) {
    if (!isActive) return;
    if (checkCollision(other)) {
        // Прилипание к Paddle
        Paddle* paddle = dynamic_cast<Paddle*>(&other);
        if (isSticky && paddle) {
            stickToPaddle(paddle->getPosition(), paddle->getSize().x);
            return;
        }
        // Обработка столкновения с блоком или кареткой
        sf::FloatRect ballBounds = shape.getGlobalBounds();
        sf::FloatRect otherBounds = other.getPosition().x >= 0 ?
            sf::FloatRect(other.getPosition(), other.getSize()) : sf::FloatRect();

        // Определение стороны столкновения
        float overlapLeft = ballBounds.left + ballBounds.width - otherBounds.left;
        float overlapRight = otherBounds.left + otherBounds.width - ballBounds.left;
        float overlapTop = ballBounds.top + ballBounds.height - otherBounds.top;
        float overlapBottom = otherBounds.top + otherBounds.height - ballBounds.top;

        // Находим минимальное перекрытие
        float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

        if (minOverlap == overlapLeft || minOverlap == overlapRight) {
            velocity.x = -velocity.x;
        }
        if (minOverlap == overlapTop || minOverlap == overlapBottom) {
            velocity.y = -velocity.y;
        }

        // Нормализация скорости
        float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        velocity = (velocity / length) * speed;
    }
}

void Ball::setVelocity(const sf::Vector2f& vel) {
    velocity = vel;
    // Нормализация скорости
    float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    velocity = (velocity / length) * speed;
}

void Ball::setSpeed(float newSpeed) {
    speed = newSpeed;
    // Сохраняем направление, но меняем скорость
    float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (length > 0) {
        velocity = (velocity / length) * speed;
    }
}

void Ball::setSticky(bool sticky) {
    isSticky = sticky;
}

void Ball::setBottomShield(bool shield) {
    hasBottomShield = shield;
    if (shield) {
        bottomShieldUsed = false;
    }
}

void Ball::randomizeTrajectory() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> angleDist(0.f, 1.f);
    float angle;
    if (angleDist(gen) < 0.5f) {
        angle = std::uniform_real_distribution<float>(-60.f, -30.f)(gen);
    } else {
        angle = std::uniform_real_distribution<float>(30.f, 60.f)(gen);
    }
    float radians = angle * 3.14159f / 180.f;
    velocity.x = std::sin(radians) * speed;
    velocity.y = -std::cos(radians) * speed; // всегда вверх
}

void Ball::handleWallCollision(const sf::Vector2u& windowSize) {
    float minY = 40.f;
    if (position.x - shape.getRadius() < 0) {
        position.x = shape.getRadius();
        velocity.x = std::abs(velocity.x);
    }
    else if (position.x + shape.getRadius() > windowSize.x) {
        position.x = windowSize.x - shape.getRadius();
        velocity.x = -std::abs(velocity.x);
    }

    if (position.y - shape.getRadius() < minY) {
        position.y = shape.getRadius() + minY;
        velocity.y = std::abs(velocity.y);
    }
    else if (position.y + shape.getRadius() > windowSize.y) {
        if (hasBottomShield && !bottomShieldUsed) {
            position.y = windowSize.y - shape.getRadius();
            velocity.y = -std::abs(velocity.y);
            bottomShieldUsed = true;
            hasBottomShield = false;
        }
        else {
            isActive = false;
        }
    }
}

void Ball::stickToPaddle(const sf::Vector2f& paddlePos, float paddleWidth) {
    isStuckToPaddle = true;
    // Центрируем мяч относительно Paddle
    stuckOffset.x = paddleWidth / 2.f;
    stuckOffset.y = -shape.getRadius() * 2.f;
    position.x = paddlePos.x + stuckOffset.x;
    position.y = paddlePos.y + stuckOffset.y;
    shape.setPosition(position);
    velocity = {0.f, 0.f};
}

void Ball::releaseFromPaddle() {
    isStuckToPaddle = false;
    isSticky = false; // сбрасываем эффект прилипания
    randomizeTrajectory();
} 