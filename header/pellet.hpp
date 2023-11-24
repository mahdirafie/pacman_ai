#ifndef _PELLET
#define _PELLET

#include <SFML/Graphics.hpp>

class Pellet
{
    public:
        Pellet(bool, sf::Vector2f);
        void draw(sf::RenderTarget &);
        bool is_power_pellet();
        sf::FloatRect get_rect() const;
        int get_score();
    private:
        sf::Vector2f pos;
        sf::CircleShape shape;
        bool power_pellet;
        int score;
};

#endif // _PELLET