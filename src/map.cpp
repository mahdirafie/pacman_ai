#include "map.hpp"

Map::Map()
{
    //setting up tiles
    //just for drawing them
    //we going to distinguish which one of them
    //is a wall later
    for(int index = 0; index < tiles.size(); ++index)
    {
        for(int index2 = 0; index2 < tiles[index].size(); ++index2)
        {
            if(index % 2 == 1)
            {
                if(index2 % 2 == 1)
                {
                    tiles[index][index2] = new tile(sf::Color(83, 70, 163, 80));
                }
                else if(index2 % 2 == 0)
                {
                    tiles[index][index2] = new tile(sf::Color(88, 88, 89, 80));
                }
            }
            else if(index % 2 == 0)
            {
                if(index2 % 2 == 1)
                {
                    tiles[index][index2] = new tile(sf::Color(88, 88, 89, 80));
                }
                else if(index2 % 2 == 0)
                {
                    tiles[index][index2] = new tile(sf::Color(83, 70, 163, 80));
                }
            }
            tiles[index][index2] -> set_position(sf::Vector2f((index2 - 2) * gridLength, index * gridLength));
        }
    }
    //done with tiles
    //now let's take care of walls

    set_up_walls();
    distinguish_walls();
    set_pink_tiles();
    set_red_tiles();
    set_doors();
}

Map::~Map()
{
    for(auto & item: tiles)
    {
        for(auto & i: item)
        {
            delete i;
        }
    }
    for(auto & item: walls)
    {
        delete item;
    }
}

void Map::draw(sf::RenderTarget & window)
{
    // for(const auto & item: tiles)
    // {
    //     for(const auto & i: item)
    //     {
    //         window.draw(i -> shape);
    //     }
    // }
    for(const auto & item: walls)
    {
        if(item -> DoesItLookLikeL())
        {
            for(const auto & i: item -> getL())
            {
                window.draw(i);
            }
        }
        else
        {
            window.draw(item -> getWall());
        }
    }
}

sf::FloatRect Map::get_tile_rect(int row, int column)
{
    return tiles[row - 1][column + 1] -> shape.getGlobalBounds();
}

tile & Map::get_tile(sf::Vector2f tilePos)
{
    tilePos.x = (int)tilePos.x / gridLength;
    tilePos.y = (int)tilePos.y / gridLength;
    tilePos.x += 2;

    return *(tiles.at(tilePos.y).at(tilePos.x));
}

