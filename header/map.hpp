#ifndef _MAP
#define _MAP

#include <array>
#include <vector>
#include "tile.hpp"
#include "wall.hpp"

class Map
{
    public:
        Map();
        ~Map();
        void draw(sf::RenderTarget &);
        sf::FloatRect get_tile_rect(int, int);
        tile & get_tile(sf::Vector2f);
        // this function is not relatable to the class :(
        int get_grid_length();
        //the functions below are just part of the Map constructor(private functions)
    private:
        void set_up_walls();
        void setWallPoses(sf::Vector2f, sf::Vector2f);
        void setWallPoses(sf::Vector2f, sf::Vector2f, sf::Vector2f);
        bool wallPosExists(sf::Vector2f);//wether a wall pos exist for pushing back to the wallsPoses vector
        void distinguish_walls();//we distinguish walls using wallsPoses vector
        void set_pink_tiles();
        void set_red_tiles();
        void set_doors();
    public:
        std::array<std::array<tile*, 32>, 36> tiles;//tiles of the map
    private:
        std::vector<Wall *> walls;//i didn't know how many walls we want so i used vector
        std::vector<sf::Vector2f> wallsPoses;//includes positions of the walls
        int gridLength = 20;//length of each grid which contains a tile
                            // equal to tileLength because all the tiles together would make a grid
};

#endif // _MAP