#include "BlocksGrid.h"
#include "Game.h"
#include "Block.h"
#include <random>

extern std::shared_ptr<Block> makeBlock(Game& parent, const sf::Vector2f& pos, const sf::Vector2f& size, int health, int hitScore, const sf::Color& color, BlockKind kind);

BlocksGrid::BlocksGrid(Game& parent, const sf::FloatRect& rect, const sf::Vector2u& uvSize, unsigned softBlockHealth)
    : _parent(parent), _uvSize(uvSize), _rect(rect), _blockSize(rect.width / uvSize.x, rect.height / uvSize.y), _blocks(uvSize.x * uvSize.y) {
    struct BlockType {
        sf::Color color;
        int health;
        int hitScore;
        BlockKind kind;
    };
    std::vector<BlockType> types = {
        {sf::Color(0, 60, 0), -1, 10, BlockKind::Indestructible},           // тёмно-зелёный — неразрушаемый
        {sf::Color(80, 120, 255), 1, 10, BlockKind::Bonus},                 // синий — бонус
        {sf::Color(220, 60, 60), 1, 10, BlockKind::Speed},                  // красный — ускорение шарика
        {sf::Color(60, 200, 60), 2, 10, BlockKind::Green},                  // зелёный — 2 жизни
        {sf::Color(255, 140, 0), 3, 10, BlockKind::Orange}                  // оранжевый — 3 жизни
    };
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, types.size() - 1);
    int speedBlockCount = 0;
    for (unsigned i = 0; i < uvSize.x; i++)
        for (unsigned j = 0; j < uvSize.y; j++) {
            BlockType t;
            do {
                t = types[dist(gen)];
            } while (t.kind == BlockKind::Speed && speedBlockCount >= 7);
            if (t.kind == BlockKind::Speed) speedBlockCount++;
            _blocks[i + j * uvSize.x] = makeBlock(parent, _blockPos(i, j), _blockSize, t.health, t.hitScore, t.color, t.kind);
        }
}

std::shared_ptr<Block>& BlocksGrid::getBlock(unsigned u, unsigned v) {
    return _blocks[u + v * _uvSize.x];
}

sf::Vector2u BlocksGrid::getUVSize() const {
    return _uvSize;
}

void BlocksGrid::draw() {
    for (auto& block : _blocks)
        if (block)
            block->draw();
}

void BlocksGrid::destroyDeadBlocks() {
    for (auto& block : _blocks)
        if (block && block->isDead())
            block = nullptr;
}

sf::Vector2f BlocksGrid::_blockPos(unsigned u, unsigned v) const {
    return sf::Vector2f(_rect.left + u * _blockSize.x, _rect.top + v * _blockSize.y);
} 