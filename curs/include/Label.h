#pragma once
#include <SFML/Graphics.hpp>
#include <string>
class Game;

class Label {
public:
    Label(Game& parent, const std::string& fontPath, const sf::Vector2f& pos = sf::Vector2f(10,10), unsigned charSize = 24, const sf::Color& color = sf::Color::Black);
    void draw();
    void setText(const std::wstring& text);
private:
    Game& _parent;
    sf::Font _font;
    sf::Text _text;
}; 