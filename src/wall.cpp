#include "wall.hpp"
#include <stdexcept>
#include <iostream>

Wall::Wall(sf::Vector2f firstP, sf::Vector2f secondP)
{
    //with this constructor we can actually create a line
    //since these lines are either vertical or horizontal
    //and the map consist of shapes created only with
    //these lines and not the diagonal lines we can 
    //create the map... so i think it is going to be
    //ENOUGH
    isL = false;
    wall.setFillColor(sf::Color::Blue);
    unsigned int number_of_walls;
    int remainder;

    if(secondP.x - firstP.x == 0)
    {
        if(secondP.y - firstP.y < 0)
        {
            number_of_walls = (firstP.y - secondP.y) / wall_size;
            wall.setSize(sf::Vector2f(2, firstP.y - secondP.y));
            wall.setPosition(secondP.x, secondP.y);
        }
        else if(secondP.y - firstP.y > 0)
        {
            number_of_walls = (secondP.y - firstP.y) / wall_size;
            wall.setSize(sf::Vector2f(2, (secondP.y - firstP.y)));
            wall.setPosition(firstP.x, firstP.y);
        }
        else if(secondP.y - firstP.y == 0)
        {
            return;
        }
    }
    else if(secondP.y - firstP.y == 0)
    {
        if(secondP.x - firstP.x < 0)
        {
            number_of_walls = (firstP.x - secondP.x) / wall_size;
            wall.setSize(sf::Vector2f((firstP.x - secondP.x), 2));
            wall.setPosition(secondP.x, secondP.y);
        }
        else if(secondP.x - firstP.x > 0)
        {
            number_of_walls = (secondP.x - firstP.x) / wall_size;
            wall.setSize(sf::Vector2f((secondP.x - firstP.x), 2));
            wall.setPosition(firstP.x, firstP.y);  
        }
        else if(secondP.x - firstP.x == 0)
        {
            return;
        }
    }
    else
    {
        throw std::invalid_argument("Can not draw a vertical or horizontal line with these inputs!\n");
    }
}

Wall::Wall(sf::Vector2f first, sf::Vector2f second, sf::Vector2f third)
{
    //since the developer is the one who input sth and not the user
    //it seems like the validation for the constructor "Wall(sf::Vector2f, sf::Vector2f)"
    //is useless, so in this constructor I try to keep it simple

    //first |
    //      |
    //      |_____
    //second      third

    isL = true;

    L[0].setFillColor(sf::Color::Blue); //vertical one
    L[1].setFillColor(sf::Color::Blue); //horizontal one

    if(first.x == second.x && second.y == third.y && first.y < second.y && second.x < third.x)
    {
        // then it is going to look like this: |_

        //setting the sizes:
        L[0].setSize(sf::Vector2f(2, (second.y) - first.y));
        L[1].setSize(sf::Vector2f(third.x - second.x - 2, 2));
        //setting the positions for each:
        L[0].setPosition(first.x, first.y);
        L[1].setPosition(second.x + 2, second.y - 2);
    }
    else if(first.x == second.x && second.y == third.y && first.y > second.y && second.x < third.x)
    {
        // then it is going to look like this: _
        //                                    |
        L[0].setSize(sf::Vector2f(2, first.y - (second.y)));
        L[1].setSize(sf::Vector2f(third.x - (second.x + 2), 2));

        L[0].setPosition(second.x, second.y);
        L[1].setPosition(second.x + 2, second.y);
    }
    else if(first.x == second.x && second.y == third.y && first.y < second.y && second.x > third.x)
    {
        //then it is going to look like this: _|
        L[0].setSize(sf::Vector2f(2, (second.y) - first.y));
        L[1].setSize(sf::Vector2f(second.x - 2 - third.x, 2));

        L[0].setPosition(first.x - 2, first.y);
        L[1].setPosition(third.x, third.y - 2);
    }
    else if(first.x == second.x && second.y == third.y && first.y > second.y && second.x > third.x)
    {
        //then it is going to look like this: _
        //                                    |
        L[0].setSize(sf::Vector2f(2, first.y - (second.y)));
        L[1].setSize(sf::Vector2f(second.x - 2 - third.x, 2));

        L[0].setPosition(second.x - 2, second.y);
        L[1].setPosition(third.x, third.y);
    }
    else
    {
        //then it is invalid
        throw std::invalid_argument("Invalid argument for L shape!\n");
    }
}

Wall::Wall(sf::Vector2f firstP, sf::Vector2f secondP, sf::Color color): Wall(firstP, secondP)
{
    //this constructor is used for the door
    wall.setFillColor(color);
    wall.setSize(sf::Vector2f(wall.getSize().x, 4));
}

const sf::RectangleShape & Wall::getWall()
{
    return wall;
}

const std::array<sf::RectangleShape, 2> Wall::getL()
{
    return L;
}

bool Wall::DoesItLookLikeL()
{
    return isL;
}