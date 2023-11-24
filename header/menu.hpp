#ifndef _MENU
#define _MENU

#include <SFML/Graphics.hpp>
#include <iostream>

class Menu
{
    public:
        Menu(float, float, sf::Vector2f, float, std::vector<std::string>, sf::Color);
        ~Menu();
        void draw(sf::RenderTarget &);
        void move_up();
        void move_down();
        int main_menu_pressed();
        bool mouse_is_on_it(sf::FloatRect, sf::Vector2f);
    private:
    int main_menu_selected;
    sf::Font font;
    std::vector<sf::Text> main_menu_txts;
    sf::Color color;
};

#endif // _MENU