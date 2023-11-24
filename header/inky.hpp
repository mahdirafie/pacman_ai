#ifndef _INKY
#define _INKY

#include "ghost.hpp"

class Inky : public Ghost
{
    public:
        Inky();
        virtual void find_target(const Pacman &) override;
        virtual void find_target(const Pacman &, sf::Vector2f) override;
    private:
        
};

#endif // _INKY