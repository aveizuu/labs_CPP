#include "Bomb.h"
#include "Map.h"
#include "Game.h"
#include <iostream>
#include <stdexcept>

constexpr unsigned GEMS_BLOWN_COUNT = 4;

Bomb::Bomb(Map& parent, const Color& color) : Gem(parent, color) {
    if (!_texture.loadFromFile("res/img/bomb.png")) {
        throw std::runtime_error("Failed to load bomb.png");
    }
    _sprite.setTexture(_texture);
    _sprite.setColor(gcolorMap[_color]);
    _sprite.setScale(
        _map.getCellSize().x / _texture.getSize().x,
        _map.getCellSize().y / _texture.getSize().y
    );
    _sprite.setPosition(_rect.getPosition());
}

void Bomb::draw() {
    sf::RectangleShape outline(_map.getCellSize() - sf::Vector2f(4, 4));
    outline.setPosition(_rect.getPosition());
    outline.setFillColor(sf::Color(0, 0, 0, 0));
    outline.setOutlineThickness(4);
    outline.setOutlineColor(gcolorMap[_color]);
    _map.getGame().getWindow()->draw(outline);

    _map.getGame().getWindow()->draw(_sprite);
}

void Bomb::onDeath() {
    std::vector<sf::Vector2u> gemsToBlow;
    while (gemsToBlow.size() < GEMS_BLOWN_COUNT) {
        sf::Vector2u randPos = sf::Vector2u(rand() % _map.getUVSize().x, rand() % _map.getUVSize().y);
        bool isSelf = randPos == _uv;
        bool isUnique = std::find(gemsToBlow.begin(), gemsToBlow.end(), randPos) == gemsToBlow.end();
        if (!isSelf && isUnique)
            gemsToBlow.push_back(randPos);
    }
    for (auto& pos : gemsToBlow) {
        if (_map.getGem(pos))
            _map.getGem(pos)->onDeath();
        _map.getGem(pos) = nullptr;
    }
}

void Bomb::bindMapCell(const sf::Vector2u& cellUV) {
    Gem::bindMapCell(cellUV);
    _sprite.setPosition(_rect.getPosition());
} 