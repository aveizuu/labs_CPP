#include "CollisionsManager.h"
#include "Ball.h"
#include "Block.h"
#include "Bonus.h"
#include "Racket.h"
#include "BlocksGrid.h"
#include "Game.h"
#include <memory>
#include <cmath>

CollisionsManager::CollisionsManager() {}

void CollisionsManager::addBall(const std::shared_ptr<Ball>& ball) {
    _balls.push_back(ball);
}
void CollisionsManager::addBlock(const std::shared_ptr<Block>& block) {
    _blocks.push_back(block);
}
void CollisionsManager::addBonus(const std::shared_ptr<Bonus>& bonus) {
    _bonuses.push_back(bonus);
}
void CollisionsManager::addRacket(const std::shared_ptr<Racket>& racket) {
    _racket = racket;
}
void CollisionsManager::addBlocksGrid(BlocksGrid& grid) {
    for (unsigned u = 0; u < grid.getUVSize().x; ++u)
        for (unsigned v = 0; v < grid.getUVSize().y; ++v)
            addBlock(grid.getBlock(u, v));
}

void CollisionsManager::handleCollisions() {
    // Мяч и блоки
    for (auto& wball : _balls) {
        auto ball = wball.lock();
        if (!ball) continue;
        for (auto& wblock : _blocks) {
            auto block = wblock.lock();
            if (!block) continue;
            sf::FloatRect ballRect(ball->getPosition(), sf::Vector2f(ball->getRadius()*2, ball->getRadius()*2));
            sf::FloatRect blockRect(block->getPosition(), block->getSize());
            if (ballRect.intersects(blockRect)) {
                if (block->getKind() == BlockKind::Speed) {
                    if (!ball->isBoosted())
                        ball->boostUp(1.2f, 2000.0f);
                    block->damage();
                } else if (block->getHealth() == -1) {
                    sf::Vector2f ballCenter = ball->getPosition() + sf::Vector2f(ball->getRadius(), ball->getRadius());
                    sf::Vector2f blockCenter = block->getPosition() + block->getSize() / 2.f;
                    sf::Vector2f overlap;
                    overlap.x = (block->getSize().x / 2 + ball->getRadius()) - std::abs(ballCenter.x - blockCenter.x);
                    overlap.y = (block->getSize().y / 2 + ball->getRadius()) - std::abs(ballCenter.y - blockCenter.y);
                    auto vel = ball->getVelocity();
                    if (overlap.x < overlap.y) {
                        if (ballCenter.x < blockCenter.x)
                            ball->setPosition(sf::Vector2f(block->getPosition().x - ball->getRadius()*2, ball->getPosition().y));
                        else
                            ball->setPosition(sf::Vector2f(block->getPosition().x + block->getSize().x, ball->getPosition().y));
                        ball->setVelocity(sf::Vector2f(-vel.x, vel.y));
                    } else {
                        if (ballCenter.y < blockCenter.y)
                            ball->setPosition(sf::Vector2f(ball->getPosition().x, block->getPosition().y - ball->getRadius()*2));
                        else
                            ball->setPosition(sf::Vector2f(ball->getPosition().x, block->getPosition().y + block->getSize().y));
                        ball->setVelocity(sf::Vector2f(vel.x, -vel.y));
                    }
                } else {
                    auto vel = ball->getVelocity();
                    ball->setVelocity(sf::Vector2f(vel.x, -vel.y));
                    block->damage();
                }
            }
        }
    }
    auto racket = _racket.lock();
    if (racket) {
        for (auto& wball : _balls) {
            auto ball = wball.lock();
            if (!ball) continue;
            sf::FloatRect ballRect(ball->getPosition(), sf::Vector2f(ball->getRadius()*2, ball->getRadius()*2));
            sf::FloatRect racketRect(racket->getPosition(), racket->getSize());
            if (ballRect.intersects(racketRect)) {
                auto vel = ball->getVelocity();
                if (ball->getVelocity().y > 0 && ball->getParent().isStickyBallActive() && !ball->getParent().isStickyBallAttached()) {
                    auto& balls = ball->getParent().getBalls();
                    for (auto& b : balls) {
                        if (b.get() != ball.get()) b->kill();
                    }
                    ball->getParent().setStickyBallAttached(true);
                    ball->getParent().setLastBallVelocity(ball->getVelocity());
                    ball->getParent().activateStickyBall(false);
                    float racketW = racket->getSize().x;
                    float ballR = ball->getRadius();
                    ball->setVelocity({0, 0});
                    ball->setPosition({racketX + racketW / 2 - ballR, racket->getPosition().y - 2 * ballR - 0.1f});
                    continue;
                }
                if (vel.y > 0) {
                    ball->setVelocity(sf::Vector2f(vel.x, -std::abs(vel.y)));
                    ball->setPosition(sf::Vector2f(
                        ball->getPosition().x,
                        racket->getPosition().y - ball->getRadius() * 2 - 0.1f
                    ));
                } else {
                    ball->setVelocity(sf::Vector2f(vel.x, std::abs(vel.y)));
                    ball->setPosition(sf::Vector2f(
                        ball->getPosition().x,
                        racket->getPosition().y + racket->getSize().y + 0.1f
                    ));
                }
            }
        }
    }
    if (racket) {
        for (auto& wbonus : _bonuses) {
            auto bonus = wbonus.lock();
            if (!bonus || bonus->isDead()) continue;
            sf::FloatRect racketRect(racket->getPosition(), racket->getSize());
            sf::FloatRect bonusRect(bonus->getPosition(), bonus->getSize());
            if (racketRect.intersects(bonusRect)) {
                bonus->invokeBonusAction();
            }
        }
    }
    for (size_t i = 0; i < _balls.size(); ++i) {
        auto ballA = _balls[i].lock();
        if (!ballA) continue;
        for (size_t j = i + 1; j < _balls.size(); ++j) {
            auto ballB = _balls[j].lock();
            if (!ballB) continue;
            sf::Vector2f posA = ballA->getPosition();
            sf::Vector2f posB = ballB->getPosition();
            float rA = ballA->getRadius();
            float rB = ballB->getRadius();
            sf::Vector2f delta = posA - posB;
            float dist2 = delta.x * delta.x + delta.y * delta.y;
            float minDist = rA + rB;
            if (dist2 < minDist * minDist) {
                auto vA = ballA->getVelocity();
                auto vB = ballB->getVelocity();
                ballA->setVelocity(-vA);
                ballB->setVelocity(-vB);
            }
        }
    }
} 