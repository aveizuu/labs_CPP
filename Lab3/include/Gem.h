#pragma once

#include <SFML/Graphics.hpp>
#include <map>

class Map;

class Gem {
public:
    enum class Color {
        Green,
        Red,
        Yellow,
        Cyan,
        Blue,
        Count
    };
    static Color RandomColor();

    explicit Gem() = delete;
    explicit Gem(Map& parent, const Color& color);
    explicit Gem(const Gem& other) = default;
    explicit Gem(Gem&& other) = default;
    virtual ~Gem() = default;

    void setPosition(const sf::Vector2f& pos);
    void setColor(const Color& color);
    virtual void bindMapCell(const sf::Vector2u& cellUV);
    sf::Vector2u getUV() const;
    Color getColor() const;
    virtual void draw();
    void select();
    void unselect();
    bool isSelected() const;
    bool operator==(const Gem& other) const;
    virtual void onDeath();

protected:
    Map& _map;
    Color _color;
    sf::RectangleShape _rect;
    sf::Vector2u _uv;
    bool _selected = false;
};

class NoGemTextureException : public std::exception {
public:
    NoGemTextureException(const std::string& texturePath);
    NoGemTextureException(const NoGemTextureException& other);
    virtual const char* what() const noexcept override;
    ~NoGemTextureException();
private:
    char* _msg;
    int _msglen;
};

extern std::map<Gem::Color, sf::Color> gcolorMap; 