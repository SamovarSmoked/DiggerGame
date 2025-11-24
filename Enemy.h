#pragma once
#include <SFML/Graphics.hpp>
class Level;

class Enemy
{
public:
    Enemy(const std::string& texturePath, sf::Vector2i gridPos);

    void update(const Level& level, const sf::Vector2i& playerPos);
    void draw(sf::RenderWindow& window) const;

    [[nodiscard]] sf::Vector2i getPosition() const;

private:
    static sf::Texture s_texture;

    sf::Sprite    m_sprite{};
    sf::Vector2i  m_gridPos{};
    sf::Clock     m_moveClock{};

    static constexpr int   TILE_SIZE = 32;
    static constexpr float MOVE_DELAY = 0.450f; // seconds
};
