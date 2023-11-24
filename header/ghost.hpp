#ifndef _GHOST
#define _GHOST

#include <SFML/Graphics.hpp>
#include "pacman.hpp"
#include "mode.hpp"
#include "timer.hpp"
#include "animation.hpp"
class Ghost
{
    public:
        Ghost();
        virtual void find_target(const Pacman &) = 0;
        virtual void find_target(const Pacman &, sf::Vector2f) = 0;//this one is for inky :(
        virtual void aim(const Pacman &);//for all ghosts except inky(chase mode)
        virtual void aim(const Pacman &, sf::Vector2f);//for inky's chase mode
        virtual void aim();//for all modes except chase mode
        void set_pos(sf::Vector2f);
        void set_speed(float);
        void set_dir(sf::Vector2f);
        void set_velocity(sf::Vector2f);
        void set_target(sf::Vector2f);
        void draw(sf::RenderTarget &);
        void move(float);
        sf::FloatRect get_rect() const;
        sf::Vector2f get_dir() const;
        float get_speed() const;
        sf::Vector2f get_target() const;
        sf::Vector2f get_pos() const;
        sf::Vector2f get_corner() const;
        void change_mode(Mode *);
        void turn_around_ghost(bool);
        bool turned_around() const;
        void change_color(sf::Color);
        sf::Color get_color() const;
        void make_frightened(bool);
        bool is_frightened() const;
        void make_eaten(bool);
        bool is_eaten() const;
        void make_in(bool);
        bool is_in();
        void change_speed(unsigned int);
        sf::Vector2f get_home_pos();
        void set_initial_sprite();
        // Timer timer;//for managing scatter and chase mode
        // //scatter and chase
        std::vector<std::vector<float>> s_c_table;
        int time_index = 0;
        float s_c_timer = 0;
        //frightened
        std::vector<std::pair<int, int>> f_table;//frightened table
        int f_time_index = 0;
        float f_timer = 0;
        //speeds
        std::vector<std::vector<float>> speeds;
        //
        static int n_ghost_eaten;
        //managing the white and blue stuff when ghosts are frightened
        void manage_white_blue(int);
        std::vector<float> white_blue_time_table;
        int white_blue_index = 0;
    protected:
        sf::Vector2f pos;
        sf::Vector2f dir;
        float speed;
        float real_speed = 100;
        sf::Vector2f velocity;
        sf::Vector2f target;
        sf::RectangleShape Tile;
        sf::Sprite sprite;
        Mode * cur_mode;
        sf::Vector2f corner;
        sf::Color color;//original color of each ghost
        bool turn_around = false;
        bool frightened = false;
        bool eaten = false;
        sf::Vector2f home_pos;
        bool in = true;
        //animation
        Animation animation[AnimationIndex::count];//for chase and scatter
        Animation frightened_anim[AnimationColor::Count];//for frightened mode
        Animation eaten_anim[AnimationIndex::count];//for eaten mode
        AnimationIndex cur_animation;
        AnimationColor cur_anim_color;
        //
};

#endif // _GHOST