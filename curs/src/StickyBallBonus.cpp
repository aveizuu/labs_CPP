#include "StickyBallBonus.h"
#include "Game.h"

StickyBallBonus::StickyBallBonus(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size, float speed, const sf::Color& color)
    : Bonus(parent, pos, size, speed, color) {}

void StickyBallBonus::invokeBonusAction() {
    _parent.setStickyBallAttached(false);
    _isDead = true;
} 