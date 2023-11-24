#include "inky.hpp"

Inky::Inky()
{
    //animation
    animation[AnimationIndex::walkingUp] = Animation("../images/inky.png", 2, 0.4, 0, 0, 30, 30);
    animation[AnimationIndex::walkingLeft] = Animation("../images/inky.png", 2, 0.4, 0, 30, 30, 30);
    animation[AnimationIndex::walkingDown] = Animation("../images/inky.png", 2, 0.4, 0, 60, 30, 30);
    animation[AnimationIndex::walkingRight] = Animation("../images/inky.png", 2, 0.4, 0, 90, 30, 30);
    cur_animation = AnimationIndex::walkingUp;
    //
    sprite.setTexture(animation[0].get_texture());
    sprite.setTextureRect({0, 30, 30, 30});

    // timer.pause();
    color = sf::Color::Cyan;
    speed = 100;
    in = true;
    corner = sf::Vector2f(27 * 20, 34 * 20);
    home_pos = sf::Vector2f(11 * 20 + 10, 17 * 20 + 10);

    Tile.setFillColor(color);
    Tile.setSize(sf::Vector2f(20, 20));
    Tile.setPosition(home_pos);
    sprite.setPosition(Tile.getPosition().x - 5, Tile.getPosition().y - 5);
}

void Inky::find_target(const Pacman &)
{

}

void Inky::find_target(const Pacman & pac, sf::Vector2f blinkyPos)
{
    //you'd better know how inky finds the target :/

    int gridLength = 20;
    sf::Vector2f pacDirection = pac.get_dir();
    sf::Vector2f pacPosition;
    sf::Vector2f two_tiles_front;
    sf::Vector2f inky_target;
    
    blinkyPos.x = (int)blinkyPos.x / gridLength * gridLength;
    blinkyPos.y = (int)blinkyPos.y / gridLength * gridLength;

    pacPosition.x = (int)pac.get_pos().x / gridLength * gridLength;
    pacPosition.y = (int)pac.get_pos().y / gridLength * gridLength;
    
    if(pacDirection.x == 1 && pacDirection.y == 0)
    {
        two_tiles_front = sf::Vector2f(pacPosition.x + (2 * gridLength), pacPosition.y);
    }
    else if(pacDirection.x == -1 && pacDirection.y == 0)
    {
        two_tiles_front = sf::Vector2f(pacPosition.x - (2 * gridLength), pacPosition.y);
    }
    else if(pacDirection.y == 1 && pacDirection.x == 0)
    {
        two_tiles_front = sf::Vector2f(pacPosition.x, pacPosition.y + (2 * gridLength));
    }
    else if(pacDirection.y == -1 && pacDirection.x == 0)
    {
        two_tiles_front = sf::Vector2f(pacPosition.x, pacPosition.y - (2 * gridLength));
    }
    else if(pacDirection.y == 0 && pacDirection.x == 0)
    {
        sf::Vector2f preDir = pac.get_preDir();
        if(preDir.x == 1 && preDir.y == 0)
        {
            two_tiles_front = sf::Vector2f(pacPosition.x + (4 * gridLength), pacPosition.y);
        }
        else if(preDir.x == -1 && preDir.y == 0)
        {
            two_tiles_front = sf::Vector2f(pacPosition.x - (4 * gridLength), pacPosition.y);
        }
        else if(preDir.y == 1 && preDir.x == 0)
        {
            two_tiles_front = sf::Vector2f(pacPosition.x, pacPosition.y + (4 * gridLength));
        }
        else if(preDir.y == -1 && preDir.x == 0)
        {
            two_tiles_front = sf::Vector2f(pacPosition.x, pacPosition.y - (4 * gridLength));
        }
    }
    blinkyPos.x -= two_tiles_front.x;
    blinkyPos.y -= two_tiles_front.y;

    inky_target.x = -blinkyPos.x;
    inky_target.y = -blinkyPos.y;

    inky_target.x += two_tiles_front.x;
    inky_target.y += two_tiles_front.y;

    set_target(inky_target);
}