void Map::set_up_walls()
{
    int length = gridLength;
    //
    walls.push_back(new Wall(sf::Vector2f(0 * length, 13 * length), sf::Vector2f(0 * length, 3 * length), sf::Vector2f(28 * length, 3 * length)));
    setWallPoses(sf::Vector2f(0 * length, 13 * length), sf::Vector2f(0 * length, 3 * length), sf::Vector2f(28 * length, 3 * length));
    //
    walls.push_back(new Wall(sf::Vector2f(28 * length, 3 * length), sf::Vector2f(28 * length, 13 * length + 2), sf::Vector2f(23 * length, 13 * length + 2)));
    setWallPoses(sf::Vector2f(28 * length, 3 * length), sf::Vector2f(28 * length, 13 * length + 2), sf::Vector2f(23 * length, 13 * length + 2));

    walls.push_back(new Wall(sf::Vector2f(23 * length, 13 * length), sf::Vector2f(23 * length, 16 * length), sf::Vector2f(28 * length, 16 * length)));
    setWallPoses(sf::Vector2f(23 * length, 13 * length), sf::Vector2f(23 * length, 16 * length), sf::Vector2f(28 * length, 16 * length));

    walls.push_back(new Wall(sf::Vector2f(23 * length, 22 * length), sf::Vector2f(23 * length, 19 * length), sf::Vector2f(28 * length, 19 * length)));
    setWallPoses(sf::Vector2f(23 * length, 22 * length), sf::Vector2f(23 * length, 19 * length), sf::Vector2f(28 * length, 19 * length));

    walls.push_back(new Wall(sf::Vector2f(28 * length, 34 * length), sf::Vector2f(28 * length, 22 * length - 2), sf::Vector2f(23 * length, 22 * length - 2)));
    setWallPoses(sf::Vector2f(28 * length, 34 * length), sf::Vector2f(28 * length, 22 * length - 2), sf::Vector2f(23 * length, 22 * length - 2));

    walls.push_back(new Wall(sf::Vector2f(0 * length, 22 * length), sf::Vector2f(0 * length, 34 * length), sf::Vector2f(28 * length, 34 * length)));
    setWallPoses(sf::Vector2f(0 * length, 22 * length), sf::Vector2f(0 * length, 34 * length), sf::Vector2f(28 * length, 34 * length));

    walls.push_back(new Wall(sf::Vector2f(5 * length, 19 * length), sf::Vector2f(5 * length, 22 * length), sf::Vector2f(0 * length, 22 * length)));
    setWallPoses(sf::Vector2f(5 * length, 19 * length), sf::Vector2f(5 * length, 22 * length), sf::Vector2f(0 * length, 22 * length));

    walls.push_back(new Wall(sf::Vector2f(0 * length, 19 * length), sf::Vector2f(5 * length, 19 * length)));
    setWallPoses(sf::Vector2f(0 * length, 19 * length), sf::Vector2f(5 * length, 19 * length));

    walls.push_back(new Wall(sf::Vector2f(0 * length, 16 * length - 2), sf::Vector2f(5 * length, 16 * length - 2)));
    setWallPoses(sf::Vector2f(0 * length, 16 * length - 2), sf::Vector2f(5 * length, 16 * length - 2));

    walls.push_back(new Wall(sf::Vector2f(5 * length, 16 * length), sf::Vector2f(5 * length, 13 * length), sf::Vector2f(0 * length, 13 * length)));
    setWallPoses(sf::Vector2f(5 * length, 16 * length), sf::Vector2f(5 * length, 13 * length), sf::Vector2f(0 * length, 13 * length));


    // //
    walls.push_back(new Wall(sf::Vector2f(0 * length + 5, 27 * length + 10), sf::Vector2f(0 * length + 5, 22 * length + 3), sf::Vector2f(5 * length + 5, 22 * length + 3)));
    setWallPoses(sf::Vector2f(0 * length + 5, 27 * length + 10), sf::Vector2f(0 * length + 5, 22 * length + 3), sf::Vector2f(5 * length + 5, 22 * length + 3));

    walls.push_back(new Wall(sf::Vector2f(2 * length + 8, 28 * length + 8), sf::Vector2f(2 * length + 8, 27 * length + 10), sf::Vector2f(0 * length + 5, 27 * length + 10)));
    setWallPoses(sf::Vector2f(2 * length + 8, 28 * length + 8), sf::Vector2f(2 * length + 8, 27 * length + 10), sf::Vector2f(0 * length + 5, 27 * length + 10));

    walls.push_back(new Wall(sf::Vector2f(0 * length + 5, 34 * length - 5), sf::Vector2f(0 * length + 5, 28 * length + 8), sf::Vector2f(2 * length + 8, 28 * length + 8)));
    setWallPoses(sf::Vector2f(0 * length + 5, 34 * length - 5), sf::Vector2f(0 * length + 5, 28 * length + 8), sf::Vector2f(2 * length + 8, 28 * length + 8));

    walls.push_back(new Wall(sf::Vector2f(28 * length - 5, 28 * length + 8), sf::Vector2f(28 * length - 5, 34 * length - 5), sf::Vector2f(0 * length + 5, 34 * length - 5)));
    setWallPoses(sf::Vector2f(28 * length - 5, 28 * length + 8), sf::Vector2f(28 * length - 5, 34 * length - 5), sf::Vector2f(0 * length + 5, 34 * length - 5));

    walls.push_back(new Wall(sf::Vector2f(25 * length + 10, 27 * length + 12), sf::Vector2f(25 * length + 10, 28 * length + 10), sf::Vector2f(28 * length - 7, 28 * length + 10)));
    setWallPoses(sf::Vector2f(25 * length + 10, 27 * length + 12), sf::Vector2f(25 * length + 10, 28 * length + 10), sf::Vector2f(28 * length - 7, 28 * length + 10));

    walls.push_back(new Wall(sf::Vector2f(28 * length - 5, 22 * length + 5), sf::Vector2f(28 * length - 5, 27 * length + 12), sf::Vector2f(25 * length + 10, 27 * length + 12)));
    setWallPoses(sf::Vector2f(28 * length - 5, 22 * length + 5), sf::Vector2f(28 * length - 5, 27 * length + 12), sf::Vector2f(25 * length + 10, 27 * length + 12));

    walls.push_back(new Wall(sf::Vector2f(22 * length + 15, 18 * length + 15), sf::Vector2f(22 * length + 15, 22 * length + 5), sf::Vector2f(28 * length - 5, 22 * length + 5)));
    setWallPoses(sf::Vector2f(22 * length + 15, 18 * length + 15), sf::Vector2f(22 * length + 15, 22 * length + 5), sf::Vector2f(28 * length - 5, 22 * length + 5));

    walls.push_back(new Wall(sf::Vector2f(22 * length + 15, 18 * length + 15), sf::Vector2f(28 * length, 18 * length + 15)));
    setWallPoses(sf::Vector2f(22 * length + 15, 18 * length + 15), sf::Vector2f(28 * length, 18 * length + 15));

    walls.push_back(new Wall(sf::Vector2f(5 * length + 5, 22 * length + 5), sf::Vector2f(5 * length + 5, 18 * length + 15), sf::Vector2f(0 * length, 18 * length + 15)));
    setWallPoses(sf::Vector2f(5 * length + 5, 22 * length + 5), sf::Vector2f(5 * length + 5, 18 * length + 15), sf::Vector2f(0 * length, 18 * length + 15));

    walls.push_back(new Wall(sf::Vector2f(5 * length + 5, 12 * length + 15), sf::Vector2f(5 * length + 5, 16 * length + 5), sf::Vector2f(0 * length, 16 * length + 5)));
    setWallPoses(sf::Vector2f(5 * length + 5, 12 * length + 15), sf::Vector2f(5 * length + 5, 16 * length + 5), sf::Vector2f(0 * length, 16 * length + 5));

    walls.push_back(new Wall(sf::Vector2f(22 * length + 15, 12 * length + 15), sf::Vector2f(22 * length + 15, 16 * length + 5), sf::Vector2f(28 * length, 16 * length + 5)));
    setWallPoses(sf::Vector2f(22 * length + 15, 12 * length + 15), sf::Vector2f(22 * length + 15, 16 * length + 5), sf::Vector2f(28 * length, 16 * length + 5));

    walls.push_back(new Wall(sf::Vector2f(0 * length + 5, 3 * length + 5), sf::Vector2f(0 * length + 5, 12 * length + 17), sf::Vector2f(5 * length + 5, 12 * length + 17)));
    setWallPoses(sf::Vector2f(0 * length + 5, 3 * length + 5), sf::Vector2f(0 * length + 5, 12 * length + 17), sf::Vector2f(5 * length + 5, 12 * length + 17));

    walls.push_back(new Wall(sf::Vector2f(28 * length - 5, 3 * length + 5), sf::Vector2f(28 * length - 5, 12 * length + 17), sf::Vector2f(22 * length + 15, 12 * length + 17)));
    setWallPoses(sf::Vector2f(28 * length - 5, 3 * length + 5), sf::Vector2f(28 * length - 5, 12 * length + 17), sf::Vector2f(22 * length + 15, 12 * length + 17));

    walls.push_back(new Wall(sf::Vector2f(13 * length + 12, 7 * length + 5), sf::Vector2f(13 * length + 12, 3 * length + 5), sf::Vector2f(0 * length + 5, 3 * length + 5)));
    setWallPoses(sf::Vector2f(13 * length + 12, 7 * length + 5), sf::Vector2f(13 * length + 12, 3 * length + 5), sf::Vector2f(0 * length + 5, 3 * length + 5));

    walls.push_back(new Wall(sf::Vector2f(14 * length + 8, 7 * length + 5), sf::Vector2f(14 * length + 8, 3 * length + 5), sf::Vector2f(28 * length - 5, 3 * length + 5)));
    setWallPoses(sf::Vector2f(14 * length + 8, 7 * length + 5), sf::Vector2f(14 * length + 8, 3 * length + 5), sf::Vector2f(28 * length - 5, 3 * length + 5));

    walls.push_back(new Wall(sf::Vector2f(13 * length + 12, 7 * length + 5), sf::Vector2f(14 * length + 8, 7 * length + 5)));
    setWallPoses(sf::Vector2f(13 * length + 12, 7 * length + 5), sf::Vector2f(14 * length + 8, 7 * length + 5));

    // //now let's make the internal walls :(

    walls.push_back(new Wall(sf::Vector2f(2 * length + length / 2, 5 * length + length / 2), sf::Vector2f(2 * length + length / 2, 7 * length + length / 2), sf::Vector2f(5 * length + length / 2, 7 * length + length / 2)));
    setWallPoses(sf::Vector2f(2 * length + length / 2, 5 * length + length / 2), sf::Vector2f(2 * length + length / 2, 7 * length + length / 2), sf::Vector2f(5 * length + length / 2, 7 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(5 * length + length / 2, 7 * length + length / 2), sf::Vector2f(5 * length + length / 2, 5 * length + length / 2), sf::Vector2f(2 * length + length / 2, 5 * length + length / 2)));
    setWallPoses(sf::Vector2f(5 * length + length / 2, 7 * length + length / 2), sf::Vector2f(5 * length + length / 2, 5 * length + length / 2), sf::Vector2f(2 * length + length / 2, 5 * length + length / 2));

    walls.push_back(new Wall(sf::Vector2f(7 * length + length / 2, 5 * length + length / 2), sf::Vector2f(7 * length + length / 2, 7 * length + length / 2 ), sf::Vector2f(11 * length + length / 2, 7 * length + length / 2)));
    setWallPoses(sf::Vector2f(7 * length + length / 2, 5 * length + length / 2), sf::Vector2f(7 * length + length / 2, 7 * length + length / 2 ), sf::Vector2f(11 * length + length / 2, 7 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(11 * length + length / 2, 7 * length + length / 2), sf::Vector2f(11 * length + length / 2, 5 * length + length / 2), sf::Vector2f(7 * length + length / 2, 5 * length + length / 2)));
    setWallPoses(sf::Vector2f(11 * length + length / 2, 7 * length + length / 2), sf::Vector2f(11 * length + length / 2, 5 * length + length / 2), sf::Vector2f(7 * length + length / 2, 5 * length + length / 2));

    walls.push_back(new Wall(sf::Vector2f(16 * length + length / 2, 5 * length + length / 2), sf::Vector2f(16 * length + length / 2, 7 * length + length / 2), sf::Vector2f(20 * length + length / 2, 7 * length + length / 2)));
    setWallPoses(sf::Vector2f(16 * length + length / 2, 5 * length + length / 2), sf::Vector2f(16 * length + length / 2, 7 * length + length / 2), sf::Vector2f(20 * length + length / 2, 7 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(20 * length + length / 2, 7 * length + length / 2), sf::Vector2f(20 * length + length / 2, 5 * length + length / 2), sf::Vector2f(16 * length + length / 2, 5 * length + length / 2)));
    setWallPoses(sf::Vector2f(20 * length + length / 2, 7 * length + length / 2), sf::Vector2f(20 * length + length / 2, 5 * length + length / 2), sf::Vector2f(16 * length + length / 2, 5 * length + length / 2));

    walls.push_back(new Wall(sf::Vector2f(22 * length + length / 2, 5 * length + length / 2), sf::Vector2f(22 * length + length / 2, 7 * length + length / 2 ), sf::Vector2f(25 * length + length / 2, 7 * length + length / 2)));
    setWallPoses(sf::Vector2f(22 * length + length / 2, 5 * length + length / 2), sf::Vector2f(22 * length + length / 2, 7 * length + length / 2 ), sf::Vector2f(25 * length + length / 2, 7 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(25 * length + length / 2, 7 * length + length / 2), sf::Vector2f(25 * length + length / 2, 5 * length + length / 2), sf::Vector2f(22 * length + length / 2, 5 * length + length / 2)));
    setWallPoses(sf::Vector2f(25 * length + length / 2, 7 * length + length / 2), sf::Vector2f(25 * length + length / 2, 5 * length + length / 2), sf::Vector2f(22 * length + length / 2, 5 * length + length / 2));

    walls.push_back(new Wall(sf::Vector2f(2 * length + length / 2, 9 * length + length / 2), sf::Vector2f(2 * length + length / 2, 10 * length + length / 2), sf::Vector2f(5 * length + length / 2, 10 * length + length / 2)));
    setWallPoses(sf::Vector2f(2 * length + length / 2, 9 * length + length / 2), sf::Vector2f(2 * length + length / 2, 10 * length + length / 2), sf::Vector2f(5 * length + length / 2, 10 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(5 * length + length / 2, 10 * length + length / 2), sf::Vector2f(5 * length + length / 2, 9 * length + length / 2), sf::Vector2f(2 * length + length / 2, 9 * length + length / 2)));
    setWallPoses(sf::Vector2f(5 * length + length / 2, 10 * length + length / 2), sf::Vector2f(5 * length + length / 2, 9 * length + length / 2), sf::Vector2f(2 * length + length / 2, 9 * length + length / 2));

    walls.push_back(new Wall(sf::Vector2f(22 * length + length / 2, 9 * length + length / 2), sf::Vector2f(22 * length + length / 2, 10 * length + length / 2), sf::Vector2f(25 * length + length / 2, 10 * length + length / 2)));
    setWallPoses(sf::Vector2f(22 * length + length / 2, 9 * length + length / 2), sf::Vector2f(22 * length + length / 2, 10 * length + length / 2), sf::Vector2f(25 * length + length / 2, 10 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(25 * length + length / 2, 10 * length + length / 2), sf::Vector2f(25 * length + length / 2, 9 * length + length / 2), sf::Vector2f(22 * length + length / 2, 9 * length + length / 2)));
    setWallPoses(sf::Vector2f(25 * length + length / 2, 10 * length + length / 2), sf::Vector2f(25 * length + length / 2, 9 * length + length / 2), sf::Vector2f(22 * length + length / 2, 9 * length + length / 2));

    walls.push_back(new Wall(sf::Vector2f(10 * length + length / 2, 9 * length + length / 2), sf::Vector2f(10 * length + length / 2, 10 * length + length / 2), sf::Vector2f(13 * length+ length / 2, 10 * length + length / 2)));
    setWallPoses(sf::Vector2f(10 * length + length / 2, 9 * length + length / 2), sf::Vector2f(10 * length + length / 2, 10 * length + length / 2), sf::Vector2f(13 * length+ length / 2, 10 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(13 * length + length / 2, 10 * length + length / 2 - 2), sf::Vector2f(13 * length + length / 2, 13 * length + length / 2), sf::Vector2f(14 * length + length / 2, 13 * length + length / 2)));
    setWallPoses(sf::Vector2f(13 * length + length / 2, 10 * length + length / 2 - 2), sf::Vector2f(13 * length + length / 2, 13 * length + length / 2), sf::Vector2f(14 * length + length / 2, 13 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(14 * length + length / 2 - 2, 13 * length + length / 2 - 2), sf::Vector2f(14 * length + length / 2 - 2, 10 * length + length / 2 - 2), sf::Vector2f(17 * length + length / 2, 10 * length + length / 2 - 2)));
    setWallPoses(sf::Vector2f(14 * length + length / 2 - 2, 13 * length + length / 2 - 2), sf::Vector2f(14 * length + length / 2 - 2, 10 * length + length / 2 - 2), sf::Vector2f(17 * length + length / 2, 10 * length + length / 2 - 2));
    walls.push_back(new Wall(sf::Vector2f(17 * length + length / 2, 10 * length + length / 2), sf::Vector2f(17 * length + length / 2, 9 * length + length / 2), sf::Vector2f(10 * length + length / 2, 9 * length + length / 2)));
    setWallPoses(sf::Vector2f(17 * length + length / 2, 10 * length + length / 2), sf::Vector2f(17 * length + length / 2, 9 * length + length / 2), sf::Vector2f(10 * length + length / 2, 9 * length + length / 2));
    // //
    walls.push_back(new Wall(sf::Vector2f(10 * length + length / 2, 21 * length + length / 2), sf::Vector2f(10 * length + length / 2, 22 * length + length / 2), sf::Vector2f(13 * length+ length / 2, 22 * length + length / 2)));
    setWallPoses(sf::Vector2f(10 * length + length / 2, 21 * length + length / 2), sf::Vector2f(10 * length + length / 2, 22 * length + length / 2), sf::Vector2f(13 * length+ length / 2, 22 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(13 * length + length / 2, 22 * length + length / 2 - 2), sf::Vector2f(13 * length + length / 2, 25 * length + length / 2), sf::Vector2f(14 * length + length / 2, 25 * length + length / 2)));
    setWallPoses(sf::Vector2f(13 * length + length / 2, 22 * length + length / 2 - 2), sf::Vector2f(13 * length + length / 2, 25 * length + length / 2), sf::Vector2f(14 * length + length / 2, 25 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(14 * length + length / 2 - 2, 25 * length + length / 2 - 2), sf::Vector2f(14 * length + length / 2 - 2, 22 * length + length / 2 - 2), sf::Vector2f(17 * length + length / 2, 22 * length + length / 2 - 2)));
    setWallPoses(sf::Vector2f(14 * length + length / 2 - 2, 25 * length + length / 2 - 2), sf::Vector2f(14 * length + length / 2 - 2, 22 * length + length / 2 - 2), sf::Vector2f(17 * length + length / 2, 22 * length + length / 2 - 2));
    walls.push_back(new Wall(sf::Vector2f(17 * length + length / 2, 22 * length + length / 2), sf::Vector2f(17 * length + length / 2, 21 * length + length / 2), sf::Vector2f(10 * length + length / 2, 21 * length + length / 2)));
    setWallPoses(sf::Vector2f(17 * length + length / 2, 22 * length + length / 2), sf::Vector2f(17 * length + length / 2, 21 * length + length / 2), sf::Vector2f(10 * length + length / 2, 21 * length + length / 2));
    // //
    walls.push_back(new Wall(sf::Vector2f(10 * length + length / 2, 27 * length + length / 2), sf::Vector2f(10 * length + length / 2, 28 * length + length / 2), sf::Vector2f(13 * length+ length / 2, 28 * length + length / 2)));
    setWallPoses(sf::Vector2f(10 * length + length / 2, 27 * length + length / 2), sf::Vector2f(10 * length + length / 2, 28 * length + length / 2), sf::Vector2f(13 * length+ length / 2, 28 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(13 * length + length / 2, 28 * length + length / 2 - 2), sf::Vector2f(13 * length + length / 2, 31 * length + length / 2), sf::Vector2f(14 * length + length / 2, 31 * length + length / 2)));
    setWallPoses(sf::Vector2f(13 * length + length / 2, 28 * length + length / 2 - 2), sf::Vector2f(13 * length + length / 2, 31 * length + length / 2), sf::Vector2f(14 * length + length / 2, 31 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(14 * length + length / 2 - 2, 31 * length + length / 2 - 2), sf::Vector2f(14 * length + length / 2 - 2, 28 * length + length / 2 - 2), sf::Vector2f(17 * length + length / 2, 28 * length + length / 2 - 2)));
    setWallPoses(sf::Vector2f(14 * length + length / 2 - 2, 31 * length + length / 2 - 2), sf::Vector2f(14 * length + length / 2 - 2, 28 * length + length / 2 - 2), sf::Vector2f(17 * length + length / 2, 28 * length + length / 2 - 2));
    walls.push_back(new Wall(sf::Vector2f(17 * length + length / 2, 28 * length + length / 2), sf::Vector2f(17 * length + length / 2, 27 * length + length / 2), sf::Vector2f(10 * length + length / 2, 27 * length + length / 2)));
    setWallPoses(sf::Vector2f(17 * length + length / 2, 28 * length + length / 2), sf::Vector2f(17 * length + length / 2, 27 * length + length / 2), sf::Vector2f(10 * length + length / 2, 27 * length + length / 2));
    // //
    walls.push_back(new Wall(sf::Vector2f(7 * length + length / 2, 9 * length + length / 2), sf::Vector2f(7 * length + length / 2, 16 * length + length / 2), sf::Vector2f(8 * length + length / 2, 16 * length + length / 2)));
    setWallPoses(sf::Vector2f(7 * length + length / 2, 9 * length + length / 2), sf::Vector2f(7 * length + length / 2, 16 * length + length / 2), sf::Vector2f(8 * length + length / 2, 16 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(8 * length + length / 2 - 2, 16 * length + length / 2 - 2), sf::Vector2f(8 * length + length / 2 - 2, 13 * length + length / 2 - 2), sf::Vector2f(11 * length + length / 2 - 2, 13 * length + length / 2 - 2)));
    setWallPoses(sf::Vector2f(8 * length + length / 2 - 2, 16 * length + length / 2 - 2), sf::Vector2f(8 * length + length / 2 - 2, 13 * length + length / 2 - 2), sf::Vector2f(11 * length + length / 2 - 2, 13 * length + length / 2 - 2));
    walls.push_back(new Wall(sf::Vector2f(11 * length + length / 2, 13 * length + length / 2), sf::Vector2f(11 * length + length / 2, 12 * length + length / 2), sf::Vector2f(8 * length + length / 2, 12 * length + length / 2)));
    setWallPoses(sf::Vector2f(11 * length + length / 2, 13 * length + length / 2), sf::Vector2f(11 * length + length / 2, 12 * length + length / 2), sf::Vector2f(8 * length + length / 2, 12 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(8 * length + length / 2, 12 * length + length / 2 + 2), sf::Vector2f(8 * length + length / 2, 9 * length + length / 2), sf::Vector2f(7 * length + length / 2, 9 * length + length / 2)));
    setWallPoses(sf::Vector2f(8 * length + length / 2, 12 * length + length / 2 + 2), sf::Vector2f(8 * length + length / 2, 9 * length + length / 2), sf::Vector2f(7 * length + length / 2, 9 * length + length / 2));
    // //
    walls.push_back(new Wall(sf::Vector2f(20 * length + length / 2, 9 * length + length / 2), sf::Vector2f(20 * length + length / 2, 16 * length + length / 2), sf::Vector2f(19 * length + length / 2, 16 * length + length / 2)));
    setWallPoses(sf::Vector2f(20 * length + length / 2, 9 * length + length / 2), sf::Vector2f(20 * length + length / 2, 16 * length + length / 2), sf::Vector2f(19 * length + length / 2, 16 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(19 * length + length / 2 + 2, 16 * length + length / 2 - 2), sf::Vector2f(19 * length + length / 2 + 2, 13 * length + length / 2 - 2), sf::Vector2f(16 * length + length / 2 + 2, 13 * length + length / 2 - 2)));
    setWallPoses(sf::Vector2f(19 * length + length / 2 + 2, 16 * length + length / 2 - 2), sf::Vector2f(19 * length + length / 2 + 2, 13 * length + length / 2 - 2), sf::Vector2f(16 * length + length / 2 + 2, 13 * length + length / 2 - 2));
    walls.push_back(new Wall(sf::Vector2f(16 * length + length / 2, 13 * length + length / 2), sf::Vector2f(16 * length + length / 2, 12 * length + length / 2), sf::Vector2f(19 * length + length / 2, 12 * length + length / 2)));
    setWallPoses(sf::Vector2f(16 * length + length / 2, 13 * length + length / 2), sf::Vector2f(16 * length + length / 2, 12 * length + length / 2), sf::Vector2f(19 * length + length / 2, 12 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(19 * length + length / 2, 12 * length + length / 2 + 2), sf::Vector2f(19 * length + length / 2, 9 * length + length / 2), sf::Vector2f(20 * length + length / 2, 9 * length + length / 2)));
    setWallPoses(sf::Vector2f(19 * length + length / 2, 12 * length + length / 2 + 2), sf::Vector2f(19 * length + length / 2, 9 * length + length / 2), sf::Vector2f(20 * length + length / 2, 9 * length + length / 2));
    // //
    walls.push_back(new Wall(sf::Vector2f(7 * length + length / 2, 18 * length + length / 2), sf::Vector2f(7 * length + length / 2, 22 * length + length / 2), sf::Vector2f(8 * length + length / 2, 22 * length + length / 2)));
    setWallPoses(sf::Vector2f(7 * length + length / 2, 18 * length + length / 2), sf::Vector2f(7 * length + length / 2, 22 * length + length / 2), sf::Vector2f(8 * length + length / 2, 22 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(8 * length + length / 2, 22 * length + length / 2), sf::Vector2f(8 * length + length / 2, 18 * length + length / 2), sf::Vector2f(7 * length + length / 2, 18 * length + length / 2)));
    setWallPoses(sf::Vector2f(8 * length + length / 2, 22 * length + length / 2), sf::Vector2f(8 * length + length / 2, 18 * length + length / 2), sf::Vector2f(7 * length + length / 2, 18 * length + length / 2));
    // //
    walls.push_back(new Wall(sf::Vector2f(19 * length + length / 2, 18 * length + length / 2), sf::Vector2f(19 * length + length / 2, 22 * length + length / 2), sf::Vector2f(20 * length + length / 2, 22 * length + length / 2)));
    setWallPoses(sf::Vector2f(19 * length + length / 2, 18 * length + length / 2), sf::Vector2f(19 * length + length / 2, 22 * length + length / 2), sf::Vector2f(20 * length + length / 2, 22 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(20 * length + length / 2, 22 * length + length / 2), sf::Vector2f(20 * length + length / 2, 18 * length + length / 2), sf::Vector2f(19 * length + length / 2, 18 * length + length / 2)));
    setWallPoses(sf::Vector2f(20 * length + length / 2, 22 * length + length / 2), sf::Vector2f(20 * length + length / 2, 18 * length + length / 2), sf::Vector2f(19 * length + length / 2, 18 * length + length / 2));
    // //
    walls.push_back(new Wall(sf::Vector2f(7 * length + length / 2, 24 * length + length / 2), sf::Vector2f(7 * length + length / 2, 25 * length + length / 2), sf::Vector2f(11 * length + length / 2, 25 * length + length / 2)));
    setWallPoses(sf::Vector2f(7 * length + length / 2, 24 * length + length / 2), sf::Vector2f(7 * length + length / 2, 25 * length + length / 2), sf::Vector2f(11 * length + length / 2, 25 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(11 * length + length / 2, 25 * length + length / 2), sf::Vector2f(11 * length + length / 2, 24 * length + length / 2), sf::Vector2f(7 * length + length / 2, 24 * length + length / 2)));
    setWallPoses(sf::Vector2f(11 * length + length / 2, 25 * length + length / 2), sf::Vector2f(11 * length + length / 2, 24 * length + length / 2), sf::Vector2f(7 * length + length / 2, 24 * length + length / 2));
    // //
    walls.push_back(new Wall(sf::Vector2f(16 * length + length / 2, 24 * length + length / 2), sf::Vector2f(16 * length + length / 2, 25 * length + length / 2), sf::Vector2f(20 * length + length / 2, 25 * length + length / 2)));
    setWallPoses(sf::Vector2f(16 * length + length / 2, 24 * length + length / 2), sf::Vector2f(16 * length + length / 2, 25 * length + length / 2), sf::Vector2f(20 * length + length / 2, 25 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(20 * length + length / 2, 25 * length + length / 2), sf::Vector2f(20 * length + length / 2, 24 * length + length / 2), sf::Vector2f(16 * length + length / 2, 24 * length + length / 2)));
    setWallPoses(sf::Vector2f(20 * length + length / 2, 25 * length + length / 2), sf::Vector2f(20 * length + length / 2, 24 * length + length / 2), sf::Vector2f(16 * length + length / 2, 24 * length + length / 2));
    // //
    walls.push_back(new Wall(sf::Vector2f(2 * length + length / 2, 25 * length + length / 2), sf::Vector2f(2 * length + length / 2, 24 * length + length / 2), sf::Vector2f(5 * length + length / 2, 24 * length + length / 2)));
    setWallPoses(sf::Vector2f(2 * length + length / 2, 25 * length + length / 2), sf::Vector2f(2 * length + length / 2, 24 * length + length / 2), sf::Vector2f(5 * length + length / 2, 24 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(5 * length + length / 2, 24 * length + length / 2), sf::Vector2f(5 * length + length / 2, 28 * length + length / 2), sf::Vector2f(4 * length + length / 2, 28 * length + length / 2)));
    setWallPoses(sf::Vector2f(5 * length + length / 2, 24 * length + length / 2), sf::Vector2f(5 * length + length / 2, 28 * length + length / 2), sf::Vector2f(4 * length + length / 2, 28 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(4 * length + length / 2, 28 * length + length / 2), sf::Vector2f(4 * length + length / 2, 25 * length + length / 2), sf::Vector2f(2 * length + length / 2, 25 * length + length / 2)));
    setWallPoses(sf::Vector2f(4 * length + length / 2, 28 * length + length / 2), sf::Vector2f(4 * length + length / 2, 25 * length + length / 2), sf::Vector2f(2 * length + length / 2, 25 * length + length / 2));
    // //
    walls.push_back(new Wall(sf::Vector2f(25 * length + length / 2, 25 * length + length / 2), sf::Vector2f(25 * length + length / 2, 24 * length + length / 2), sf::Vector2f(22 * length + length / 2, 24 * length + length / 2)));
    setWallPoses(sf::Vector2f(25 * length + length / 2, 25 * length + length / 2), sf::Vector2f(25 * length + length / 2, 24 * length + length / 2), sf::Vector2f(22 * length + length / 2, 24 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(22 * length + length / 2, 24 * length + length / 2), sf::Vector2f(22 * length + length / 2, 28 * length + length / 2), sf::Vector2f(23 * length + length / 2, 28 * length + length / 2)));
    setWallPoses(sf::Vector2f(22 * length + length / 2, 24 * length + length / 2), sf::Vector2f(22 * length + length / 2, 28 * length + length / 2), sf::Vector2f(23 * length + length / 2, 28 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(23 * length + length / 2, 28 * length + length / 2), sf::Vector2f(23 * length + length / 2, 25 * length + length / 2), sf::Vector2f(25 * length + length / 2, 25 * length + length / 2)));
    setWallPoses(sf::Vector2f(23 * length + length / 2, 28 * length + length / 2), sf::Vector2f(23 * length + length / 2, 25 * length + length / 2), sf::Vector2f(25 * length + length / 2, 25 * length + length / 2));
    // //
    walls.push_back(new Wall(sf::Vector2f(8 * length + length / 2, 30 * length + length / 2), sf::Vector2f(8 * length + length / 2, 27 * length + length / 2), sf::Vector2f(7 * length + length / 2, 27 * length + length / 2)));
    setWallPoses(sf::Vector2f(8 * length + length / 2, 30 * length + length / 2), sf::Vector2f(8 * length + length / 2, 27 * length + length / 2), sf::Vector2f(7 * length + length / 2, 27 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(11 * length + length / 2, 31 * length + length / 2), sf::Vector2f(11 * length + length / 2, 30 * length + length / 2), sf::Vector2f(8 * length + length / 2 - 2, 30 * length + length / 2)));
    setWallPoses(sf::Vector2f(11 * length + length / 2, 31 * length + length / 2), sf::Vector2f(11 * length + length / 2, 30 * length + length / 2), sf::Vector2f(8 * length + length / 2 - 2, 30 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(2 * length + length / 2, 30 * length + length / 2), sf::Vector2f(2 * length + length / 2, 31 * length + length / 2), sf::Vector2f(11 * length + length / 2, 31 * length + length / 2)));
    setWallPoses(sf::Vector2f(2 * length + length / 2, 30 * length + length / 2), sf::Vector2f(2 * length + length / 2, 31 * length + length / 2), sf::Vector2f(11 * length + length / 2, 31 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(7 * length + length / 2, 27 * length + length / 2), sf::Vector2f(7 * length + length / 2, 30 * length + length / 2 + 2), sf::Vector2f(2 * length + length / 2, 30 * length + length / 2 + 2)));
    setWallPoses(sf::Vector2f(7 * length + length / 2, 27 * length + length / 2), sf::Vector2f(7 * length + length / 2, 30 * length + length / 2 + 2), sf::Vector2f(2 * length + length / 2, 30 * length + length / 2 + 2));
    // //
    walls.push_back(new Wall(sf::Vector2f(19 * length + length / 2, 30 * length + length / 2), sf::Vector2f(19 * length + length / 2, 27 * length + length / 2), sf::Vector2f(20 * length + length / 2, 27 * length + length / 2)));
    setWallPoses(sf::Vector2f(19 * length + length / 2, 30 * length + length / 2), sf::Vector2f(19 * length + length / 2, 27 * length + length / 2), sf::Vector2f(20 * length + length / 2, 27 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(16 * length + length / 2, 31 * length + length / 2), sf::Vector2f(16 * length + length / 2, 30 * length + length / 2), sf::Vector2f(19 * length + length / 2 + 2, 30 * length + length / 2)));
    setWallPoses(sf::Vector2f(16 * length + length / 2, 31 * length + length / 2), sf::Vector2f(16 * length + length / 2, 30 * length + length / 2), sf::Vector2f(19 * length + length / 2 + 2, 30 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(25 * length + length / 2, 30 * length + length / 2), sf::Vector2f(25 * length + length / 2, 31 * length + length / 2), sf::Vector2f(16 * length + length / 2, 31 * length + length / 2)));
    setWallPoses(sf::Vector2f(25 * length + length / 2, 30 * length + length / 2), sf::Vector2f(25 * length + length / 2, 31 * length + length / 2), sf::Vector2f(16 * length + length / 2, 31 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(20 * length + length / 2, 27 * length + length / 2), sf::Vector2f(20 * length + length / 2, 30 * length + length / 2 + 2), sf::Vector2f(25 * length + length / 2, 30 * length + length / 2 + 2)));
    setWallPoses(sf::Vector2f(20 * length + length / 2, 27 * length + length / 2), sf::Vector2f(20 * length + length / 2, 30 * length + length / 2 + 2), sf::Vector2f(25 * length + length / 2, 30 * length + length / 2 + 2));

    // //ghosts' house
    walls.push_back(new Wall(sf::Vector2f(10 * length + length / 2, 15 * length + length / 2), sf::Vector2f(10 * length + length / 2, 19 * length + length / 2), sf::Vector2f(17 * length + length / 2, 19 * length + length / 2)));
    setWallPoses(sf::Vector2f(10 * length + length / 2, 15 * length + length / 2), sf::Vector2f(10 * length + length / 2, 19 * length + length / 2), sf::Vector2f(17 * length + length / 2, 19 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(17 * length + length / 2, 19 * length + length / 2), sf::Vector2f(17 * length + length / 2, 15 * length + length / 2), sf::Vector2f(15 * length, 15 * length + length / 2)));
    setWallPoses(sf::Vector2f(17 * length + length / 2, 19 * length + length / 2), sf::Vector2f(17 * length + length / 2, 15 * length + length / 2), sf::Vector2f(15 * length, 15 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(10 * length + length / 2, 15 * length + length / 2), sf::Vector2f(13 * length, 15 * length + length / 2)));
    setWallPoses(sf::Vector2f(10 * length + length / 2, 15 * length + length / 2), sf::Vector2f(13 * length, 15 * length + length / 2));
    walls.push_back(new Wall(sf::Vector2f(11 * length - 2, 16 * length), sf::Vector2f(11 * length - 2, 19 * length + 2), sf::Vector2f(17 * length, 19 * length + 2)));
    setWallPoses(sf::Vector2f(11 * length - 2, 16 * length), sf::Vector2f(11 * length - 2, 19 * length + 2), sf::Vector2f(17 * length, 19 * length + 2));
    walls.push_back(new Wall(sf::Vector2f(17 * length + 2, 19 * length + 2), sf::Vector2f(17 * length + 2, 16 * length - 2), sf::Vector2f(15 * length, 16 * length - 2)));
    setWallPoses(sf::Vector2f(17 * length + 2, 19 * length + 2), sf::Vector2f(17 * length + 2, 16 * length - 2), sf::Vector2f(15 * length, 16 * length - 2));
    walls.push_back(new Wall(sf::Vector2f(11 * length - 2, 16 * length - 2), sf::Vector2f(13 * length, 16 * length - 2)));
    setWallPoses(sf::Vector2f(11 * length - 2, 16 * length - 2), sf::Vector2f(13 * length, 16 * length - 2));
    walls.push_back(new Wall(sf::Vector2f(13 * length - 2, 15 * length + length / 2), sf::Vector2f(13 * length - 2, 16 * length - 2)));
    setWallPoses(sf::Vector2f(13 * length - 2, 15 * length + length / 2), sf::Vector2f(13 * length - 2, 16 * length - 2));
    walls.push_back(new Wall(sf::Vector2f(15 * length, 15 * length + length / 2), sf::Vector2f(15 * length, 16 * length - 2)));
    setWallPoses(sf::Vector2f(15 * length, 15 * length + length / 2), sf::Vector2f(15 * length, 16 * length - 2));
    walls.push_back(new Wall(sf::Vector2f(13 * length, 16 * length - 6), sf::Vector2f(15 * length, 16 * length - 6), sf::Color::White));
    // ghost should be able to pass the door do we won't consider it as a wall :(
    
    setWallPoses(sf::Vector2f(13 * length, 16 * length - 6), sf::Vector2f(15 * length, 16 * length - 6));
}

