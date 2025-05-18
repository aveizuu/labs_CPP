#define _CRT_SECURE_NO_WARNINGS

#include "Gem.h"
#include "Game.h"
#include "Map.h"
#include "Resources.h"
#include "Bomb.h"
#include "Brush.h"
#include <iostream>
#include <map>
#include <memory>

constexpr float CELL_MARGIN_RATIO = .2f;
constexpr float CELL_MARGIN_RATIO_SELECTED = .05f;
constexpr unsigned BONUS_RADIUS = 2;
constexpr float BONUS_PROB = 0.3f; 

std::map<Gem::Color, sf::Color> gcolorMap = {
    { Gem::Color::Green, sf::Color::Green },
    { Gem::Color::Red, sf::Color(255, 150, 150, 255)},
    { Gem::Color::Yellow, sf::Color(255, 255, 150, 255)},
    { Gem::Color::Cyan, sf::Color::Cyan },
    { Gem::Color::Blue, sf::Color(150, 150, 255, 255)}
};

Gem::Color Gem::RandomColor() {
    return static_cast<Gem::Color>(rand() % static_cast<int>(Color::Count));
}

Gem::Gem(Map& parent, const Color& color) : _map(parent), _color(color) {
    _rect.setSize(_map.getCellSize() - sf::Vector2f(4, 4));
    _rect.setFillColor(gcolorMap[_color]);
    _rect.setOutlineThickness(2);
    _rect.setOutlineColor(sf::Color::White);
}

void Gem::setPosition(const sf::Vector2f& pos) {
    _rect.setPosition(pos);
}

void Gem::setColor(const Color& color) {
    _color = color;
    _rect.setFillColor(gcolorMap[_color]);
}

void Gem::bindMapCell(const sf::Vector2u& cellUV) {
    _uv = cellUV;
    const float x0 = _map.getPos().x;
    const float y0 = _map.getPos().y;
    const float cellWidth = _map.getCellSize().x;
    const float cellHeight = _map.getCellSize().y;
    sf::Vector2f pos = sf::Vector2f(
        x0 + _uv.x * cellWidth + 2,
        y0 + _uv.y * cellHeight + 2
    );
    setPosition(pos);
}

sf::Vector2u Gem::getUV() const { return _uv; }
Gem::Color Gem::getColor() const { return _color; }

void Gem::draw() {
    if (_selected)
        _rect.setOutlineColor(sf::Color::Yellow);
    else
        _rect.setOutlineColor(sf::Color::White);
    _map.getGame().getWindow()->draw(_rect);
}

void Gem::select() { _selected = true; }
void Gem::unselect() { _selected = false; }
bool Gem::isSelected() const { return _selected; }
bool Gem::operator==(const Gem& other) const { return _color == other._color; }

void Gem::onDeath() {
    std::cout << "Gem::onDeath called for uv: " << _uv.x << "," << _uv.y << std::endl;
    if ((rand() / static_cast<float>(RAND_MAX)) < BONUS_PROB) {
        unsigned BONUS_RADIUS = 3;
        unsigned uMin = (_uv.x > BONUS_RADIUS) ? _uv.x - BONUS_RADIUS : 0;
        unsigned vMin = (_uv.y > BONUS_RADIUS) ? _uv.y - BONUS_RADIUS : 0;
        unsigned uMax = std::min(_uv.x + BONUS_RADIUS, _map.getUVSize().x - 1);
        unsigned vMax = std::min(_uv.y + BONUS_RADIUS, _map.getUVSize().y - 1);
        for (int attempt = 0; attempt < 10; ++attempt) {
            sf::Vector2u randPos = sf::Vector2u(rand() % (uMax - uMin + 1) + uMin, rand() % (vMax - vMin + 1) + vMin);
            if (randPos == _uv) continue;
            if (!_map.getGem(randPos)) {
                if (rand() % 2) {
                    std::cout << "Bomb created at " << randPos.x << "," << randPos.y << std::endl;
                    _map.getGem(randPos) = std::make_unique<Bomb>(_map, _color);
                } else {
                    std::cout << "Brush created at " << randPos.x << "," << randPos.y << std::endl;
                    _map.getGem(randPos) = std::make_unique<Brush>(_map, _color);
                }
                _map.getGem(randPos)->bindMapCell(randPos);
                break;
            }
        }
    }
}

NoGemTextureException::NoGemTextureException(const std::string& texturePath) {
    std::string strmsg = std::string("No gem texture found! Make sure ")
        + texturePath + std::string(" exists.");
    _msglen = strmsg.size() + 1;
    _msg = new char[_msglen];
    strcpy(_msg, strmsg.c_str());
    _msg[strmsg.size()] = '\0';
}

NoGemTextureException::NoGemTextureException(const NoGemTextureException& other) {
    _msglen = other._msglen;
    _msg = new char[_msglen];
    strcpy(_msg, other._msg);
}

const char* NoGemTextureException::what() const noexcept {
    return _msg;
}

NoGemTextureException::~NoGemTextureException() {
    delete[] _msg;
}