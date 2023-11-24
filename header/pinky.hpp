#ifndef _PINKY
#define _PINKY

#include "ghost.hpp"

class Pinky : public Ghost
{
    public:
        Pinky();
        virtual void find_target(const Pacman &) override;
        virtual void find_target(const Pacman &, sf::Vector2f) override;
    private:
        
};

#endif // _PINKY