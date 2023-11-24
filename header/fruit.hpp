#ifndef FRUIT
#define FRUIT

#include <SFML/Graphics.hpp>
#include <iostream>

class Fruit
{
    public:
        Fruit(int);
        int get_score();
        void draw(sf::RenderTarget &);
        sf::FloatRect get_rect();
        void manage_fruit(float, bool &);
        bool is_time_up();
    private:
        int score;
        sf::Texture texture;
        sf::Sprite sprite;
        float timer = 0;
        sf::Vector2f pos;
        bool time_is_up = false;
};

#endif // FRUIT