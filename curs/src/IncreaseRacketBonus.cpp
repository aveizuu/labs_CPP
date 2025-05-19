#include "IncreaseRacketBonus.h"
#include "Game.h"

IncreaseRacketBonus::IncreaseRacketBonus(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size, float speed, const sf::Color& color)
    : Bonus(parent, pos, size, speed, color) {}

void IncreaseRacketBonus::invokeBonusAction() {
    _parent.applyRacketSizeBonus(1.5f, 3000.0f);
    _isDead = true;
} 