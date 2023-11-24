#ifndef _EATEN
#define _EATEN

#include "ghost.hpp"//the mode.hpp is in it

class Eaten : public Mode
{
    public:
        virtual void start(Ghost *) override;
        virtual void start(Ghost *, const Pacman &) override;
        virtual void start(Ghost *, const Pacman &, sf::Vector2f) override;
    private:
        
};

#endif // _EATEN