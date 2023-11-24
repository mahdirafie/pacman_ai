#include "pinky.hpp"

Pinky::Pinky()
{
    //animation
    animation[AnimationIndex::walkingUp] = Animation("../images/pinky.png", 2, 0.4, 0, 0, 30, 30);
    animation[AnimationIndex::walkingLeft] = Animation("../images/pinky.png", 2, 0.4, 0, 30, 30, 30);
    animation[AnimationIndex::walkingDown] = Animation("../images/pinky.png", 2, 0.4, 0, 60, 30, 30);
    animation[AnimationIndex::walkingRight] = Animation("../images/pinky.png", 2, 0.4, 0, 90, 30, 30);
    cur_animation = AnimationIndex::walkingUp;
    //
    sprite.setTexture(animation[0].get_texture());
    sprite.setTextureRect({0, 30, 30, 30});

    // timer.pause();
    color = sf::Color(252, 3, 236);
    speed = 100;
    in = true;
    corner = sf::Vector2f(0 * 20, 2 * 20);
    home_pos = sf::Vector2f(13 * 20 + 10, 17 * 20 + 10);

    Tile.setFillColor(color);
    Tile.setSize(sf::Vector2f(20, 20));
    Tile.setPosition(home_pos);
    sprite.setPosition(Tile.getPosition().x - 5, Tile.getPosition().y - 5);
}

void Pinky::find_target(const Pacman &, sf::Vector2f)
{

}

void Pinky::find_target(const Pacman & pac)
{
    //pinky's target tile is four tiles in front of pacman

    int gridLength = 20;
    sf::Vector2f pacDirection = pac.get_dir();
    sf::Vector2f pacPosition;

    pacPosition.x = (int)pac.get_pos().x / gridLength * gridLength;
    pacPosition.y = (int)pac.get_pos().y / gridLength * gridLength;
    
    if(pacDirection.x == 1 && pacDirection.y == 0)
    {
        set_target(sf::Vector2f(pacPosition.x + (4 * gridLength), pacPosition.y));
    }
    else if(pacDirection.x == -1 && pacDirection.y == 0)
    {
        set_target(sf::Vector2f(pacPosition.x - (4 * gridLength), pacPosition.y));
    }
    else if(pacDirection.y == 1 && pacDirection.x == 0)
    {
        set_target(sf::Vector2f(pacPosition.x, pacPosition.y + (4 * gridLength)));
    }
    else if(pacDirection.y == -1 && pacDirection.x == 0)
    {
        set_target(sf::Vector2f(pacPosition.x, pacPosition.y - (4 * gridLength)));
    }
    else if(pacDirection.y == 0 && pacDirection.x == 0)
    {
        sf::Vector2f preDir = pac.get_preDir();
        if(preDir.x == 1 && preDir.y == 0)
        {
            set_target(sf::Vector2f(pacPosition.x + (4 * gridLength), pacPosition.y));
        }
        else if(preDir.x == -1 && preDir.y == 0)
        {
            set_target(sf::Vector2f(pacPosition.x - (4 * gridLength), pacPosition.y));
        }
        else if(preDir.y == 1 && preDir.x == 0)
        {
            set_target(sf::Vector2f(pacPosition.x, pacPosition.y + (4 * gridLength)));
        }
        else if(preDir.y == -1 && preDir.x == 0)
        {
            set_target(sf::Vector2f(pacPosition.x, pacPosition.y - (4 * gridLength)));
        }
    }
}