#include "Game.h"
#include "Ball.h"
#include "Racket.h"
#include "Block.h"
#include "BlocksGrid.h"
#include "Bonus.h"
#include "CollisionsManager.h"
#include "Label.h"
#include "ExtraBallBonus.h"
#include "IncreaseRacketBonus.h"
#include "SpeedBallBonus.h"
#include "StickyBallBonus.h"
#include "OneTimeBottomBonus.h"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <string>

#define GAME_NAME "Arkanoid"
const std::string ARIAL_FONT_PATH = "arial.ttf";

Game::Game(unsigned winWidth, unsigned winHeight)
    : _timeDeltaMs(0), _score(0),
      _window(std::make_unique<sf::RenderWindow>(sf::VideoMode(winWidth, winHeight), GAME_NAME)),
      _balls({ std::make_shared<Ball>(*this, sf::Vector2f(0.3f, 0.4f), 10.0f) }),
      _racket(std::make_shared<Racket>(*this, winHeight * 0.8f, 1.0f, sf::Vector2f(winWidth * 0.2f, winHeight * 0.02f))),
      _collisionsManager(std::make_unique<CollisionsManager>()),
      _scoreLabel(std::make_unique<Label>(*this, ARIAL_FONT_PATH)),
      _blocksGrid(std::make_unique<BlocksGrid>(*this, sf::FloatRect(winWidth * 0.15f, winHeight * 0.1f, winWidth * 0.7f, winHeight * 0.3f), sf::Vector2u(5, 7), 2)),
      _racketBonusTimer(0),
      _racketOriginalSize(_racket->getSize()),
      _stickyBallActive(false),
      _stickyBallAttached(false),
      _oneTimeBottomActive(false)
{
    // Центрируем каретку
    float racketX = (winWidth - _racket->getSize().x) / 2.f;
    _racket->setPosition(sf::Vector2f(racketX, winHeight * 0.8f));
    float ballX = racketX + _racket->getSize().x / 2.f - _balls[0]->getRadius();
    float ballY = winHeight * 0.8f - 2 * _balls[0]->getRadius() - 60;
    _balls[0]->setPosition(sf::Vector2f(ballX, ballY));
    _collisionsManager->addBall(_balls[0]);
    _collisionsManager->addRacket(_racket);
    _collisionsManager->addBlocksGrid(*_blocksGrid);
}

std::unique_ptr<sf::RenderWindow>& Game::getWindow() {
    return _window;
}

float Game::getTimeMsSinceLastFrame() const {
    return _timeDeltaMs;
}

void Game::spawnExtraBall() {
    if (_balls.size() < 2 && !_balls.empty()) {
        auto pos = _balls[0]->getPosition() + sf::Vector2f(20, 0);
        float angle = (rand() % 2 == 0) ? 0.4f : -0.4f;
        auto ball = std::make_shared<Ball>(*this, sf::Vector2f(angle, 0.4f), 10.0f);
        ball->setPosition(pos);
        _balls.push_back(ball);
        _collisionsManager->addBall(ball);
    }
}

void Game::spawnRandomBonus(const sf::Vector2f& pos) {
    sf::Vector2f size(20, 20);
    int r = rand() % 100;
    std::shared_ptr<Bonus> bonus;
    if (r < 20) {
        bonus = std::make_shared<OneTimeBottomBonus>(*this, pos, size, 0.1f);
    } else if (r < 40) {
        bonus = std::make_shared<ExtraBallBonus>(*this, pos, size, 0.1f);
    } else if (r < 60) {
        bonus = std::make_shared<StickyBallBonus>(*this, pos, size, 0.1f);
    } else if (r < 80) {
        bonus = std::make_shared<IncreaseRacketBonus>(*this, pos, size, 0.1f);
    } else {
        bonus = std::make_shared<SpeedBallBonus>(*this, pos, size, 0.1f);
    }
    _bonuses.push_back(bonus);
    _collisionsManager->addBonus(bonus);
}

void Game::increaseScore(int score) {
    _score += score;
}

int Game::run() {
    _lastTimePoint = std::chrono::high_resolution_clock::now();
    while (_window->isOpen()) {
        _refreshTimeDeltaMs();
        sf::Event event;
        while (_window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                _window->close();
            _racket->handleEvent(event);
        }
        _window->clear(sf::Color::White);
        _collisionsManager->handleCollisions();
        _updateScoreLabel();
        _blocksGrid->draw();
        _racket->draw();
        for (auto& ball : _balls)
            ball->draw();
        for (auto& bonus : _bonuses)
            bonus->draw();
        _scoreLabel->draw();
        _window->display();
        _blocksGrid->destroyDeadBlocks();
        _destroyDeadBalls();
        _destroyDeadBonuses();
        if (_racketBonusTimer > 0) {
            _racketBonusTimer -= _timeDeltaMs;
            if (_racketBonusTimer <= 0) {
                _racket->setSize(_racketOriginalSize);
            }
        }
    }
    return 0;
}

void Game::_refreshTimeDeltaMs() {
    auto now = std::chrono::high_resolution_clock::now();
    _timeDeltaMs = std::chrono::duration_cast<std::chrono::nanoseconds>(now - _lastTimePoint).count() / 1e6f;
    _lastTimePoint = now;
}

void Game::_destroyDeadBalls() {
    auto ballIt = _balls.begin();
    while (ballIt != _balls.end()) {
        if ((*ballIt)->isDead())
            ballIt = _balls.erase(ballIt);
        else
            ballIt++;
    }
}

void Game::_destroyDeadBonuses() {
    auto bonusIt = _bonuses.begin();
    while (bonusIt != _bonuses.end()) {
        if ((*bonusIt)->isDead())
            bonusIt = _bonuses.erase(bonusIt);
        else
            bonusIt++;
    }
}

void Game::_updateScoreLabel() {
    _scoreLabel->setText(L"Score: " + std::to_wstring(_score));
}

bool Game::isMainBall(const Ball* ball) const {
    if (_balls.empty()) return false;
    return _balls[0].get() == ball;
}

void Game::applyRacketSizeBonus(float multiplier, float ms) {
    if (_racketBonusTimer > 0) {
        _racket->setSize(_racketOriginalSize);
    }
    _racketOriginalSize = _racket->getSize();
    _racket->setSize(_racketOriginalSize * multiplier);
    _racketBonusTimer = ms;
}

void Game::activateStickyBall(bool active) { _stickyBallActive = active; }
bool Game::isStickyBallActive() const { return _stickyBallActive; }
void Game::setStickyBallAttached(bool attached) { _stickyBallAttached = attached; }
bool Game::isStickyBallAttached() const { return _stickyBallAttached; }

void Game::activateOneTimeBottom() { _oneTimeBottomActive = true; }
bool Game::isOneTimeBottomActive() const { return _oneTimeBottomActive; }
void Game::setOneTimeBottomActive(bool active) { _oneTimeBottomActive = active; } 