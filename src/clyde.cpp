#include "clyde.hpp"
#include <cmath>

Clyde::Clyde()
{
    //animation
    animation[AnimationIndex::walkingUp] = Animation("../images/clyde.png", 2, 0.4, 0, 0, 30, 30);
    animation[AnimationIndex::walkingLeft] = Animation("../images/clyde.png", 2, 0.4, 0, 30, 30, 30);
    animation[AnimationIndex::walkingDown] = Animation("../images/clyde.png", 2, 0.4, 0, 60, 30, 30);
    animation[AnimationIndex::walkingRight] = Animation("../images/clyde.png", 2, 0.4, 0, 90, 30, 30);
    cur_animation = AnimationIndex::walkingUp;
    //
    sprite.setTexture(animation[0].get_texture());
    sprite.setTextureRect({0, 30, 30, 30});

    // timer.pause();
    color = sf::Color(168, 86, 19);
    speed = 100;
    in = true;
    corner = sf::Vector2f(0 * 20, 34 * 20);
    home_pos = sf::Vector2f(15 * 20 + 10, 17 * 20 + 10);

    Tile.setFillColor(color);
    Tile.setSize(sf::Vector2f(20, 20));
    Tile.setPosition(home_pos);
    sprite.setPosition(Tile.getPosition().x - 5, Tile.getPosition().y - 5);
}

void Clyde::find_target(const Pacman &, sf::Vector2f)
{

}

void Clyde::find_target(const Pacman & pac)
{

    //clyde acts like blinky
    //unless it is less than 8 tiles away from pacman
    //in this case this guy would go to sth like scatter mode

    int gridLength = 20;
    sf::Vector2f pacPosition;
    sf::Vector2f clydePosition;

    pacPosition.x = (int)pac.get_pos().x / gridLength * gridLength;
    pacPosition.y = (int)pac.get_pos().y / gridLength * gridLength;

    clydePosition.x = (int)get_pos().x / gridLength * gridLength;
    clydePosition.y = (int)get_pos().y / gridLength * gridLength;

    long int distance = (pacPosition.x - clydePosition.x) * (pacPosition.x - clydePosition.x) + (pacPosition.y - clydePosition.y) * (pacPosition.y - clydePosition.y);

    if(sqrt(distance) <= 160) // 8 tiles
    {
        set_target(sf::Vector2f(0 * gridLength, 34 * gridLength));
    }
    else
    {
        set_target(pac.get_pos());//just like blinky
    }
}