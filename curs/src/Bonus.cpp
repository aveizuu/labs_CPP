#include "Bonus.h"
#include "ExtraBallBonus.h"
#include "Game.h"

Bonus::Bonus(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size, float speed, const sf::Color& color)
    : _parent(parent), _speed(speed), _isDead(false) {
    _shape.setPosition(pos);
    _shape.setSize(size);
    _shape.setFillColor(color);
}

void Bonus::draw() {
    _parent.getWindow()->draw(_shape);
    _move();
}

bool Bonus::isDead() const {
    return _isDead;
}

sf::Vector2f Bonus::getPosition() const {
    return _shape.getPosition();
}

sf::Vector2f Bonus::getSize() const {
    return _shape.getSize();
}

void Bonus::_move() {
    _shape.move(sf::Vector2f(0, _parent.getTimeMsSinceLastFrame() * _speed));
    if (getPosition().y + getSize().y > _parent.getWindow()->getSize().y)
        _isDead = true;
}

ExtraBallBonus::ExtraBallBonus(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size, float speed, const sf::Color& color)
    : Bonus(parent, pos, size, speed, color) {}

void ExtraBallBonus::invokeBonusAction() {
    this->_parent.spawnExtraBall();
    this->_isDead = true;
} 