void Map::setWallPoses(sf::Vector2f first, sf::Vector2f second, sf::Vector2f third)
{
    if(first.x == second.x && second.y == third.y && first.y < second.y && second.x < third.x)
    {
        // then it is going to look like this: |_
        setWallPoses(first, second);
        second.x += 2;
        second.y -= 2;
        third.y -= 2;
        setWallPoses(second, third);
    }
    else if(first.x == second.x && second.y == third.y && first.y > second.y && second.x < third.x)
    {
        // then it is going to look like this: _
        //                                    |
        setWallPoses(second, third);
        second.y += 2;
        setWallPoses(second, first);
    }
    else if(first.x == second.x && second.y == third.y && first.y < second.y && second.x > third.x)
    {
        //then it is going to look like this: _|
        first.x -= 2;
        second.x -= 2;
        setWallPoses(first, second);
        third.y -= 2;
        second.y -= 2;
        setWallPoses(third, second);
    }
    else if(first.x == second.x && second.y == third.y && first.y > second.y && second.x > third.x)
    {
        //then it is going to look like this: _
        //                                    |
        first.x -= 2;
        second.x -= 2;
        setWallPoses(second, first);
        setWallPoses(third, second);
    }
}

void Map::setWallPoses(sf::Vector2f first, sf::Vector2f second)
{
    if(first.x == 560)
    {
        first.x -= 2;
    }
    if(second.x == 560)
    {
        second.x -= 2;
    }
    if(first.y == 680)
    {
        first.y -= 2;
    }
    if(second.y == 680)
    {
        second.y -= 2;
    }

    int length = gridLength;
    sf::Vector2f temp(0, 0);
    int reachPoint;
    if(second.x - first.x == 0)
    {
        if(second.y - first.y > 0)
        {
            temp.x = (int)first.x / length * length;
            temp.y = (int)first.y / length * length;
            if((int)second.y % length > 0)
            {
                reachPoint = (int)second.y / length * length;
            }
            else if((int)second.y % length == 0)
            {
                reachPoint = (int)second.y / length * length - 1 * length;
            }
            while(reachPoint + length != temp.y)
            {
                if(!wallPosExists(temp))
                {
                    wallsPoses.push_back(temp);
                }
                temp.y += length;
            }
        }
        else if(second.y - first.y < 0)
        {
            temp.x = (int)second.x / length * length;
            temp.y = (int)second.y / length * length;
            if((int)first.y % length > 0)
            {
                reachPoint = (int)first.y / length * length;
            }
            else if((int)first.y % length == 0)
            {
                reachPoint = (int)first.y / length * length - 1 * length;
            }
            while(reachPoint + length != temp.y)
            {
                if(!wallPosExists(temp))
                {
                    wallsPoses.push_back(temp);
                }
                temp.y += length;
            }
        }
    }
    else if(second.y - first.y == 0)
    {
        if(second.x - first.x > 0)
        {
            temp.x = (int)first.x / length * length;
            temp.y = (int)first.y / length * length;
            if((int)second.x % length > 0)
            {
                reachPoint = (int)second.x / length * length;
            }
            else if((int)second.x % length == 0)
            {
                reachPoint = (int)second.x / length * length - 1 * length;
            }
            while(reachPoint + length != temp.x)
            {
                if(!wallPosExists(temp))
                {
                    wallsPoses.push_back(temp);
                }
                temp.x += length;
            }
        }
        else if(second.x - first.x < 0)
        {
            temp.x = (int)second.x / length * length;
            temp.y = (int)second.y / length * length;
            if((int)first.x % length > 0)
            {
                reachPoint = (int)first.x / length * length;
            }
            else if((int)first.x % length == 0)
            {
                reachPoint = (int)first.x / length * length - 1 * length;
            }
            while(reachPoint + length != temp.x)
            {
                if(!wallPosExists(temp))
                {
                    wallsPoses.push_back(temp);
                }
                temp.x += length;
            }
        }
    }
    else
    {
        throw std::invalid_argument("Invalid Input for setting wall positions!\n");
    }
}

