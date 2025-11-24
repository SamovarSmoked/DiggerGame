#include "Player.h"
#include "Level.h"
#include "SpriteUtils.h"
#include <iostream>

sf::Texture     Player::s_texture{};
sf::SoundBuffer Player::s_bufPickup{};
sf::Sound       Player::s_pickup{};

Player::Player(const std::string& texturePath, sf::Vector2i gridPos)
    : m_gridPos(gridPos)
{

    if (s_texture.getSize().x == 0 && !s_texture.loadFromFile(texturePath))
    {
        std::cerr << "Failed to load player texture: " << texturePath ;
    }


    if (s_bufPickup.getSampleCount() == 0)
    {
        if (!s_bufPickup.loadFromFile("Sounds/pickup.wav"))
        {
            std::cerr << "Failed to load sound: Sounds/pickup.wav" ;
        }
        else
        {
            s_pickup.setBuffer(s_bufPickup);
            s_pickup.setVolume(30.f);
        }
    }

    m_sprite.setTexture(s_texture);
    fitTile(m_sprite, s_texture, TILE_SIZE);
    m_sprite.setPosition(
        static_cast<float>(m_gridPos.x * TILE_SIZE),
        static_cast<float>(m_gridPos.y * TILE_SIZE));
}

void Player::handleInput(Level& level)
{
    if (m_moveClock.getElapsedTime().asSeconds() < MOVE_DELAY)
        return;

    sf::Vector2i dir{ 0, 0 };
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  dir.x = -1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dir.x = 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    dir.y = -1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  dir.y = 1;

    if (dir == sf::Vector2i{})
        return;

    const sf::Vector2i next = m_gridPos + dir;
    const char         cell = level.getTile(next.x, next.y);

    if (cell == '#' || cell == 'R')
        return;

    if (cell == 'G')
    {
        m_score += 10;
        level.setTile(next.x, next.y, ' ');
        if (s_pickup.getBuffer()) s_pickup.play();
    }
    else if (cell == 'D')
    {
        level.setTile(next.x, next.y, ' ');
    }

    m_gridPos = next;
    m_sprite.setPosition(
        static_cast<float>(m_gridPos.x * TILE_SIZE),
        static_cast<float>(m_gridPos.y * TILE_SIZE));

    m_moveClock.restart();
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

sf::Vector2i Player::getPosition() const { return m_gridPos; }
int Player::getScore()    const { return m_score; }
