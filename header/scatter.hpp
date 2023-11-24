#ifndef _SCATTER
#define _SCATTER

#include "ghost.hpp"//includes mode in it

class Scatter : public Mode
{
    public:
        virtual void start(Ghost *) override;
        virtual void start(Ghost *, const Pacman &) override;
        virtual void start(Ghost *, const Pacman &, sf::Vector2f) override;
    private:
        
};

#endif // _SCATTER