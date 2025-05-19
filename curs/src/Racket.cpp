#include "Racket.h"
#include "Game.h"

Racket::Racket(Game& parent, float y, float speed, const sf::Vector2f& size, const sf::Color& color)
    : _parent(parent), _speedDefault(speed), _speed(0) {
    _shape.setFillColor(color);
    _shape.setSize(size);
    _shape.setPosition(sf::Vector2f(0, y));
}

void Racket::handleEvent(const sf::Event& event) {
    switch (event.type) {
    case sf::Event::KeyPressed:
        switch (event.key.code) {
        case sf::Keyboard::Left:
            if (_speed == 0)
                _speed = -_speedDefault;
            break;
        case sf::Keyboard::Right:
            if (_speed == 0)
                _speed = _speedDefault;
            break;
        case sf::Keyboard::Space:
            if (_parent.isStickyBallAttached()) {
                auto balls = _parent.getBalls();
                if (!balls.empty()) {
                    sf::Vector2f v = _parent.getLastBallVelocity();
                    balls[0]->setVelocity({-v.x, -v.y});
                }
                _parent.setStickyBallAttached(false);
            }
            break;
        }
        break;
    case sf::Event::KeyReleased:
        switch (event.key.code) {
        case sf::Keyboard::Left:
            if (_speed < 0)
                _speed = 0;
            break;
        case sf::Keyboard::Right:
            if (_speed > 0)
                _speed = 0;
            break;
        }
        break;
    }
}

sf::Vector2f Racket::getPosition() const {
    return _shape.getPosition();
}

sf::Vector2f Racket::getSize() const {
    return _shape.getSize();
}

void Racket::_move() {
    float dt = _parent.getTimeMsSinceLastFrame();
    sf::Vector2f velocity = sf::Vector2f(_speed, 0);
    _shape.move(dt * velocity);
}

void Racket::_update() {
    _move();
    _handleWindowCollision();
}

void Racket::_handleWindowCollision() {
    sf::Vector2f pos = _shape.getPosition();
    float width = _shape.getSize().x;
    unsigned winWidth = _parent.getWindow()->getSize().x;
    if (pos.x < 0)
        _shape.setPosition(sf::Vector2f(0, pos.y));
    if (pos.x + width > winWidth)
        _shape.setPosition(sf::Vector2f(winWidth - width, pos.y));
}

void Racket::draw() {
    _update();
    _parent.getWindow()->draw(_shape);
}

void Racket::setPosition(const sf::Vector2f& pos) {
    _shape.setPosition(pos);
}

void Racket::setSize(const sf::Vector2f& size) {
    _shape.setSize(size);
} 