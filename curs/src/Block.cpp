#include "Block.h"
#include "Game.h"
#include <cmath>
#include <random>
#include <memory>

// Добавим поле _health и _hitScore
class BlockImpl : public Block {
public:
    BlockImpl(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size, int health, int hitScore, const sf::Color& color, BlockKind kind)
        : Block(parent, pos, size, color, kind), _health(health), _hitScore(hitScore) {}
    void damage() override {
        if (_health == -1) return; // неразрушаемый блок
        if (_health > 0) {
            _health--;
            _parent.increaseScore(_hitScore);
            if (_health == 0) {
                _shape.setFillColor(sf::Color(0,0,0,0));
                // 30% шанс выпадения бонуса только если это бонусный или зелёный блок
                if ((_kind == BlockKind::Bonus || _kind == BlockKind::Green) && (rand() % 100) < 30) {
                    _parent.spawnRandomBonus(_shape.getPosition() + _shape.getSize() / 2.f);
                }
            }
        }
    }
    bool isDead() const override {
        if (_health == -1) return false; // неразрушаемый блок всегда жив
        return _health == 0;
    }
    int getHealth() const override { return _health; }
    BlockKind getKind() const override { return _kind; }
private:
    int _health;
    int _hitScore;
};

Block::Block(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color, BlockKind kind)
    : _parent(parent), _kind(kind) {
    _shape.setPosition(pos);
    _shape.setSize(size);
    _shape.setFillColor(color);
    _shape.setOutlineColor(sf::Color::Black);
    _shape.setOutlineThickness(-2.f);
}

void Block::draw() {
    if (!isDead())
        _parent.getWindow()->draw(_shape);
}

void Block::setSpeed(float speed) {}
float Block::getSpeed() const { return 0.0f; }
void Block::setPosition(const sf::Vector2f& pos) { _shape.setPosition(pos); }
bool Block::isDead() const { return false; }
void Block::damage() {}
sf::Vector2f Block::getPosition() const { return _shape.getPosition(); }
sf::Vector2f Block::getSize() const { return _shape.getSize(); }
int Block::getHealth() const { return 1; } 
BlockKind Block::getKind() const { return _kind; }

std::shared_ptr<Block> makeBlock(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size, int health, int hitScore, const sf::Color& color, BlockKind kind) {
    return std::make_shared<BlockImpl>(parent, pos, size, health, hitScore, color, kind);
} 