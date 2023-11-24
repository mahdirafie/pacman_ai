#include "chase.hpp"

void Chase::start(Ghost *)
{

}
void Chase::start(Ghost * ghost, const Pacman & pac)
{
    ghost -> find_target(pac);
}
void Chase::start(Ghost * ghost, const Pacman & pac, sf::Vector2f blinkyPos)
{
    ghost -> find_target(pac, blinkyPos);
}