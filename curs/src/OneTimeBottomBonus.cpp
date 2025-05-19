#include "OneTimeBottomBonus.h"
#include "Game.h"

OneTimeBottomBonus::OneTimeBottomBonus(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size, float speed, const sf::Color& color)
    : Bonus(parent, pos, size, speed, color) {}

void OneTimeBottomBonus::invokeBonusAction() {
    _parent.activateOneTimeBottom();
    _isDead = true;
} 