#include "Brush.h"
#include "Map.h"
#include "Game.h"
#include <iostream>
#include <stdexcept>

constexpr unsigned GEMS_BRUSHED_COUNT = 2;

Brush::Brush(Map& parent, const Color& color) : Gem(parent, color) {
    if (!_texture.loadFromFile("res/img/brush.png")) {
        throw std::runtime_error("Failed to load brush.png");
    }
    _sprite.setTexture(_texture);
    _sprite.setColor(gcolorMap[_color]);
    _sprite.setScale(
        _map.getCellSize().x / _texture.getSize().x,
        _map.getCellSize().y / _texture.getSize().y
    );
    _sprite.setPosition(_rect.getPosition());
}

void Brush::draw() {
    sf::RectangleShape outline(_map.getCellSize() - sf::Vector2f(4, 4));
    outline.setPosition(_rect.getPosition());
    outline.setFillColor(sf::Color(0, 0, 0, 0));
    outline.setOutlineThickness(4);
    outline.setOutlineColor(gcolorMap[_color]);
    _map.getGame().getWindow()->draw(outline);

    _map.getGame().getWindow()->draw(_sprite);
}

void Brush::onDeath() {
    std::vector<sf::Vector2u> gemsToBrush;
    while (gemsToBrush.size() < GEMS_BRUSHED_COUNT) {
        sf::Vector2u randPos = sf::Vector2u(rand() % _map.getUVSize().x, rand() % _map.getUVSize().y);
        bool isSelf = randPos == _uv;
        bool isUnique = std::find(gemsToBrush.begin(), gemsToBrush.end(), randPos) == gemsToBrush.end();
        bool isNeighbour = abs(static_cast<int>(randPos.x) - static_cast<int>(_uv.x)) +
            abs(static_cast<int>(randPos.y) - static_cast<int>(_uv.y)) < 2;
        if (!isSelf && isUnique && !isNeighbour)
            gemsToBrush.push_back(randPos);
    }
    for (auto& pos : gemsToBrush)
        if (_map.getGem(pos))
            _map.getGem(pos)->setColor(_color);
}

void Brush::bindMapCell(const sf::Vector2u& cellUV) {
    Gem::bindMapCell(cellUV);
    _sprite.setPosition(_rect.getPosition());
} 