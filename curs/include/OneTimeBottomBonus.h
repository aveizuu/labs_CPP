#pragma once
#include "Bonus.h"

class OneTimeBottomBonus : public Bonus {
public:
    OneTimeBottomBonus(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size, float speed, const sf::Color& color = sf::Color::Yellow);
    void invokeBonusAction() override;
}; 