#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "Label.h"
#include "BlocksGrid.h"
#include "CollisionsManager.h"
#include "Ball.h"
#include "Racket.h"
#include "Block.h"
#include "Bonus.h"

class Game {
public:
    Game(unsigned winWidth, unsigned winHeight);
    int run();
    std::unique_ptr<sf::RenderWindow>& getWindow();
    float getTimeMsSinceLastFrame() const;
    void spawnExtraBall();
    void spawnRandomBonus(const sf::Vector2f& pos);
    void increaseScore(int score);
    bool isMainBall(const Ball* ball) const;
    void applyRacketSizeBonus(float multiplier, float ms);
    const std::vector<std::shared_ptr<Ball>>& getBalls() const { return _balls; }
    void activateStickyBall(bool active = true);
    bool isStickyBallActive() const;
    void setStickyBallAttached(bool attached);
    bool isStickyBallAttached() const;
    std::shared_ptr<Racket> getRacket() const { return _racket; }
    void setLastBallVelocity(const sf::Vector2f& v) { _lastBallVelocity = v; }
    sf::Vector2f getLastBallVelocity() const { return _lastBallVelocity; }
    void activateOneTimeBottom();
    bool isOneTimeBottomActive() const;
    void setOneTimeBottomActive(bool active);
private:
    void _refreshTimeDeltaMs();
    void _destroyDeadBalls();
    void _destroyDeadBonuses();
    void _updateScoreLabel();
    std::unique_ptr<sf::RenderWindow> _window;
    std::vector<std::shared_ptr<Ball>> _balls;
    std::shared_ptr<Racket> _racket;
    std::unique_ptr<CollisionsManager> _collisionsManager;
    std::unique_ptr<BlocksGrid> _blocksGrid;
    std::vector<std::shared_ptr<Bonus>> _bonuses;
    std::unique_ptr<Label> _scoreLabel;
    std::chrono::time_point<std::chrono::high_resolution_clock> _lastTimePoint;
    float _timeDeltaMs;
    int _score;
    float _racketBonusTimer = 0;
    sf::Vector2f _racketOriginalSize;
    bool _stickyBallActive = false;
    bool _stickyBallAttached = false;
    sf::Vector2f _lastBallVelocity = {0.3f, -0.4f};
    bool _oneTimeBottomActive = false;
}; 