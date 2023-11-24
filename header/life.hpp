#ifndef _LIFE
#define _LIFE

#include <SFML/Graphics.hpp>

class Life
{
    public:
        Life()
        {
            if(!texture.loadFromFile("../images/life.png"))
            {
                throw std::logic_error("Couldn't load the life image\n");
            }
            sprite.setTexture(texture);
        }
        void set_pos(sf::Vector2f pos)
        {
            this -> pos = pos;
            sprite.setPosition(pos);
        }
        void draw(sf::RenderTarget & window)
        {
            window.draw(sprite);
        }
    private:
        sf::Texture texture;
        sf::Sprite sprite;
        sf::Vector2f pos;
};

#endif // _LIFE