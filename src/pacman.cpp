#include "pacman.hpp"

Pacman::Pacman()
{
    //animation
    animations[AnimationIndex::walkingUp] = Animation("../images/pacman.png", 3, 0.06, 0, 0, 30, 30);
    animations[AnimationIndex::walkingLeft] = Animation("../images/pacman.png", 3, 0.06, 0, 30, 30, 30);
    animations[AnimationIndex::walkingDown] = Animation("../images/pacman.png", 3, 0.06, 0, 60, 30, 30);
    animations[AnimationIndex::walkingRight] = Animation("../images/pacman.png", 3, 0.06, 0, 90, 30, 30);
    cur_animation = AnimationIndex::walkingLeft;

    deadAnimation = Animation("../images/dead_pac.png", 11, 0.092, 0, 0, 30, 30);

    sprite.setTexture(animations[0].get_texture());
    sprite.setTextureRect({0, 0, 30, 30});
    //
    real_speed = 100;
    speed = 85;
    dir = {-1, 0};
    home_pos = sf::Vector2f(270, 520);

    Tile.setFillColor(sf::Color::White);
    Tile.setSize(sf::Vector2f(20, 20));
    Tile.setPosition(home_pos);
    sprite.setPosition(Tile.getPosition().x - 5, Tile.getPosition().y - 5);
    //speed percentages

    speed_percentages = std::array<std::array<unsigned int, 4>, 4>({80, 71, 90, 79, 90, 79, 95, 83, 100, 87, 100, 87, 90, 79, 0, 0});
}

void Pacman::draw(sf::RenderTarget & window)
{
    // window.draw(Tile);
    window.draw(sprite);
}

void Pacman::set_pos(sf::Vector2f newPos)
{
    pos = newPos;
    Tile.setPosition(pos);
    sprite.setPosition(pos.x - 5, pos.y - 5);
}
sf::Vector2f Pacman::get_pos() const
{
    return pos;
}
void Pacman::set_dir(sf::Vector2f newDir)
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
    else if(dir.x == 1 && dir.y == 0)
    {
        cur_animation = AnimationIndex::walkingRight;
    }
}
sf::Vector2f Pacman::get_dir() const
{
    return dir;
}
void Pacman::set_speed(float newSpeed)
{
    speed = newSpeed;
}

void Pacman::move(float dt)
{
    Tile.move(velocity * dt);
    if(!(velocity.x == 0 && velocity.y == 0))
    {
        animations[int(cur_animation)].Update(dt);
        animations[int(cur_animation)].ApplyToSprite(sprite);
    }
    set_pos(Tile.getPosition());
}

void Pacman::set_velocity(sf::Vector2f newVel)
{
    velocity = newVel;
}
sf::Vector2f Pacman::get_velocity() const
{
    return velocity;
}

sf::FloatRect Pacman::get_rect() const
{
    return Tile.getGlobalBounds();
}
void Pacman::set_preDir(sf::Vector2f newDir)
{
    preDir = newDir;
}
sf::Vector2f Pacman::get_preDir() const
{
    return preDir;
}
void Pacman::increase_score(int newScore)
{
    previous_score = score;
    score += newScore;
}
long int Pacman::get_score() const
{
    return score;
}

void Pacman::set_score(long int newScore)
{
    score = newScore;
}

float Pacman::get_speed() const
{
    return speed;
}
void Pacman::make_energized(bool energized)
{
    isEnergized = energized;
}
bool Pacman::is_energized()
{
    return isEnergized;
}
sf::Vector2f Pacman::get_home_pos() const
{
    return home_pos;
}
void Pacman::operator--()
{
    --lives;
}

unsigned int Pacman::get_no_lives() const
{
    return lives;
}

void Pacman::set_no_lives(int noLives)
{
    lives = noLives;
}

void Pacman::operator++()
{
    ++eaten_pellets;
}
unsigned int Pacman::get_no_eaten_pellets() const
{
    return eaten_pellets;
}
void Pacman::reset_no_eaten_pellets()
{
    eaten_pellets = 0;
}
bool Pacman::wait(unsigned int no_frames, float dt)
{
    //assuming that the frame rate is 60

    hold_time += dt;
    if(no_frames * 0.0168 > hold_time)
    {
        return true;//means that it is still waiting
    }
    hold_time = 0;
    return false;
}
unsigned int Pacman::speed_manager(int level, int state)
{
    state = state % 4;
    if(level >= 1 && level <= 4)
    {
        return speed_percentages[0][state] / speed;
    }
    else if(level >= 5 && level <= 20)
    {
        return speed_percentages[1][state] / speed;
    }
    else if(level >= 21 && level <= 32)
    {
        return speed_percentages[2][state] / speed;
    }
    else if(level >= 33)
    {
        return speed_percentages[3][state] / speed;
    }
    return 80;
}

long int Pacman::get_added_score()
{
    return score - previous_score;
}

void Pacman::change_mode(unsigned int percent)
{
    percent = percent % 101;

    speed = real_speed * percent / 100;
}

void Pacman::set_initial_sprite()
{
    sprite.setTextureRect({0, 0, 30, 30});
}

void Pacman::play_dead_animation(float dt)
{
    deadAnimation.Update(dt);
    deadAnimation.ApplyToSprite(sprite);
}