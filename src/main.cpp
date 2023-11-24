#include "Game.hpp"



int main()
{
    Game * game = Game::get_instance();

    game -> run();

    return 0;
}