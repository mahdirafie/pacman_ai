#ifndef _PACMAN
#define _PACMAN

#include <SFML/Graphics.hpp>
#include "tile.hpp"
#include "timer.hpp"
#include "animation.hpp"

class Pacman
{
    public:
        Pacman();
        void draw(sf::RenderTarget &);
        void set_pos(sf::Vector2f);
        sf::Vector2f get_pos() const;
        void set_dir(sf::Vector2f);//velocity is also set in this function
        sf::Vector2f get_dir() const;
        void set_speed(float);
        float get_speed() const;
        void set_velocity(sf::Vector2f);
        sf::FloatRect get_rect() const;
        sf::Vector2f get_velocity() const;
        void set_preDir(sf::Vector2f);
        sf::Vector2f get_preDir() const;
        void move(float);
        void increase_score(int);
        long int get_score() const;
        void set_score(long int);
        void make_energized(bool);
        bool is_energized();
        Timer timer;//energized timer
        sf::Vector2f get_home_pos() const;
        void operator--();
        unsigned int get_no_lives() const;
        void set_no_lives(int);
        void operator++();
        unsigned int get_no_eaten_pellets() const;
        void reset_no_eaten_pellets();
        bool wait(unsigned int, float);
        unsigned int speed_manager(int, int);
        long int get_added_score();
        void change_mode(unsigned int);
        void set_initial_sprite();
        void play_dead_animation(float);
    private:
        long int score = 0;
        long int previous_score = 0;
        sf::Vector2f pos;
        sf::Vector2f dir;
        sf::Vector2f preDir;//previous direction that pacman was following
                            //this is useful for when pacman is stopped to figure out what was the previous direction
        sf::Vector2f velocity;
        sf::RectangleShape Tile;//the tile that pacman is moving on
        float speed;
        float real_speed = 100;
        sf::RectangleShape shape;
        sf::Sprite sprite;
        bool isEnergized = false;
        sf::Vector2f home_pos;
        unsigned int lives = 5;
        unsigned int eaten_pellets = 0;
        float hold_time = 0;
        Animation animations[AnimationIndex::count];
        AnimationIndex cur_animation;
        Animation deadAnimation;
        std::array<std::array<unsigned int, 4>, 4> speed_percentages;
};

#endif // _PACMAN