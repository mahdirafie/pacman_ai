#ifndef _FRIGHTENED
#define _FRIGHTENED

#include "ghost.hpp" //Has the mode.hpp in it

class Frightened : public Mode
{
    public:
        virtual void start(Ghost *) override;
        virtual void start(Ghost *, const Pacman &) override;
        virtual void start(Ghost *, const Pacman &, sf::Vector2f) override;
    private:
        
};

#endif // _FRIGHTENED