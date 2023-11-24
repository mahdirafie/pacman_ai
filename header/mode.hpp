#ifndef _MODE
#define _MODE

#include "pacman.hpp"

class Ghost;
class Mode
{
    public:
        virtual void start(Ghost *) = 0;//for all modes except chase mode
        virtual void start(Ghost *, const Pacman &) = 0;//for all ghost's chase mode except inky's
        virtual void start(Ghost *, const Pacman &, sf::Vector2f) = 0;//for inky's chase mode
    private:
        
};

#endif // _MODE