#pragma once

#include "Gem.h"
#include "Map.h"
#include <SFML/Graphics.hpp>

class Bomb : public Gem {
public:
    Bomb(Map& parent, const Color& color);
    Bomb(const Bomb& other) = default;
    Bomb(Bomb&& other) = default;
    ~Bomb() override = default;
    void draw() override;
    void onDeath() override;
    void bindMapCell(const sf::Vector2u& cellUV) override;
private:
    sf::Texture _texture;
    sf::Sprite _sprite;
}; 