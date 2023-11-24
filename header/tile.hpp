#ifndef _TILE
#define _TILE

#include <SFML/Graphics.hpp>
#include <iostream>

class tile
{ 
    public:
        tile(sf::Color);
        void set_position(sf::Vector2f);
        sf::Vector2f get_position();
        int get_tile_length();
        void make_it_wall();
        void make_it_pink();
        void make_it_red();
        void make_it_door();
        bool it_is_wall() const;
        bool it_is_red() const;
        bool it_is_pink() const;
        bool it_is_door() const;
        sf::RectangleShape shape; // the rectangle we use for the tile
    private:
        sf::Vector2f pos; // position of each tile
        int tileLength;// length of each tile
        bool is_wall;//we can distinguish wether it is a wall.
        bool is_pink;//for special areas
        bool is_red;//for special areas
        bool is_door;//for doors
};

#endif // _TILE