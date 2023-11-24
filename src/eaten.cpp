#include "eaten.hpp"

void Eaten::start(Ghost * ghost)
{
    ghost -> set_target(sf::Vector2f(13 * 20, 14 * 20));
}
void Eaten::start(Ghost *, const Pacman &)
{

}
void Eaten::start(Ghost *, const Pacman &, sf::Vector2f)
{

}