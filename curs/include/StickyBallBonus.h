#pragma once
#include "Bonus.h"

class StickyBallBonus : public Bonus {
public:
    StickyBallBonus(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size, float speed, const sf::Color& color = sf::Color::Green);
    void invokeBonusAction() override;
}; 