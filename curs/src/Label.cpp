#include "Label.h"
#include "Game.h"
#include <stdexcept>

Label::Label(Game& parent, const std::string& fontPath, const sf::Vector2f& pos, unsigned charSize, const sf::Color& color)
    : _parent(parent) {
    if (!_font.loadFromFile(fontPath))
        throw std::runtime_error("Font " + fontPath + " not found!");
    _text.setFont(_font);
    _text.setFillColor(color);
    _text.setCharacterSize(charSize);
    _text.setPosition(pos);
}

void Label::draw() {
    _parent.getWindow()->draw(_text);
}

void Label::setText(const std::wstring& text) {
    _text.setString(text);
} 