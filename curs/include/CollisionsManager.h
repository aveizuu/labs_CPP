#pragma once
#include <vector>
#include <memory>
class Ball;
class Block;
class Bonus;
class Racket;
class BlocksGrid;

class CollisionsManager {
public:
    CollisionsManager();
    void addBall(const std::shared_ptr<Ball>& ball);
    void addBlock(const std::shared_ptr<Block>& block);
    void addBonus(const std::shared_ptr<Bonus>& bonus);
    void addRacket(const std::shared_ptr<Racket>& racket);
    void addBlocksGrid(BlocksGrid& grid);
    void handleCollisions();
private:
    std::vector<std::weak_ptr<Ball>> _balls;
    std::vector<std::weak_ptr<Block>> _blocks;
    std::vector<std::weak_ptr<Bonus>> _bonuses;
    std::weak_ptr<Racket> _racket;
}; 