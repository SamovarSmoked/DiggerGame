#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Level
{
public:
    explicit Level(const std::string& filename);

    void draw(sf::RenderWindow& window);

    [[nodiscard]] sf::Vector2i getPlayerSpawn() const;
    [[nodiscard]] sf::Vector2i getEnemySpawn() const;

    [[nodiscard]] char getTile(int x, int y) const;
    void setTile(int x, int y, char value);

    [[nodiscard]] bool hasGold() const;

private:
    std::vector<std::string> m_map;

    sf::Texture m_texDirt;
    sf::Texture m_texRock;
    sf::Texture m_texWall;
    sf::Texture m_texGold;
    sf::Texture m_texBack;

    sf::Sprite  m_tileSprite;

    static constexpr int TILE_SIZE = 32;

    sf::Vector2i m_playerSpawn{};
    sf::Vector2i m_enemySpawn{};
};