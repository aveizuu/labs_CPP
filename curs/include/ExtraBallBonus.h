#pragma once
#include "Bonus.h"
 
class ExtraBallBonus : public Bonus {
public:
    ExtraBallBonus(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size, float speed, const sf::Color& color = sf::Color::Magenta);
    void invokeBonusAction() override;
}; 