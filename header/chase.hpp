#ifndef _CHASE
#define _CHASE

#include "ghost.hpp"//has the mode.hpp in it

class Chase : public Mode
{
    public:
        virtual void start(Ghost *) override;
        virtual void start(Ghost *, const Pacman &) override;
        virtual void start(Ghost *, const Pacman &, sf::Vector2f) override;
    private:
        
};

#endif // _CHASE