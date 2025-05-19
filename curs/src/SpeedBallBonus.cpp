#include "SpeedBallBonus.h"
#include "Game.h"
#include "Ball.h"

SpeedBallBonus::SpeedBallBonus(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size, float speed, const sf::Color& color)
    : Bonus(parent, pos, size, speed, color) {}

void SpeedBallBonus::invokeBonusAction() {
    // Ускоряем все мячи
    for (auto& ball : _parent.getBalls()) {
        if (ball)
            ball->boostUp(1.2f, 3000.0f);
    }
    _isDead = true;
} 