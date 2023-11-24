#include "tile.hpp"

tile::tile(sf::Color color)
{
    is_wall = false;
    is_pink = false;
    is_red = false;
    shape.setSize(sf::Vector2f(20, 20));
    shape.setFillColor(color);
}

int tile::get_tile_length()
{
    return tileLength;
}

void tile::set_position(sf::Vector2f newPos)
{
    pos = newPos;
    shape.setPosition(pos);
}

sf::Vector2f tile::get_position()
{
    return pos;
}

void tile::make_it_wall()
{
    is_wall = true;
    is_pink = false;
    is_red = false;
    is_door = false;
    shape.setFillColor(sf::Color::Black);
}

void tile::make_it_pink()
{
    is_pink = true;
    is_wall = false;
    is_red = false;
    is_door = false;
    shape.setFillColor(sf::Color(237, 76, 229, 100));
}

void tile::make_it_red()
{
    is_red = true;
    is_pink = false;
    is_wall = false;
    is_door = false;
    shape.setFillColor(sf::Color(247, 64, 73, 100));
}
void tile::make_it_door()
{
    is_red = false;
    is_pink = false;
    is_wall = true;
    is_door = true;
    // shape.setFillColor(sf::Color::Green);
}
bool tile::it_is_wall() const
{
    return is_wall;
}

bool tile::it_is_red() const
{
    return is_red;
}
bool tile::it_is_pink() const
{
    return is_pink;
}
bool tile::it_is_door() const
{
    return is_door;
}