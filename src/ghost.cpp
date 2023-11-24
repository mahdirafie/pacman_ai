#include "ghost.hpp"

int Ghost::n_ghost_eaten = 0;

Ghost::Ghost()
{
    frightened_anim[(AnimationColor::BLUE)] = Animation("../images/frightened.png", 2, 0.3, 0, 0, 30, 30);
    frightened_anim[(AnimationColor::WHITE)] = Animation("../images/frightened.png", 2, 0.3, 0, 30, 30, 30);
    cur_anim_color = AnimationColor::BLUE;
    //
    eaten_anim[AnimationIndex::walkingUp] = Animation("../images/eaten.png", 2, 0.4, 0, 0, 30, 30);
    eaten_anim[AnimationIndex::walkingLeft] = Animation("../images/eaten.png", 2, 0.4, 0, 30, 30, 30);
    eaten_anim[AnimationIndex::walkingDown] = Animation("../images/eaten.png", 2, 0.4, 0, 60, 30, 30);
    eaten_anim[AnimationIndex::walkingRight] = Animation("../images/eaten.png", 2, 0.4, 0, 90, 30, 30);
    //
    s_c_table = std::vector<std::vector<float>>({{7, 27, 34, 54, 59, 79, 84},
    {7, 27, 34, 54, 59, 1092, 1093.6}, {5, 25, 30, 50, 55, 1092, 1093.6}});

    f_table = {{6, 5}, {5, 5}, {4, 5}, {3, 4}, {2, 4}, {4, 5}, {3, 4}, {2, 4}, {1, 3}, {1, 3}, {0, 0}};

    speeds = {{75, 50, 40}, {85, 55, 45}, {95, 60, 50}};

    real_speed = 100;
}

void Ghost::aim(const Pacman & pac)
{
    cur_mode -> start(this, pac);
}

void Ghost::aim(const Pacman & pac, sf::Vector2f blinkyPos)
{
    cur_mode -> start(this, pac, blinkyPos);
}

void Ghost::aim()
{
    cur_mode -> start(this);
}

void Ghost::set_pos(sf::Vector2f newPos)
{
    pos = newPos;
    Tile.setPosition(pos);
    sprite.setPosition(pos.x - 5, pos.y - 5);
}
void Ghost::set_speed(float newSpeed)
{
    speed = newSpeed;
}
void Ghost::set_dir(sf::Vector2f newDir)
{
    dir = newDir;
    if(dir.y == -1 && dir.x == 0)
    {
        cur_animation = AnimationIndex::walkingUp;
    }
    else if(dir.x == -1 && dir.y == 0)
    {
        cur_animation = AnimationIndex::walkingLeft;
    }
    else if(dir.y == 1 && dir.x == 0)
    {
        cur_animation = AnimationIndex::walkingDown;
    }
    else if(dir.x = 1 && dir.y == 0)
    {
        cur_animation = AnimationIndex::walkingRight;
    }
}
void Ghost::set_velocity(sf::Vector2f newVel)
{
    velocity = newVel;
}
void Ghost::set_target(sf::Vector2f newTar)
{
    target = newTar;
}
void Ghost::draw(sf::RenderTarget & window)
{
    // window.draw(Tile);
    window.draw(sprite);
}
void Ghost::move(float dt)
{
    Tile.move(velocity * dt);
    if(is_frightened())
    {
        frightened_anim[int(cur_anim_color)].Update(dt);
        frightened_anim[int(cur_anim_color)].ApplyToSprite(sprite);
    }
    else if(is_eaten())
    {
        eaten_anim[int(cur_animation)].Update(dt);
        eaten_anim[int(cur_animation)].ApplyToSprite(sprite);
    }
    else
    {
        animation[int(cur_animation)].Update(dt);
        animation[int(cur_animation)].ApplyToSprite(sprite);
    }
    set_pos(Tile.getPosition());
}
sf::FloatRect Ghost::get_rect() const
{
    return Tile.getGlobalBounds();
}

sf::Vector2f Ghost::get_dir() const
{
    return dir;
}

float Ghost::get_speed() const
{
    return speed;
}

sf::Vector2f Ghost::get_target() const
{
    return target;
}

