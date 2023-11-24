#include "blinky.hpp"

Blinky::Blinky()
{
    //animation
    animation[AnimationIndex::walkingUp] = Animation("../images/blinky.png", 2, 0.4, 0, 0, 30, 30);
    animation[AnimationIndex::walkingLeft] = Animation("../images/blinky.png", 2, 0.4, 0, 30, 30, 30);
    animation[AnimationIndex::walkingDown] = Animation("../images/blinky.png", 2, 0.4, 0, 60, 30, 30);
    animation[AnimationIndex::walkingRight] = Animation("../images/blinky.png", 2, 0.4, 0, 90, 30, 30);
    cur_animation = AnimationIndex::walkingLeft;
    //
    sprite.setTexture(animation[0].get_texture());
    sprite.setTextureRect({0, 30, 30, 30});

    color = sf::Color::Red;
    speed = 70;
    dir = {-1, 0};
    home_pos = sf::Vector2f(13 * 20 + 10, 14 * 20);
    corner = sf::Vector2f(25 * 20, 0 * 20);

    Tile.setFillColor(color);
    Tile.setSize(sf::Vector2f(20, 20));
    Tile.setPosition(home_pos);
    sprite.setPosition(Tile.getPosition().x - 5, Tile.getPosition().y - 5);
}

void Blinky::find_target(const Pacman &, sf::Vector2f)
{

}

void Blinky::find_target(const Pacman & pac)
{
    //blinky's target tile is exactly the tile that the pacman is on
    set_target(pac.get_pos());
}