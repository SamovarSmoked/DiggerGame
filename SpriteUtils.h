#pragma once
#include <SFML/Graphics.hpp>

inline void fitTile(sf::Sprite& sprite, const sf::Texture& tex, int tileSize)
{
    sprite.setScale(
        static_cast<float>(tileSize) / tex.getSize().x,
        static_cast<float>(tileSize) / tex.getSize().y);
}