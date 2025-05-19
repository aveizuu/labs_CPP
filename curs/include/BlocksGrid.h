#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Block.h"
class Game;

class BlocksGrid {
public:
    BlocksGrid(Game& parent, const sf::FloatRect& rect, const sf::Vector2u& uvSize, unsigned softBlockHealth);
    std::shared_ptr<Block>& getBlock(unsigned u, unsigned v);
    sf::Vector2u getUVSize() const;
    void draw();
    void destroyDeadBlocks();
private:
    sf::Vector2f _blockPos(unsigned u, unsigned v) const;
    Game& _parent;
    sf::Vector2u _uvSize;
    std::vector<std::shared_ptr<Block>> _blocks;
    sf::FloatRect _rect;
    sf::Vector2f _blockSize;
}; 