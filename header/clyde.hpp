#ifndef _CLYDE
#define _CLYDE

#include "ghost.hpp"

class Clyde : public Ghost
{
    public:
        Clyde();
        virtual void find_target(const Pacman &) override;
        virtual void find_target(const Pacman &, sf::Vector2f) override;
    private:
        
};

#endif // _CLYDE