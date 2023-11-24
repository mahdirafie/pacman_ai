#include "fruit.hpp"

Fruit::Fruit(int level)
{
    if(!texture.loadFromFile("../images/fruits.png"))
    {
        std::cerr << "Couldn't load the fruits image!" << std::endl;
        throw std::logic_error("Couldn't load the fruits image!\n");
    }
    sprite.setTexture(texture);
    if(level >= 1 && level <= 64)
    {
        score = 100;
        sprite.setTextureRect({0, 0, 20, 20});
    }
    else if(level >= 65 && level <= 128)
    {
        score = 300;
        sprite.setTextureRect({20, 0, 20, 20});
    }
    else if(level >= 129 && level <= 192)
    {
        score = 500;
        sprite.setTextureRect({40, 0, 20, 20});
    }
    else if(level >= 193 && level <= 224)
    {
        score = 700;
        sprite.setTextureRect({60, 0, 20, 20});
    }
    else if(level >= 225 && level <= 240)
    {
        score = 1000;
        sprite.setTextureRect({80, 0, 20, 20});
    }
    else if(level >= 241 && level <= 256)
    {
        score = 2000;
        sprite.setTextureRect({100, 0, 20, 20});
    }
    sprite.setPosition(13 * 20 + 10, 20 * 20);
}
int Fruit::get_score()
{
    return score;
}
void Fruit::draw(sf::RenderTarget & window)
{
    window.draw(sprite);
}
sf::FloatRect Fruit::get_rect()
{
    return sprite.getGlobalBounds();
}
void Fruit::manage_fruit(float dt, bool & is_added)
{
    if(is_added)
    {
        timer += dt;
        if(timer > 10)
        {
            is_added = false;
            timer = 0;
            time_is_up = true;
        }
    }
}

bool Fruit::is_time_up()
{
    return time_is_up;
}