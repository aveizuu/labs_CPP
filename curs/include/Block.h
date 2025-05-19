#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
class Game;

enum class BlockKind {
    Indestructible,
    Bonus,
    Speed,
    Green,
    Orange
};

class Block {
public:
    Block(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color = sf::Color(100, 100, 100, 255), BlockKind kind = BlockKind::Indestructible);
    virtual ~Block() = default;
    virtual void draw();
    virtual void setSpeed(float speed);
    virtual float getSpeed() const;
    virtual void setPosition(const sf::Vector2f& pos);
    virtual bool isDead() const;
    virtual void damage();
    virtual sf::Vector2f getPosition() const;
    virtual sf::Vector2f getSize() const;
    virtual int getHealth() const;
    virtual BlockKind getKind() const;
protected:
    Game& _parent;
    sf::RectangleShape _shape;
    BlockKind _kind;
};

std::shared_ptr<Block> makeBlock(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size, int health, int hitScore, const sf::Color& color, BlockKind kind); 