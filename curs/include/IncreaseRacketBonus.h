#pragma once
#include "Bonus.h"
 
class IncreaseRacketBonus : public Bonus {
public:
    IncreaseRacketBonus(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size, float speed, const sf::Color& color = sf::Color::Cyan);
    void invokeBonusAction() override;
}; 