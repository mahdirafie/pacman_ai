#ifndef _BLINKY
#define _BLINKY

#include "ghost.hpp"

class Blinky : public Ghost
{
    public:
        Blinky();
        virtual void find_target(const Pacman &) override;
        virtual void find_target(const Pacman &, sf::Vector2f) override;
    private:
        
};

#endif // _BLINKY