#include "pellet.hpp"

Pellet::Pellet(bool isPower, sf::Vector2f thePos)
{
    power_pellet = isPower;
    shape.setFillColor(sf::Color(230, 156, 9));
    shape.setPosition(thePos);

    if(power_pellet)
    {
        shape.setRadius(8);
        score = 50;
    }
    else
    {
        shape.setRadius(4);
        score = 10;
    }
}

void Pellet::draw(sf::RenderTarget & window)
{
    window.draw(shape);
}

bool Pellet::is_power_pellet()
{
    return power_pellet;
}

sf::FloatRect Pellet::get_rect() const
{
    return shape.getGlobalBounds();
}

int Pellet::get_score()
{
    return score;
}