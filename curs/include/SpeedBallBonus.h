#pragma once
#include "Bonus.h"

class SpeedBallBonus : public Bonus {
public:
    SpeedBallBonus(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size, float speed, const sf::Color& color = sf::Color::Blue);
    void invokeBonusAction() override;
}; 