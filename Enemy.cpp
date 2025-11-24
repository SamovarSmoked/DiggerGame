#include "Enemy.h"
#include "Level.h"
#include "SpriteUtils.h"
#include <iostream>

sf::Texture Enemy::s_texture{};

Enemy::Enemy(const std::string& texturePath, sf::Vector2i gridPos) : m_gridPos(gridPos){
    if (s_texture.getSize().x == 0 && !s_texture.loadFromFile(texturePath))
    {
        std::cerr << "Failed to load enemy texture: " << texturePath << '\n';
    }

    m_sprite.setTexture(s_texture);
    fitTile(m_sprite, s_texture, TILE_SIZE);
    m_sprite.setPosition(static_cast<float>(m_gridPos.x * TILE_SIZE),
        static_cast<float>(m_gridPos.y * TILE_SIZE));
}

void Enemy::update(const Level& level, const sf::Vector2i& playerPos)
{
    if (m_moveClock.getElapsedTime().asSeconds() < MOVE_DELAY)
        return;

    const auto isWalkable = [&](int x, int y) {
        const char c = level.getTile(x, y);
        return (c == ' ' || c == 'B');
    };

    sf::Vector2i next = m_gridPos;
    if (playerPos.x < m_gridPos.x) next.x--;
    else if (playerPos.x > m_gridPos.x) next.x++;
    if (isWalkable(next.x, next.y))
        m_gridPos.x = next.x;

    next = m_gridPos;
    if (playerPos.y < m_gridPos.y) next.y--;
    else if (playerPos.y > m_gridPos.y) next.y++;
    if (isWalkable(next.x, next.y))
        m_gridPos.y = next.y;

    m_sprite.setPosition(static_cast<float>(m_gridPos.x * TILE_SIZE),
        static_cast<float>(m_gridPos.y * TILE_SIZE));

    m_moveClock.restart();
}

void Enemy::draw(sf::RenderWindow& window) const
{
    window.draw(m_sprite);
}

sf::Vector2i Enemy::getPosition() const { return m_gridPos; }
