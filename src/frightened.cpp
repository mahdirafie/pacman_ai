#include "frightened.hpp"
#include <stdlib.h>
#include "ctime"

void Frightened::start(Ghost * ghost)
{
    //a random generated target can make a random movement
    //since we created the ghost based on having a target
    //it can be a good idea

    srand(static_cast<unsigned int>(time(NULL)));

    int length = 20;

    int height = rand() % 721;
    int width = rand() % 561;

    height = height / length * length;
    width = width / length * length;

    ghost -> set_target(sf::Vector2f(width, height));
}
void Frightened::start(Ghost *, const Pacman &)
{

}
void Frightened::start(Ghost *, const Pacman &, sf::Vector2f)
{

}