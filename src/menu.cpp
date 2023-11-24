#include "menu.hpp"

Menu::Menu(float width, float height, sf::Vector2f starting_pos, float space, std::vector<std::string> items,
sf::Color theColor)
{
    color = theColor;

    if(!font.loadFromFile("../Fonts/FONT.ttf"))
    {
        std::cout << "COULDN'T LOAD THE FONT!\n" << std::endl;
    }

    if(starting_pos.y < 0)
    {
        starting_pos.y = -starting_pos.y;
    }
    if(starting_pos.x < 0)
    {
        starting_pos.x = -starting_pos.x;
    }
    starting_pos.y = (int)starting_pos.y % (int)height;
    starting_pos.x = (int)starting_pos.x % (int)width;

    for(int index = 0; index < items.size(); ++index)
    {
        if(starting_pos.y + 2 * space > height)
        {
            break;
        }
        main_menu_txts.push_back(sf::Text());
        main_menu_txts[index].setFont(font);
        main_menu_txts[index].setFillColor(sf::Color::White);
        main_menu_txts[index].setString(items[index]);
        main_menu_txts[index].setCharacterSize(70);
        main_menu_txts[index].setPosition(starting_pos.x, starting_pos.y);
        starting_pos.y += space;
    }

    main_menu_selected = 0;
    main_menu_txts[main_menu_selected].setFillColor(color);
}

Menu::~Menu()
{

}

int Menu::main_menu_pressed()
{
    return main_menu_selected;
}

void Menu::draw(sf::RenderTarget & window)
{
    for(int index = 0; index < main_menu_txts.size(); ++index)
    {
        window.draw(main_menu_txts[index]);
    }
}

void Menu::move_up()
{
    if(main_menu_selected >= 0
    && main_menu_selected < main_menu_txts.size())
    {
        main_menu_txts[main_menu_selected].setFillColor(sf::Color::White);
        --main_menu_selected;
        if(main_menu_selected == -1)
        {
            main_menu_selected = main_menu_txts.size() - 1;
        }
        main_menu_txts[main_menu_selected].setFillColor(color);
    }
}

void Menu::move_down()
{
    if(main_menu_selected + 1 <= main_menu_txts.size()
    && main_menu_selected + 1 >= 0)
    {
        main_menu_txts[main_menu_selected].setFillColor(sf::Color::White);
        ++main_menu_selected;
        if(main_menu_selected == main_menu_txts.size())
        {
            main_menu_selected = 0;
        }
        main_menu_txts[main_menu_selected].setFillColor(color);
    }
}