#pragma once

#include "Gem.h"
#include "Map.h"
#include <SFML/Graphics.hpp>

class Brush : public Gem {
public:
    Brush(Map& parent, const Color& color);
    Brush(const Brush& other) = default;
    Brush(Brush&& other) = default;
    ~Brush() override = default;
    void draw() override;
    void onDeath() override;
    void bindMapCell(const sf::Vector2u& cellUV) override;
private:
    sf::Texture _texture;
    sf::Sprite _sprite;
}; 