bool Map::wallPosExists(sf::Vector2f curWall)
{
    for(auto const & item: wallsPoses)
    {
        if(item == curWall)
        {
            return true;
        }
    }

    return false;
}

void Map::distinguish_walls()
{
    for(auto item : wallsPoses)
    {
        item.x /= gridLength;
        item.y /= gridLength;
        item.x += 2;

        tiles.at(item.y).at(item.x) -> make_it_wall();
    }
    //these are out of the screen but for safety (magin tunnel) we are going to make these tiles walls as well.
    tiles.at(16).at(30) -> make_it_wall();
    tiles.at(16).at(31) -> make_it_wall();
    tiles.at(18).at(30) -> make_it_wall();
    tiles.at(18).at(31) -> make_it_wall();
    tiles.at(17).at(31) -> make_it_wall();
    //
    tiles.at(16).at(0) -> make_it_wall();
    tiles.at(16).at(1) -> make_it_wall();
    tiles.at(18).at(0) -> make_it_wall();
    tiles.at(18).at(1) -> make_it_wall();
    tiles.at(17).at(0) -> make_it_wall();
}

void Map::set_pink_tiles()
{
    tiles.at(17).at(1) -> make_it_pink();
    tiles.at(17).at(2) -> make_it_pink();
    tiles.at(17).at(3) -> make_it_pink();
    tiles.at(17).at(4) -> make_it_pink();
    tiles.at(17).at(5) -> make_it_pink();
    tiles.at(17).at(6) -> make_it_pink();
    //---------
    tiles.at(17).at(30) -> make_it_pink();
    tiles.at(17).at(29) -> make_it_pink();
    tiles.at(17).at(28) -> make_it_pink();
    tiles.at(17).at(27) -> make_it_pink();
    tiles.at(17).at(26) -> make_it_pink();
    tiles.at(17).at(25) -> make_it_pink();
}
void Map::set_red_tiles()
{
    tiles.at(14).at(14) -> make_it_red();
    tiles.at(14).at(15) -> make_it_red();
    tiles.at(14).at(16) -> make_it_red();
    tiles.at(14).at(17) -> make_it_red();
    //---------
    tiles.at(26).at(14) -> make_it_red();
    tiles.at(26).at(15) -> make_it_red();
    tiles.at(26).at(16) -> make_it_red();
    tiles.at(26).at(17) -> make_it_red();
}

void Map::set_doors()
{
    tiles.at(15).at(15) -> make_it_door();
}

int Map::get_grid_length()
{
    return gridLength;
}