sf::Vector2f Ghost::get_pos() const
{
    return pos;
}

sf::Vector2f Ghost::get_corner() const
{
    return corner;
}

void Ghost::change_mode(Mode * newMode)
{
    cur_mode = newMode;
}

void Ghost::turn_around_ghost(bool turnAround)
{
    turn_around = turnAround;
}
bool Ghost::turned_around() const
{
    return turn_around;
}
void Ghost::change_color(sf::Color color)
{
    Tile.setFillColor(color);
}
sf::Color Ghost::get_color() const
{
    return color;
}

void Ghost::make_frightened(bool scared)
{
    frightened = scared;
}
bool Ghost::is_frightened() const
{
    return frightened;
}
void Ghost::make_eaten(bool eat)
{
    eaten = eat;
}
bool Ghost::is_eaten() const
{
    return eaten;
}
void Ghost::make_in(bool In)
{
    in = In;
}
bool Ghost::is_in()
{
    return in;
}

sf::Vector2f Ghost::get_home_pos()
{
    return home_pos;
}

void Ghost::change_speed(unsigned int percent)
{
    percent = percent % 101;

    speed = real_speed * percent / 100;
}

void Ghost::manage_white_blue(int level)
{
    float time;
    float time_for_each_color;
    int size;

    if(level == 1)
    {
        time = this -> f_table[0].first;
        time_for_each_color = time / ((this -> f_table[0].second * 2) + 1);
        size = this -> f_table[0].second * 2;
    }
    else if(level == 2)
    {
        time = this -> f_table[1].first;
        time_for_each_color = time / ((this -> f_table[1].second * 2) + 1);
        size = this -> f_table[1].second * 2;
    }
    else if(level == 3)
    {
        time = this -> f_table[2].first;
        time_for_each_color = time / ((this -> f_table[2].second * 2) + 1);
        size = this -> f_table[2].second * 2;
    }
    else if(level == 4)
    {
        time = this -> f_table[3].first;
        time_for_each_color = time / ((this -> f_table[3].second * 2) + 1);
        size = this -> f_table[3].second * 2;
    }
    else if(level == 5)
    {
        time = this -> f_table[4].first;
        time_for_each_color = time / ((this -> f_table[4].second * 2) + 1);
        size = this -> f_table[4].second * 2;
    }
    else if(level >= 6 && level <= 10)
    {
        time = this -> f_table[5].first;
        time_for_each_color = time / ((this -> f_table[5].second * 2) + 1);
        size = this -> f_table[5].second * 2;
    }
    else if(level >= 11 && level <= 16)
    {
        time = this -> f_table[6].first;
        time_for_each_color = time / ((this -> f_table[6].second * 2) + 1);
        size = this -> f_table[6].second * 2;
    }
    else if(level >= 17 && level <= 32)
    {
        time = this -> f_table[7].first;
        time_for_each_color = time / ((this -> f_table[7].second * 2) + 1);
        size = this -> f_table[7].second * 2;
    }
    else if(level >= 33 && level <= 64)
    {
        time = this -> f_table[8].first;
        time_for_each_color = time / ((this -> f_table[8].second * 2) + 1);
        size = this -> f_table[8].second * 2;
    }
    else if(level >= 65 && level <= 128)
    {
        time = this -> f_table[9].first;
        time_for_each_color = time / ((this -> f_table[9].second * 2) + 1);
        size = this -> f_table[9].second * 2;
    }
    else if(level > 128)
    {
        //do nothing please
    }

    if(level <= 128)
    {
        for(int index = 0; index < size; ++index)
        {
            white_blue_time_table.push_back((index + 1) * time_for_each_color);
        }

        if(this -> f_timer > white_blue_time_table[white_blue_index])
        {
            if(cur_anim_color == AnimationColor::BLUE)
            {
                cur_anim_color = AnimationColor::WHITE;
            }
            else if(cur_anim_color == AnimationColor::WHITE)
            {
                cur_anim_color = AnimationColor::BLUE;
            }
            ++white_blue_index;
            if(white_blue_index == size)
            {
                white_blue_index = 0;
            }
        }
    }
}

void Ghost::set_initial_sprite()
{
    sprite.setTextureRect({0, 30, 30, 30});
}