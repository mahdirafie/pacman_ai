#include "scatter.hpp"

void Scatter::start(Ghost * ghost)
{
    ghost -> set_target(ghost -> get_corner());
}
void Scatter::start(Ghost *, const Pacman &)
{

}
void Scatter::start(Ghost *, const Pacman &, sf::Vector2f)
{

}