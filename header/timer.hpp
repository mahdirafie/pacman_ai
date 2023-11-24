#ifndef _TIMER
#define _TIMER

#include <SFML/Graphics.hpp>

class Timer
{
    public:
        Timer();
        void reset();
        void pause();
        void resume();
        float get_elapsed_time();
        float restart();
    private:
        sf::Clock clock;
        float running_time;
        bool is_paused;
};

#endif // _TIMER