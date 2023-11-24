#ifndef _WALL
#define _WALL

#include <SFML/Graphics.hpp>
#include <array>

class Wall
{
    public:
        Wall(sf::Vector2f, sf::Vector2f); //making a line
        Wall(sf::Vector2f, sf::Vector2f, sf::Vector2f); //making a L
        Wall(sf::Vector2f, sf::Vector2f, sf::Color);
        const std::array<sf::RectangleShape, 2> getL();
        const sf::RectangleShape & getWall();
        bool DoesItLookLikeL();
    private:
        bool isL = false;
        sf::RectangleShape wall; //for a wall which looks like a line
        std::array<sf::RectangleShape, 2> L; //for a wall which looks like a L
        unsigned int wall_size = 20; // amount of space a single wall is going to take
                                     // and we get it equal to a tile size
};

#endif // _WALL