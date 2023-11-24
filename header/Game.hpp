#ifndef _PACMAN_GAME
#define _PACMAN_GAME

#include "pacman.hpp"
#include "blinky.hpp"
#include "pinky.hpp"
#include "inky.hpp"
#include "clyde.hpp"
#include "ghost.hpp"
#include "map.hpp"
#include "menu.hpp"
#include "chase.hpp"
#include "scatter.hpp"
#include "frightened.hpp"
#include "eaten.hpp"
#include "pellet.hpp"
#include "fruit.hpp"
#include "life.hpp"

class Game
{
    public:
        ~Game();
        void run();
        void set_initial_state();
        void reset();
        //
        bool is_on_new_tile();//for pacman
        bool dir_would_collide(sf::Vector2f);//for pacman
        bool is_exactly_on_tile();//for pacman
        void manage_pac_dir(sf::Vector2f &, bool &);//for pacman
        //
        bool wait_n_frames(unsigned int, float);//assuming that the frame rate is 60
        //pellets
        void set_pellets();
        void draw_pellets(sf::RenderTarget &);
        void eat_pellets();
        //fruits
        void draw_fruits(sf::RenderTarget &);
        void eat_fruits();
        //collision
        bool ghost_pac_collision();//returns true if pacman would die
        //lives
        void set_lives();
        void draw_lives(sf::RenderTarget &);
        //pacman speed manager
        void pac_speed_manager();
        //
        static Game * get_instance();
    private:
        Game();//for singleton stuff
        static Game * game_ptr;
        int level = 1;
        Map map;
        Pacman pacman;
        std::array<Ghost *, 4> ghosts;
        Blinky blinky;
        Chase chase;
        Scatter scatter;
        Frightened frightened;
        Eaten eaten;
        float time = 0;//for ready text
        std::vector<Pellet *> pellets;
        std::vector<Fruit *> fruits;
        bool fruit_added = false;
        std::vector<Life *> lives;
        bool lost = false;
        bool won = false;
        long int high_score = 0;
};

bool mouse_is_on_it(sf::FloatRect, sf::Vector2f);
void magic_tunnel(Pacman &);
void magic_tunnel(Ghost &);
bool Is_on_new_tile(Ghost *, int);
long int calculate_distance(sf::Vector2f, sf::Vector2f);
void decide_direction(Ghost *, Map &, sf::Vector2f);
void s_c_manager(Ghost *, int, float, Chase &, Scatter &);
void turn_around_rule(Ghost *);
bool is_on_initial_tile(sf::FloatRect);
void f_manager(Ghost *, int, float, Frightened &, Eaten &, Chase &, Scatter &);
Mode & scatter_chase_time_table(Chase &, Scatter &, float, int);
void speed_manager(Ghost *, int, Map &);
void change_high_score(std::fstream &, long int);
void read_high_score(std::fstream &, long int &);

#endif // _PACMAN_GAME
