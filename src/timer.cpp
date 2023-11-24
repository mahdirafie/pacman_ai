#include "timer.hpp"

Timer::Timer()
{
    is_paused = false;
    running_time = 0;
    clock.restart();
}
void Timer::reset()
{
    is_paused = false;
    running_time = 0;
    clock.restart();
}
void Timer::pause()
{
    if(!is_paused)
    {
        running_time += clock.getElapsedTime().asSeconds();
    }
    is_paused = true;
}
void Timer::resume()
{
    if(is_paused)
    {
        clock.restart();
    }
    is_paused = false;
}
float Timer::get_elapsed_time()
{
    if(!is_paused)
    {
        return running_time + clock.getElapsedTime().asSeconds();
    }
    return running_time;
}
float Timer::restart()
{
    float return_value = get_elapsed_time();
    running_time = 0;
    is_paused = false;
    clock.restart();

    return return_value;
}