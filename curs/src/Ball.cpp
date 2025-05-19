#include "Ball.h"
#include "Game.h"
#include <cmath>

Ball::Ball(Game& parent, const sf::Vector2f& velocity, float radius, int fallScoreIncrease, const sf::Color& color)
    : _parent(parent), _velocity(velocity), _fallScoreIncrease(fallScoreIncrease), _velocityNormDefault(std::sqrt(velocity.x*velocity.x + velocity.y*velocity.y)), _lastBoostDuration(0), _colorDefault(color), _boosted(false), _isDead(false) {
    _shape.setRadius(radius);
    _shape.setFillColor(color);
    _shape.setPosition(400, 300);
}

void Ball::_move() {
    if (_parent.isStickyBallAttached()) {
        auto racket = _parent.getRacket();
        float racketX = racket->getPosition().x;
        float racketW = racket->getSize().x;
        float ballR = getRadius();
        setPosition({racketX + racketW / 2 - ballR, racket->getPosition().y - 2 * ballR - 0.1f});
        return;
    }
    float dt = _parent.getTimeMsSinceLastFrame();
    _shape.move(dt * _velocity);
}

void Ball::_update() {
    _move();
    _handleWindowCollision();
    _handleBoost();
}

void Ball::_handleWindowCollision() {
    auto& window = _parent.getWindow();
    auto winSize = window->getSize();
    auto pos = _shape.getPosition();
    float radius = _shape.getRadius();
    if (pos.x < 0 || pos.x + 2 * radius > winSize.x) {
        _velocity.x = -_velocity.x;
    }
    if (pos.y < 0) {
        _velocity.y = -_velocity.y;
    }
    if (pos.y + 2 * radius > winSize.y) {
        if (_parent.isOneTimeBottomActive()) {
            _velocity.y = -std::abs(_velocity.y);
            _parent.setOneTimeBottomActive(false);
            return;
        }
        if (_parent.isMainBall(this)) {
            _velocity.y = -std::abs(_velocity.y);
            _parent.increaseScore(-15);
        } else {
            if (!_isDead) {
                _parent.increaseScore(-15);
                kill();
            }
        }
    }
}

void Ball::_handleBoost() {
    if (_boosted) {
        _boostEndTime -= _parent.getTimeMsSinceLastFrame();
        if (_boostEndTime <= 0) {
            _velocity /= _boostMultiplier;
            _shape.setFillColor(_colorDefault);
            _boosted = false;
            _boostMultiplier = 1.0f;
        }
    }
}

void Ball::kill() {
    _isDead = true;
}

bool Ball::isDead() const {
    return _isDead;
}

void Ball::draw() {
    _update();
    _parent.getWindow()->draw(_shape);
}

sf::Vector2f Ball::getPosition() const {
    return _shape.getPosition();
}

float Ball::getRadius() const {
    return _shape.getRadius();
}

sf::Vector2f Ball::getVelocity() const {
    return _velocity;
}

void Ball::setPosition(const sf::Vector2f& pos) {
    _shape.setPosition(pos);
}

void Ball::setVelocity(const sf::Vector2f& velocity) {
    _velocity = velocity;
}

void Ball::boostUp(float multiplier, float timeMs) {
    _boostMultiplier = multiplier;
    _boostEndTime = _parent.getTimeMsSinceLastFrame() + timeMs;
    _boosted = true;
    _shape.setFillColor(_boostColor);
    _velocity *= multiplier;
}

bool Ball::isBoosted() const {
    return _boosted;
} 