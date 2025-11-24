#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
class Level;

class Player
{
public:
    Player(const std::string& texturePath, sf::Vector2i gridPos);

    void handleInput(Level& level);
    void draw(sf::RenderWindow& window);

    [[nodiscard]] sf::Vector2i getPosition() const;
    [[nodiscard]] int          getScore()    const;

private:
    static sf::Texture s_texture;
    static sf::SoundBuffer s_bufPickup;
    static sf::Sound       s_pickup;

    sf::Sprite    m_sprite{};
    sf::Vector2i  m_gridPos{};
    sf::Clock     m_moveClock{};
    int           m_score{ 0 };

    static constexpr int   TILE_SIZE = 32;
    static constexpr float MOVE_DELAY = 0.15f;
};