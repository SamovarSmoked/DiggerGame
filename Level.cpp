#include "Level.h"
#include <fstream>
#include <iostream>

Level::Level(const std::string& filename){
    std::ifstream inFile(filename);
    if (!inFile){
        std::cerr << "Cannot open level file: " << filename << '\n';
        return;
    }

    std::string line;
    int         y = 0;
    while (std::getline(inFile, line))
    {
        m_map.push_back(line);
        for (int x = 0; x < static_cast<int>(line.size()); ++x)
        {
            if (line[x] == 'P') m_playerSpawn = { x, y };
            else if (line[x] == 'E') m_enemySpawn = { x, y };
        }
        ++y;
    }

    const auto tryLoad = [](sf::Texture& tex, const std::string& file) {
        if (!tex.loadFromFile(file))
            std::cerr << "Failed to load texture: " << file << '\n';
    };

    tryLoad(m_texDirt, "Textures/Dirt.png");
    tryLoad(m_texRock, "Textures/Rock.png");
    tryLoad(m_texWall, "Textures/Wall.png");
    tryLoad(m_texGold, "Textures/Gold.png");
    tryLoad(m_texBack, "Textures/Back.png");

    m_tileSprite.setTexture(m_texBack);
}

void Level::draw(sf::RenderWindow& window)
{
    for (int y = 0; y < static_cast<int>(m_map.size()); ++y)
    {
        for (int x = 0; x < static_cast<int>(m_map[y].size()); ++x)
        {
            m_tileSprite.setPosition(
                static_cast<float>(x * TILE_SIZE),
                static_cast<float>(y * TILE_SIZE));

            char cell = m_map[y][x];
            switch (cell)
            {
            case '#': m_tileSprite.setTexture(m_texWall); break;
            case 'G': m_tileSprite.setTexture(m_texGold); break;
            case 'D': m_tileSprite.setTexture(m_texDirt); break;
            case 'R': m_tileSprite.setTexture(m_texRock); break;
            default: m_tileSprite.setTexture(m_texBack); break;
            }

            window.draw(m_tileSprite);
        }
    }
}

sf::Vector2i Level::getPlayerSpawn() const { return m_playerSpawn; }
sf::Vector2i Level::getEnemySpawn()  const { return m_enemySpawn; }

char Level::getTile(int x, int y) const
{
    if (y < 0 || y >= static_cast<int>(m_map.size()) ||
        x < 0 || x >= static_cast<int>(m_map[y].size()))
    {
        return '#';
    }
    return m_map[y][x];
}

void Level::setTile(int x, int y, char value)
{
    if (y < 0 || y >= static_cast<int>(m_map.size()) ||
        x < 0 || x >= static_cast<int>(m_map[y].size()))
        return;

    m_map[y][x] = value;
}

bool Level::hasGold() const
{
    for (const auto& row : m_map)
        if (row.find('G') != std::string::npos)
            return true;
    return false;
}