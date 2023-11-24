#include "Game.hpp"
#include <cmath>
#include <string>
#include <fstream>

Game * Game::game_ptr = nullptr;

Game * Game::get_instance()
{
    if(game_ptr == nullptr)
    {
        game_ptr = new Game();
    }
    return game_ptr;
}

Game::Game()
{
    ghosts[0] = new Blinky;
    ghosts[1] = new Pinky;
    ghosts[2] = new Inky;
    ghosts[3] = new Clyde;

    set_pellets();
    set_initial_state();
    set_lives();
}

Game::~Game()
{
    for(auto & item: ghosts)
    {
        delete item;
    }
    for(auto & item: pellets)
    {
        delete item;
    }
    for(auto & item: fruits)
    {
        delete item;
    }
    for(auto & item: lives)
    {
        delete item;
    }

    delete game_ptr;
}

bool mouse_is_on_it(sf::FloatRect object, sf::Vector2i mousePos)
{
    if(object.left < mousePos.x && object.top < mousePos.y && object.left + object.width > mousePos.x && object.top + object.height > mousePos.y)
    {
        return true;
    }
    return false;
}

void change_high_score(std::fstream & fs, long int newScore)
{
    if(!fs.is_open())
    {
        throw std::logic_error("File is not open!\n");
    }

    fs.clear();
    fs.seekp(0, std::ios::beg);

    fs.write(reinterpret_cast<const char *>(&newScore), sizeof(newScore));
}

void read_high_score(std::fstream & fs, long int & theScore)
{
    if(!fs.is_open())
    {
        throw std::logic_error("File is not open!\n");
    }

    fs.clear();
    fs.seekg(0, std::ios::beg);

    fs.read(reinterpret_cast<char *>(&theScore), sizeof(theScore));
}

void Game::run()
{
    sf::RenderWindow window(sf::VideoMode(560, 720), "Pacman Game", sf::Style::Titlebar | sf::Style::Close);
    Menu setting_menu(window.getSize().x, window.getSize().y, sf::Vector2f(100, 200), 100, std::vector<std::string>({"Reset", "Clear High Score", "Exit"}),
    sf::Color::Green);

    window.setPosition(sf::Vector2i(680, 180));

    std::fstream fs("../data.dat", std::ios::out | std::ios::binary | std::ios::in);
    if(!fs)
    {
        throw std::logic_error("Couldn't open the file!\n");
    }

    read_high_score(fs, high_score);

    sf::RectangleShape pause_pic;
    pause_pic.setSize(sf::Vector2f(58, 58));
    pause_pic.setPosition(500, 0);
    sf::Texture pause;
    if(!pause.loadFromFile("../images/pause.png"))
    {
        throw std::logic_error("Couldn't load the pause image!\n");
    }
    pause_pic.setTexture(&pause);
    sf::Font setting_font;
    if(!setting_font.loadFromFile("../Fonts/FONT.ttf"))
    {
        throw std::logic_error("Couldn't load the font!\n");
    }
    sf::Text setting_text;
    setting_text.setFont(setting_font);
    setting_text.setCharacterSize(24);
    setting_text.setFillColor(sf::Color::White);
    setting_text.setPosition(25, 670);
    setting_text.setString("Press Esc to get back to the game!");

    //Ready text
    sf::Text ready_t;
    ready_t.setFont(setting_font);
    ready_t.setCharacterSize(40);
    ready_t.setFillColor(sf::Color::Yellow);
    ready_t.setPosition(12 * 20, 19 * 20);
    ready_t.setString("READY!");
    //score text
    sf::Text score;
    score.setFont(setting_font);
    score.setCharacterSize(24);
    score.setFillColor(sf::Color::Green);
    score.setPosition(20, 20);
    //level text
    sf::Text level_txt;
    level_txt.setFont(setting_font);
    level_txt.setCharacterSize(25);
    level_txt.setFillColor(sf::Color::White);
    level_txt.setPosition(20 * 20, 34 * 20);
    //new score_text
    sf::Text new_score;
    new_score.setFont(setting_font);
    new_score.setCharacterSize(25);
    new_score.setFillColor(sf::Color::Magenta);
    new_score.setPosition(200, 20);
    //high score text
    sf::Text high_score_text;
    high_score_text.setFont(setting_font);
    high_score_text.setCharacterSize(25);
    high_score_text.setFillColor(sf::Color::Red);
    high_score_text.setPosition(280, 20);
    //new level text
    sf::Text new_level_text;
    new_level_text.setFont(setting_font);
    new_level_text.setCharacterSize(40);
    new_level_text.setFillColor(sf::Color::Magenta);
    new_level_text.setPosition(11 * 20, 19 * 20);
    new_level_text.setString("NEW LEVEL!");

    sf::Vector2i mouse_pos;
    bool paused = false;
    bool ready = true;
    bool stop = false;
    bool dead = false;
    bool done = false;
    bool new_level = false;
    int length = 20;
    bool was_reset = false;
    sf::Vector2f initial_target {260, 280};

    //pacman
    sf::Vector2f dir {-1, 0};
    pacman.set_dir(dir);
    bool stuck = false;

    //setting initial mode for the ghosts
    for(auto & item: ghosts)
    {
        item -> change_mode(&scatter);
    }
    //
    sf::Clock clock;
    sf::Time dt;

    while(window.isOpen())
    {
        dt = clock.restart();
        sf::Event event;

        high_score_text.setString("High Score: " + std::to_string(high_score));

        mouse_pos = sf::Mouse::getPosition(window);

        if(stop)
        {
            if(dead)
            {
                pacman.play_dead_animation(dt.asSeconds());
                if(!wait_n_frames(60, dt.asSeconds()))
                {
                    stop = false;
                    dead = false;
                    done = true;
                }
            }
            else if(new_level)
            {
                if(!wait_n_frames(120, dt.asSeconds()))
                {
                    stop = false;
                    new_level = false;
                    done = true;
                }
            }
        }
        if(ready && !stop)
        {
            paused = false;
            if(!wait_n_frames(180, dt.asSeconds()))//equal to 3 seconds
            {
                ready = false;
            }
        }
        //handle input
        while(window.pollEvent(event))
        {
            if(paused && !stop)
            {
                if(event.type == sf::Event::KeyReleased)
                {
                    if(event.key.code == sf::Keyboard::Up)
                    {
                        setting_menu.move_up();
                        break;
                    }
                    if(event.key.code == sf::Keyboard::Down)
                    {
                        setting_menu.move_down();
                        break;
                    }
                    if(event.key.code == sf::Keyboard::Return)
                    {
                        int selected = setting_menu.main_menu_pressed();

                        if(selected == 0)
                        {
                            level = 1;
                            reset();
                            ready = true;
                            for(auto & item: lives)
                            {
                                delete item;
                            }
                            lives.clear();
                            pacman.set_no_lives(5);
                            pacman.set_score(0);
                            set_lives();
                            break;
                        }
                        else if(selected == 1)
                        {
                            change_high_score(fs, 0);
                            was_reset = true;
                            ready = true;
                            break;
                        }
                        else if(selected == 2)
                        {
                            window.close();
                        }
                    }
                    if(event.key.code == sf::Keyboard::Escape)
                    {
                        ready = true;
                    }
                }
            }
            else if(!paused && !ready && !stop)
            {
                //game
                if(event.type == sf::Event::Closed)
                {
                    window.close();
                    break;
                }
                if(event.type == sf::Event::KeyReleased)
                {
                    if(event.key.code == sf::Keyboard::Escape)
                    {
                        window.close();
                        break;
                    }
                }
                if(event.type == sf::Event::MouseButtonReleased)
                {
                    if(event.mouseButton.button == sf::Mouse::Left)
                    {
                        if(mouse_is_on_it(pause_pic.getGlobalBounds(), mouse_pos))
                        {
                            paused = true;
                            break;
                        }
                    }
                }
                if(event.type == sf::Event::KeyPressed)
                {
                    if(event.key.code == sf::Keyboard::Up)
                    {
                        dir.x = 0;
                        dir.y = -1;
                        break;
                    }
                    if(event.key.code == sf::Keyboard::Left)
                    {
                        dir.x = -1;
                        dir.y = 0;
                        break;
                    }
                    if(event.key.code == sf::Keyboard::Down)
                    {
                        dir.x = 0;
                        dir.y = 1;
                        break;
                    }
                    if(event.key.code == sf::Keyboard::Right)
                    {
                        dir.x = 1;
                        dir.y = 0;
                        break;
                    }
                }
            }
        }
        //update
        if(was_reset)
        {
            read_high_score(fs, high_score);
            was_reset = false;
        }

        if(paused && !stop)
        {
            
        }
        else if(!paused && !ready && !stop)
        {
            //pacman
            if(is_on_new_tile() || is_exactly_on_tile())
            {
                pac_speed_manager();
                magic_tunnel(pacman);
                manage_pac_dir(dir, stuck);
            }
            pacman.set_velocity(pacman.get_dir() * pacman.get_speed());
            pacman.move(dt.asSeconds());
            eat_pellets();
            score.setString("Score: " + std::to_string(pacman.get_score()));
            //showing the added score!
            if(pacman.get_added_score() != 0)
            {
                if(pacman.get_added_score() < 0)
                {
                    new_score.setString(std::to_string(pacman.get_added_score()));
                }
                else if(pacman.get_added_score() > 0)
                {
                    new_score.setString("+" + std::to_string(pacman.get_added_score()));
                }
            }
            if(ghost_pac_collision())
            {
                if(!dead && !done && !stop)
                {
                    stop = true;
                    dead = true;
                    continue;
                }
                if(!stop && done && !dead)
                {
                    pacman.increase_score(-20);
                    set_initial_state();
                    ready = true;
                    if(pacman.get_no_lives() != 1)
                    {
                        delete lives[lives.size() - 1];
                        lives.erase(lives.end() - 1);
                    }
                    --pacman;
                    if(pacman.get_no_lives() == 0)
                    {
                        lost = true;
                        break;
                    }
                    done = false;
                    continue;
                }
            }
            if(pacman.get_no_eaten_pellets() == 244)
            {
                if(!stop && !new_level && !done)
                {
                    stop = true;
                    new_level = true;
                    continue;
                }
                if(!stop && done && !new_level)
                {
                    ++level;
                    if(level == 256)
                    {
                        won = true;
                        break;
                    }
                    reset();
                    ready = true;
                    done = false;
                    continue;
                }
            }
            level_txt.setString("Level: " + std::to_string(level) + " / 256");
            if((pacman.get_no_eaten_pellets() == 70 || pacman.get_no_eaten_pellets() == 170) && !fruit_added)
            {
                fruit_added = true;
                fruits.push_back(new Fruit(level));
            }
            for(int index = 0; index < fruits.size(); ++index)
            {
                fruits[index] -> manage_fruit(dt.asSeconds(), fruit_added);
                if(fruits[index] -> is_time_up())
                {
                    delete fruits[index];
                    fruits.erase(fruits.begin() + index);
                    break;
                }
            }
            eat_fruits();

            //ghosts
            for(int index = 0; index < ghosts.size(); ++index)
            {
                s_c_manager(ghosts[index], level, dt.asSeconds(), chase, scatter);
                f_manager(ghosts[index], level, dt.asSeconds(), frightened, eaten, chase, scatter);
                ghosts[index] -> manage_white_blue(level);
                if(Is_on_new_tile(ghosts[index], length))
                {
                    speed_manager(ghosts[index], level, map);
                    if(is_on_initial_tile(ghosts[index] -> get_rect()) && ghosts[index] -> is_eaten())
                    {
                        ghosts[index] -> set_pos(sf::Vector2f(14 * 20, 17 * 20));
                        ghosts[index] -> make_eaten(false);
                        ghosts[index] -> make_in(true);
                        ghosts[index] -> change_mode(&scatter_chase_time_table(chase, scatter, ghosts[index] -> s_c_timer, level));
                    }
                    if(is_on_initial_tile(ghosts[index] -> get_rect()) && ghosts[index] -> is_in())
                    {
                        ghosts[index] -> make_in(false);
                    }
                    if(ghosts[index] -> is_in())
                    {
                        ghosts[index] -> set_target(initial_target);
                    }
                    else
                    {
                        //real deal
                        magic_tunnel(*(ghosts[index]));
                        ghosts[index] -> aim();
                        ghosts[index] -> aim(pacman);
                        ghosts[index] -> aim(pacman, ghosts[index] -> get_pos());
                    }
                    decide_direction(ghosts[index], map, ghosts[index] -> get_target());
                    
                }
                ghosts[index] -> set_velocity(ghosts[index] -> get_speed() * ghosts[index] -> get_dir());
                ghosts[index] -> move(dt.asSeconds());
            }
        }

        //display
        window.clear();

        if(paused)
        {
            setting_menu.draw(window);
            window.draw(setting_text);
        }
        else if(ready)
        {
            //when the game is displayed
            window.draw(ready_t);
            map.draw(window);
            draw_pellets(window);
            window.draw(pause_pic);
            pacman.draw(window);
            for(const auto & item: ghosts)
            {
                item -> draw(window);
            }
        }
        else if(stop)
        {
            if(new_level)
            {
                window.draw(new_level_text);
            }
            map.draw(window);
            pacman.draw(window);     
        }
        else
        {
            //when the game is displayed
            map.draw(window);
            draw_pellets(window);
            window.draw(pause_pic);
            draw_fruits(window);
            pacman.draw(window);
            for(const auto & item: ghosts)
            {
                item -> draw(window);
            }
            window.draw(score);
            window.draw(level_txt);
            window.draw(new_score);
            draw_lives(window);
            window.draw(high_score_text);
        }

        window.display();
    }
    sf::Text end_text;
    end_text.setFillColor(sf::Color::Red);
    end_text.setCharacterSize(25);
    end_text.setFont(setting_font);
    end_text.setPosition(225, 350);

    if(lost)
    {
        Timer counter;
        end_text.setString("Game Over!");
        if(pacman.get_score() > high_score)
        {
            change_high_score(fs, pacman.get_score());
        }
        while(counter.get_elapsed_time() < 3)
        {
            window.clear();
            window.draw(end_text);
            window.display();
        }
    }
    if(won)
    {
        Timer counter;
        end_text.setFillColor(sf::Color::White);
        end_text.setString("YOU WON!");
        if(pacman.get_score() > high_score)
        {
            change_high_score(fs, pacman.get_score());
        }
        while(counter.get_elapsed_time() < 3)
        {
            window.clear();
            window.draw(end_text);
            window.display();
        }
    }
    fs.close();
}

bool Game::is_on_new_tile()
{
    //pacman moving right
    int length = map.get_grid_length();
    sf::FloatRect rect = pacman.get_rect();
    if(pacman.get_dir().x == 1 && pacman.get_dir().y == 0)
    {
        if(((int)rect.left / length * length + length) - rect.left < 1.68
        && ((int)rect.left / length * length + length) - rect.left > 0)
        {
            pacman.set_pos(sf::Vector2f((int)(rect.left) / length * length + length, rect.top));
            return true;
        }
    }//moving left
    else if(pacman.get_dir().x == -1 && pacman.get_dir().y == 0)
    {
        if(rect.left - ((int)rect.left / length * length) < 1.68
        && rect.left - ((int)rect.left / length * length) > 0)
        {
            pacman.set_pos(sf::Vector2f(((int)rect.left / length * length), rect.top));
            return true;
        }
        if(rect.left - ((int)rect.left / length * length - length) < 1.68
        && rect.left - ((int)rect.left / length * length - length) > 0)
        {
            pacman.set_pos(sf::Vector2f(((int)rect.left / length * length - length), rect.top));
            return true;  
        }
    }//moving bottom
    else if(pacman.get_dir().y == 1 && pacman.get_dir().x == 0)
    {
        if(((int)rect.top / length * length + length) - rect.top < 1.68
        && ((int)rect.top / length * length + length) - rect.top > 0)
        {
            pacman.set_pos(sf::Vector2f(rect.left, ((int)(rect.top) / length * length + length)));
            return true;
        }
    }//moving top
    else if(pacman.get_dir().y == -1 && pacman.get_dir().x == 0)
    {
        if(rect.top - ((int)rect.top / length * length) < 1.68
        && rect.top - ((int)rect.top / length * length) > 0)
        {
            pacman.set_pos(sf::Vector2f(rect.left, ((int)rect.top) / length * length));
            return true;
        }
    }
    return false;
}

bool Game::dir_would_collide(sf::Vector2f theDir)
{
    sf::Vector2f pacPos = pacman.get_pos();
    int length = map.get_grid_length();
    if(theDir.x == 1 && theDir.y == 0)
    {
        if(map.get_tile(sf::Vector2f(pacPos.x + length, pacPos.y)).it_is_wall())
        {
            // map.get_tile(sf::Vector2f(pacPos.x + length, pacPos.y)).shape.setFillColor(sf::Color::Red);
            return true;
        }
    }
    else if(theDir.x == -1 && theDir.y == 0)
    {
        if(map.get_tile(sf::Vector2f(pacPos.x - length, pacPos.y)).it_is_wall())
        {
            // map.get_tile(sf::Vector2f(pacPos.x - length, pacPos.y)).shape.setFillColor(sf::Color::Red);
            return true;
        }
    }
    else if(theDir.y == 1 && theDir.x == 0)
    {
        if(map.get_tile(sf::Vector2f(pacPos.x, pacPos.y + length)).it_is_wall())
        {
            // map.get_tile(sf::Vector2f(pacPos.x, pacPos.y + length)).shape.setFillColor(sf::Color::Red);
            return true;
        }
    }
    else if(theDir.y == -1 && theDir.x == 0)
    {
        if(map.get_tile(sf::Vector2f(pacPos.x, pacPos.y - length)).it_is_wall())
        {
            // map.get_tile(sf::Vector2f(pacPos.x, pacPos.y - length)).shape.setFillColor(sf::Color::Red);
            return true;
        }
    }
    return false;
}

bool Game::is_exactly_on_tile()
{
    int length = map.get_grid_length();
    sf::FloatRect pacBounds = pacman.get_rect();
    sf::FloatRect tileBounds = map.get_tile(sf::Vector2f(pacman.get_pos())).shape.getGlobalBounds();

    if(pacBounds.left == tileBounds.left && pacBounds.top == tileBounds.top)
    {
        return true;
    }
    return false;
}

void Game::manage_pac_dir(sf::Vector2f & dir, bool & stuck)
{
    if(stuck && (dir.x != 0 || dir.y != 0))
    {
        pacman.set_dir(dir);
        stuck = false;
    }
    if((pacman.get_dir().x == 1 || pacman.get_dir().x == -1) && pacman.get_dir().y == 0 && (dir.x == 1 || dir.x == -1) && dir.y == 0)
    {
        if(dir_would_collide(dir))
        {
            pacman.set_preDir(dir);
            dir.x = 0;
            dir.y = 0;
            stuck = true;
        }
        pacman.set_dir(dir);
    }
    else if((pacman.get_dir().y == 1 || pacman.get_dir().y == -1) && pacman.get_dir().x == 0 && (dir.y == 1 || dir.y == -1) && dir.x == 0)
    {
        if(dir_would_collide(dir))
        {
            pacman.set_preDir(dir);
            dir.x = 0;
            dir.y = 0;
            stuck = true;
        }
        pacman.set_dir(dir);
    }
    else if((pacman.get_dir().x == 1 || pacman.get_dir().x == -1) && pacman.get_dir().y == 0 && (dir.y == 1 || dir.y == -1) && dir.x == 0)
    {
        if(!dir_would_collide(dir))
        {
            pacman.set_dir(dir);
        }
        else
        {
            if(dir_would_collide(pacman.get_dir()))
            {
                pacman.set_preDir(dir);
                dir.x = 0;
                dir.y = 0;
                stuck = true;
                pacman.set_dir(dir);
            }
        }
    }
    else if((pacman.get_dir().y == 1 || pacman.get_dir().y == -1) && pacman.get_dir().x == 0 && (dir.x == 1 || dir.x == -1) && dir.y == 0)
    {
        if(!dir_would_collide(dir))
        {
            pacman.set_dir(dir);
        }
        else
        {
            if(dir_would_collide(pacman.get_dir()))
            {
                pacman.set_preDir(dir);
                dir.x = 0;
                dir.y = 0;
                stuck = true;
                pacman.set_dir(dir);
            }
        }
    }
}

void magic_tunnel(Pacman & pac)
{
    int gridLength = 20;
    if(pac.get_rect().left == 28 * gridLength && pac.get_rect().top == 17 * gridLength)
    {
        pac.set_pos(sf::Vector2f((-1) * gridLength, 17 * gridLength));
    }
    else if(pac.get_rect().left == (-1) * gridLength && pac.get_rect().top == 17 * gridLength)
    {
        pac.set_pos(sf::Vector2f(28 * gridLength, 17 * gridLength));
    }
}

void magic_tunnel(Ghost & ghost)
{
    int gridLength = 20;
    if(ghost.get_rect().left == 28 * gridLength && ghost.get_rect().top == 17 * gridLength)
    {
        ghost.set_pos(sf::Vector2f((-1) * gridLength, 17 * gridLength));
    }
    else if(ghost.get_rect().left == (-1) * gridLength && ghost.get_rect().top == 17 * gridLength)
    {
        ghost.set_pos(sf::Vector2f(28 * gridLength, 17 * gridLength));
    }
}

bool Game::wait_n_frames(unsigned int n_frames, float dt)
{
    time += dt;
    if(n_frames * 0.0169 > time)
    {
        return true;
    }

    time = 0;
    return false;
}
bool Is_on_new_tile(Ghost* ghost, int length)
{
    sf::FloatRect rect = ghost -> get_rect();
    if(ghost -> get_dir().x == 1 && ghost -> get_dir().y == 0)
    {
        if(((int)rect.left / length * length + length) - rect.left < 1.68
        && ((int)rect.left / length * length + length) - rect.left > 0)
        {
            ghost -> set_pos(sf::Vector2f((int)(rect.left) / length * length + length, rect.top));
            return true;
        }
    }//moving left
    else if(ghost -> get_dir().x == -1 && ghost -> get_dir().y == 0)
    {
        if(rect.left - ((int)rect.left / length * length) < 1.68
        && rect.left - ((int)rect.left / length * length) > 0)
        {
            ghost -> set_pos(sf::Vector2f(((int)rect.left / length * length), rect.top));
            return true;
        }
        if(rect.left - ((int)rect.left / length * length - length) < 1.68
        && rect.left - ((int)rect.left / length * length - length) > 0)
        {
            ghost -> set_pos(sf::Vector2f(((int)rect.left / length * length - length), rect.top));
            return true;  
        }
    }//moving bottom
    else if(ghost -> get_dir().y == 1 && ghost -> get_dir().x == 0)
    {
        if(((int)rect.top / length * length + length) - rect.top < 1.68
        && ((int)rect.top / length * length + length) - rect.top > 0)
        {
            ghost -> set_pos(sf::Vector2f(rect.left, ((int)(rect.top) / length * length + length)));
            return true;
        }
    }//moving top
    else if(ghost -> get_dir().y == -1 && ghost -> get_dir().x == 0)
    {
        if(rect.top - ((int)rect.top / length * length) < 1.68
        && rect.top - ((int)rect.top / length * length) > 0)
        {
            ghost -> set_pos(sf::Vector2f(rect.left, ((int)rect.top) / length * length));
            return true;
        }
    }
    return false;
}

long int calculate_distance(sf::Vector2f first, sf::Vector2f second)
{
    return sqrt((first.x - second.x) * (first.x - second.x) + (first.y - second.y) * (first.y - second.y));
}

void decide_direction(Ghost * ghost, Map & map, sf::Vector2f target)
{
    int length = map.get_grid_length();
    sf::Vector2f ghostDir = ghost -> get_dir();
    sf::FloatRect ghostBounds = ghost -> get_rect();
    std::multimap<long int, sf::Vector2f> mulMap;
    bool must_turn_around = ghost -> turned_around();

    long int distance;

    //if two tiles have the same distance from the target tile
    //we act based on these priorities:UP, LEFT, DOWN, RIGHT
    //that is why we start from the top

    //starting from the top
    if(!map.get_tile(sf::Vector2f(ghostBounds.left, ghostBounds.top - length)).it_is_wall()
    && sf::Vector2f(0, -1) != -ghostDir && !map.get_tile(sf::Vector2f(ghostBounds.left, ghostBounds.top)).it_is_red()
    || (map.get_tile(sf::Vector2f(ghostBounds.left, ghostBounds.top)).it_is_red() && ghost -> is_eaten())
    || (ghost -> is_in() && map.get_tile(sf::Vector2f(ghostBounds.left, ghostBounds.top - length)).it_is_door()))
    {
        distance = calculate_distance(sf::Vector2f(ghostBounds.left, ghostBounds.top - length), sf::Vector2f(target.x, target.y));
        mulMap.insert(std::make_pair(distance, sf::Vector2f(0, -1)));
    }//now left
    if(!map.get_tile(sf::Vector2f(ghostBounds.left - length, ghostBounds.top)).it_is_wall()
    && sf::Vector2f(-1, 0) != -ghostDir)
    {
        distance = calculate_distance(sf::Vector2f(ghostBounds.left - length, ghostBounds.top), sf::Vector2f(target.x, target.y));
        mulMap.insert(std::make_pair(distance, sf::Vector2f(-1, 0)));
    }//now down
    if(!map.get_tile(sf::Vector2f(ghostBounds.left, ghostBounds.top + length)).it_is_wall()
    && sf::Vector2f(0, 1) != -ghostDir)
    {
        distance = calculate_distance(sf::Vector2f(ghostBounds.left, ghostBounds.top + length), sf::Vector2f(target.x, target.y));
        mulMap.insert(std::make_pair(distance, sf::Vector2f(0, 1)));
    }//now right
    if(!map.get_tile(sf::Vector2f(ghostBounds.left + length, ghostBounds.top)).it_is_wall()
    && sf::Vector2f(1, 0) != -ghostDir)
    {
        distance = calculate_distance(sf::Vector2f(ghostBounds.left + length, ghostBounds.top), sf::Vector2f(target.x, target.y));
        mulMap.insert(std::make_pair(distance, sf::Vector2f(1, 0)));
    }

    if(must_turn_around)//turn around rule
    {
        if(!map.get_tile(-ghostDir).it_is_wall())//it never going to happen but in a special case it is going to happen and that special case is when they go to the magic tunnel
        {
            mulMap.insert(std::make_pair(0, -ghostDir));
            ghost -> turn_around_ghost(false);
        }
    }

    ghost -> set_dir(mulMap.cbegin() -> second);
}
void s_c_manager(Ghost * ghost, int level, float dt, Chase & chase, Scatter & scatter)
{
    if(!ghost -> is_frightened() && !ghost -> is_eaten() && !ghost -> is_in())
    {
        ghost -> s_c_timer += dt;
        if(level == 1)
        {
            if(ghost -> time_index != ghost -> s_c_table[0].size())
            {
                if(ghost -> s_c_timer > ghost -> s_c_table[0][ghost -> time_index])
                {
                    if(ghost -> time_index % 2 == 0)
                    {
                        ghost -> change_mode(&chase);
                        turn_around_rule(ghost);
                        ++(ghost -> time_index);
                    }
                    else if(ghost -> time_index % 2 == 1)
                    {
                        ghost -> change_mode(&scatter);
                        turn_around_rule(ghost);
                        ++(ghost -> time_index);
                    }
                }
            }
        }
        else if(level >= 2 && level <= 4)
        {
            if(ghost -> time_index != ghost -> s_c_table[1].size())
            {
                if(ghost -> s_c_timer > ghost -> s_c_table[1][ghost -> time_index])
                {
                    if(ghost -> time_index % 2 == 0)
                    {
                        ghost -> change_mode(&chase);
                        turn_around_rule(ghost);
                        ++(ghost -> time_index);
                    }
                    else if(ghost -> time_index % 2 == 1)
                    {
                        ghost -> change_mode(&scatter);
                        turn_around_rule(ghost);
                        ++(ghost -> time_index);
                    }
                }
            }
        }
        else if(level >= 5)
        {
            if(ghost -> time_index != ghost -> s_c_table[2].size())
            {
                if(ghost -> s_c_timer > ghost -> s_c_table[2][ghost -> time_index])
                {
                    if(ghost -> time_index % 2 == 0)
                    {
                        ghost -> change_mode(&chase);
                        turn_around_rule(ghost);
                        ++(ghost -> time_index);
                    }
                    else if(ghost -> time_index % 2 == 1)
                    {
                        ghost -> change_mode(&scatter);
                        turn_around_rule(ghost);
                        ++(ghost -> time_index);
                    }
                }
            }
        }
    }
}

void f_manager(Ghost * ghost, int level, float dt, Frightened & Frightened, Eaten & eaten, Chase & chase, Scatter & scatter)
{
    if(ghost -> is_frightened())
    {
        if(ghost -> is_eaten())
        {
            ghost -> change_mode(&eaten);
            ghost -> make_eaten(true);
            ghost -> make_frightened(false);
            ghost -> f_timer = 0;
            if(ghost -> n_ghost_eaten == 4)
            {
                ghost -> n_ghost_eaten = 0;
            }
            ghost -> white_blue_time_table.clear();
            ghost -> white_blue_index = 0;
        }
        else if(!ghost -> is_eaten())
        {
            ghost -> f_timer += dt;

            if(level == 1)
            {
                if(ghost -> f_timer > ghost -> f_table[0].first)
                {
                    ghost -> make_frightened(false);
                    ghost -> change_mode(&scatter_chase_time_table(chase, scatter, ghost -> s_c_timer, level));
                    ghost -> f_timer = 0;
                    ghost -> n_ghost_eaten = 0;
                    ghost -> white_blue_time_table.clear();
                    ghost -> white_blue_index = 0;
                }
            }
            else if(level == 2)
            {
                if(ghost -> f_timer > ghost -> f_table[1].first)
                {
                    ghost -> make_frightened(false);
                    ghost -> change_mode(&scatter_chase_time_table(chase, scatter, ghost -> s_c_timer, level));
                    ghost -> f_timer = 0;
                }
            }
            else if(level == 3)
            {
                if(ghost -> f_timer > ghost -> f_table[2].first)
                {
                    ghost -> make_frightened(false);
                    ghost -> change_mode(&scatter_chase_time_table(chase, scatter, ghost -> s_c_timer, level));
                    ghost -> f_timer = 0;
                }
            }
            else if(level == 4)
            {
                if(ghost -> f_timer > ghost -> f_table[3].first)
                {
                    ghost -> make_frightened(false);
                    ghost -> change_mode(&scatter_chase_time_table(chase, scatter, ghost -> s_c_timer, level));
                    ghost -> f_timer = 0;
                }
            }
            else if(level == 5)
            {
                if(ghost -> f_timer > ghost -> f_table[4].first)
                {
                    ghost -> make_frightened(false);
                    ghost -> change_mode(&scatter_chase_time_table(chase, scatter, ghost -> s_c_timer, level));
                    ghost -> f_timer = 0;
                }
            }
            else if(level >= 6 && level <= 10)
            {
                if(ghost -> f_timer > ghost -> f_table[5].first)
                {
                    ghost -> make_frightened(false);
                    ghost -> change_mode(&scatter_chase_time_table(chase, scatter, ghost -> s_c_timer, level));
                    ghost -> f_timer = 0;
                }
            }
            else if(level >= 11 && level <= 16)
            {
                if(ghost -> f_timer > ghost -> f_table[6].first)
                {
                    ghost -> make_frightened(false);
                    ghost -> change_mode(&scatter_chase_time_table(chase, scatter, ghost -> s_c_timer, level));
                    ghost -> f_timer = 0;
                }
            }
            else if(level >= 17 && level <= 32)
            {
                if(ghost -> f_timer > ghost -> f_table[7].first)
                {
                    ghost -> make_frightened(false);
                    ghost -> change_mode(&scatter_chase_time_table(chase, scatter, ghost -> s_c_timer, level));
                    ghost -> f_timer = 0;
                }
            }
            else if(level >= 33 && level <= 64)
            {
                if(ghost -> f_timer > ghost -> f_table[8].first)
                {
                    ghost -> make_frightened(false);
                    ghost -> change_mode(&scatter_chase_time_table(chase, scatter, ghost -> s_c_timer, level));
                    ghost -> f_timer = 0;
                }
            }
            else if(level >= 65 && level <= 128)
            {
                if(ghost -> f_timer > ghost -> f_table[9].first)
                {
                    ghost -> make_frightened(false);
                    ghost -> change_mode(&scatter_chase_time_table(chase, scatter, ghost -> s_c_timer, level));
                    ghost -> f_timer = 0;
                }
            }
            else if(level > 128)
            {
                if(ghost -> f_timer > ghost -> f_table[10].first)
                {
                    ghost -> make_frightened(false);
                    ghost -> change_mode(&scatter_chase_time_table(chase, scatter, ghost -> s_c_timer, level));
                    ghost -> f_timer = 0;
                }
            }
        }
    }
}
void turn_around_rule(Ghost * ghost)
{
    ghost -> turn_around_ghost(true);
}

bool is_on_initial_tile(sf::FloatRect bounds)
{
    if(bounds.left == 260 && bounds.top == 280)
    {
        return true;
    }
    return false;
}

void Game::set_pellets()
{
    int length = 20;
    pellets.push_back(new Pellet(false, sf::Vector2f(1 * length + 6, 4 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(1 * length + 6, 5 * length + 6)));
    pellets.push_back(new Pellet(true, sf::Vector2f(1 * length + 2, 6 * length + 2)));
    pellets.push_back(new Pellet(false, sf::Vector2f(1 * length + 6, 7 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(1 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(1 * length + 6, 9 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(1 * length + 6, 10 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(1 * length + 6, 11 * length + 6)));
    //
    pellets.push_back(new Pellet(false, sf::Vector2f(26 * length + 6, 4 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(26 * length + 6, 5 * length + 6)));
    pellets.push_back(new Pellet(true, sf::Vector2f(26 * length + 2, 6 * length + 2)));
    pellets.push_back(new Pellet(false, sf::Vector2f(26 * length + 6, 7 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(26 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(26 * length + 6, 9 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(26 * length + 6, 10 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(26 * length + 6, 11 * length + 6)));
    //
    pellets.push_back(new Pellet(false, sf::Vector2f(2 * length + 6, 4 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(3 * length + 6, 4 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(4 * length + 6, 4 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(5 * length + 6, 4 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 4 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(7 * length + 6, 4 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(8 * length + 6, 4 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(9 * length + 6, 4 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(10 * length + 6, 4 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(11 * length + 6, 4 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(12 * length + 6, 4 * length + 6)));
    //
    pellets.push_back(new Pellet(false, sf::Vector2f(15 * length + 6, 4 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(16 * length + 6, 4 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(17 * length + 6, 4 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(18 * length + 6, 4 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(19 * length + 6, 4 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(20 * length + 6, 4 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 4 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(22 * length + 6, 4 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(23 * length + 6, 4 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(24 * length + 6, 4 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(25 * length + 6, 4 * length + 6)));
    //
    pellets.push_back(new Pellet(false, sf::Vector2f(2 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(3 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(4 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(5 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(7 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(8 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(9 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(10 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(11 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(12 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(13 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(14 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(15 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(16 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(17 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(18 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(19 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(20 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(22 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(23 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(24 * length + 6, 8 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(25 * length + 6, 8 * length + 6)));
    //
    pellets.push_back(new Pellet(false, sf::Vector2f(12 * length + 6, 5 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(12 * length + 6, 6 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(12 * length + 6, 7 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(15 * length + 6, 5 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(15 * length + 6, 6 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(15 * length + 6, 7 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 5 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 6 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 7 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 5 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 6 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 7 * length + 6)));
    //
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 9 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 10 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 11 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 12 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 13 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 14 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 15 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 16 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 17 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 18 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 19 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 20 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 21 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 22 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 23 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 24 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 25 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 26 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 27 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 28 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 29 * length + 6)));
    //
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 9 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 10 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 11 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 12 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 13 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 14 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 15 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 16 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 17 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 18 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 19 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 20 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 21 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 22 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 23 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 24 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 25 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 26 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 27 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 28 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 29 * length + 6)));
    //
    pellets.push_back(new Pellet(false, sf::Vector2f(1 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(2 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(3 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(4 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(5 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(6 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(7 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(8 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(9 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(10 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(11 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(12 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(13 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(14 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(15 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(16 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(17 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(18 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(19 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(20 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(21 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(22 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(23 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(24 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(25 * length + 6, 32 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(26 * length + 6, 32 * length + 6)));
    //
    pellets.push_back(new Pellet(false, sf::Vector2f(2 * length + 6, 11 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(3 * length + 6, 11 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(4 * length + 6, 11 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(5 * length + 6, 11 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(22 * length + 6, 11 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(23 * length + 6, 11 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(24 * length + 6, 11 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(25 * length + 6, 11 * length + 6)));
    //
    pellets.push_back(new Pellet(false, sf::Vector2f(9 * length + 6, 9 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(9 * length + 6, 10 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(9 * length + 6, 11 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(18 * length + 6, 9 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(18 * length + 6, 10 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(18 * length + 6, 11 * length + 6)));
    //
    pellets.push_back(new Pellet(false, sf::Vector2f(10 * length + 6, 11 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(11 * length + 6, 11 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(12 * length + 6, 11 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(15 * length + 6, 11 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(16 * length + 6, 11 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(17 * length + 6, 11 * length + 6)));
    //
    pellets.push_back(new Pellet(false, sf::Vector2f(1 * length + 6, 23 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(2 * length + 6, 23 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(3 * length + 6, 23 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(4 * length + 6, 23 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(5 * length + 6, 23 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(22 * length + 6, 23 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(23 * length + 6, 23 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(24 * length + 6, 23 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(25 * length + 6, 23 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(26 * length + 6, 23 * length + 6)));
    //
    pellets.push_back(new Pellet(false, sf::Vector2f(7 * length + 6, 23 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(8 * length + 6, 23 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(9 * length + 6, 23 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(10 * length + 6, 23 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(11 * length + 6, 23 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(12 * length + 6, 23 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(15 * length + 6, 23 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(16 * length + 6, 23 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(17 * length + 6, 23 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(18 * length + 6, 23 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(19 * length + 6, 23 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(20 * length + 6, 23 * length + 6)));
    //
    pellets.push_back(new Pellet(false, sf::Vector2f(1 * length + 6, 24 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(1 * length + 6, 25 * length + 6)));
    pellets.push_back(new Pellet(true, sf::Vector2f(1 * length + 2, 26 * length + 2)));
    pellets.push_back(new Pellet(false, sf::Vector2f(12 * length + 6, 24 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(12 * length + 6, 25 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(12 * length + 6, 26 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(15 * length + 6, 24 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(15 * length + 6, 25 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(15 * length + 6, 26 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(26 * length + 6, 24 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(26 * length + 6, 25 * length + 6)));
    pellets.push_back(new Pellet(true, sf::Vector2f(26 * length + 2, 26 * length + 2)));
    //
    pellets.push_back(new Pellet(false, sf::Vector2f(2 * length + 6, 26 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(3 * length + 6, 26 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(24 * length + 6, 26 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(25 * length + 6, 26 * length + 6)));
    //
    pellets.push_back(new Pellet(false, sf::Vector2f(7 * length + 6, 26 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(8 * length + 6, 26 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(9 * length + 6, 26 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(10 * length + 6, 26 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(11 * length + 6, 26 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(16 * length + 6, 26 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(17 * length + 6, 26 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(18 * length + 6, 26 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(19 * length + 6, 26 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(20 * length + 6, 26 * length + 6)));
    //
    pellets.push_back(new Pellet(false, sf::Vector2f(3 * length + 6, 27 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(3 * length + 6, 28 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(9 * length + 6, 27 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(9 * length + 6, 28 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(18 * length + 6, 27 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(18 * length + 6, 28 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(24 * length + 6, 27 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(24 * length + 6, 28 * length + 6)));
    //
    pellets.push_back(new Pellet(false, sf::Vector2f(2 * length + 6, 29 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(3 * length + 6, 29 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(4 * length + 6, 29 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(5 * length + 6, 29 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(9 * length + 6, 29 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(10 * length + 6, 29 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(11 * length + 6, 29 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(12 * length + 6, 29 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(15 * length + 6, 29 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(16 * length + 6, 29 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(17 * length + 6, 29 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(18 * length + 6, 29 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(22 * length + 6, 29 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(23 * length + 6, 29 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(24 * length + 6, 29 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(25 * length + 6, 29 * length + 6)));
    //
    pellets.push_back(new Pellet(false, sf::Vector2f(1 * length + 6, 29 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(1 * length + 6, 30 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(1 * length + 6, 31 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(26 * length + 6, 29 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(26 * length + 6, 30 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(26 * length + 6, 31 * length + 6)));
    //
    pellets.push_back(new Pellet(false, sf::Vector2f(12 * length + 6, 30 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(12 * length + 6, 31 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(15 * length + 6, 30 * length + 6)));
    pellets.push_back(new Pellet(false, sf::Vector2f(15 * length + 6, 31 * length + 6)));
}

void Game::draw_pellets(sf::RenderTarget & window)
{
    for(const auto & item: pellets)
    {
        item -> draw(window);
    }
}


Mode & scatter_chase_time_table(Chase & chase, Scatter & scatter, float seconds, int level)
{
    if(level == 1)
    {
        if(seconds < 7)
        {
            return scatter;
        }
        else if(seconds < 27)
        {
            return chase;
        }
        else if(seconds < 34)
        {
            return scatter;
        }
        else if(seconds < 54)
        {
            return chase;
        }
        else if(seconds < 59)
        {
            return scatter;
        }
        else if(seconds < 79)
        {
            return chase;
        }
        else if(seconds < 84)
        {
            return scatter;
        }
        else if(seconds >= 84)
        {
            return chase;
        }
    }
    else if(level >= 2 && level <= 4)
    {
        if(seconds < 7)
        {
            return scatter;
        }
        else if(seconds < 27)
        {
            return chase;
        }
        else if(seconds < 34)
        {
            return scatter;
        }
        else if(seconds < 54)
        {
            return chase;
        }
        else if(seconds < 59)
        {
            return scatter;
        }
        else if(seconds < 1092)
        {
            return chase;
        }
        else if(seconds < 1093.6)
        {
            return scatter;
        }
        else if(seconds >= 1093.6)
        {
            return chase;
        }
    }
    else if(level > 5)
    {
        if(seconds < 5)
        {
            return scatter;
        }
        else if(seconds < 25)
        {
            return chase;
        }
        else if(seconds < 30)
        {
            return scatter;
        }
        else if(seconds < 50)
        {
            return chase;
        }
        else if(seconds < 55)
        {
            return scatter;
        }
        else if(seconds < 1092)
        {
            return chase;
        }
        else if(seconds < 1093.6)
        {
            return scatter;
        }
        else if(seconds >= 1093.6)
        {
            return chase;
        }
    }
    return chase;
}

void Game::eat_pellets()
{
    int keep;
    bool found = false;
    for(int index = 0; index < pellets.size(); ++index)
    {
        if(pellets[index] -> get_rect().intersects(pacman.get_rect()))
        {
            keep = index;
            found = true;
            break;
        }
    }
    if(found)
    {
        ++pacman;
        pacman.increase_score(pellets[keep] -> get_score());
        if(pellets[keep] -> is_power_pellet())
        {
            for(auto & item: ghosts)
            {
                if(!item -> is_eaten() && !item -> is_in())
                {
                    item -> change_mode(&frightened);
                    item -> make_frightened(true);
                    item -> f_timer = 0;
                    turn_around_rule(item);
                    item -> white_blue_time_table.clear();
                    item -> white_blue_index = 0;
                }
            }
        }
        delete pellets[keep];
        pellets.erase(pellets.begin() + keep);
    }
}

bool Game::ghost_pac_collision()
{
    for(const auto & item: ghosts)
    {
        if(pacman.get_rect().intersects(item -> get_rect()))
        {
            if(item -> is_frightened() && !item -> is_eaten())
            {
                item -> change_mode(&eaten);
                item -> make_eaten(true);
                // item -> make_frightened(false);
                ++(item -> n_ghost_eaten);
                pacman.increase_score(pow(2, item -> n_ghost_eaten) * 100);
                return false;
            }
            else if(!item -> is_frightened() && !item -> is_eaten())
            {
                return true;
            }
        }
    }
    return false;//no collision
}
void speed_manager(Ghost * ghost, int level, Map & map)//0 showing normal speed 1 for frightened and 2 for pink areas
{
    if(level >= 1 && level <= 4)
    {
        if(ghost -> is_frightened() && !ghost -> is_eaten())
        {
            ghost -> change_speed(ghost -> speeds[0][1]);
        }
        else if(map.get_tile(ghost -> get_pos()).it_is_pink())
        {
            ghost -> change_speed(ghost -> speeds[0][2]);
        }
        else if(ghost -> is_eaten())
        {
            ghost -> change_speed(100);
        }
        else if(ghost -> is_in())
        {
            ghost -> change_speed(40);
        }
        else
        {
            ghost -> change_speed(ghost -> speeds[0][0]);
        }
    }
    else if(level >= 5 && level <= 20)
    {
        if(ghost -> is_frightened() && !ghost -> is_eaten())
        {
            ghost -> change_speed(ghost -> speeds[1][1]);
        }
        else if(map.get_tile(ghost -> get_pos()).it_is_pink())
        {
            ghost -> change_speed(ghost -> speeds[1][2]);
        }
        else if(ghost -> is_eaten())
        {
            ghost -> change_speed(100);
        }
        else if(ghost -> is_in())
        {
            ghost -> change_speed(40);
        }
        else
        {
            ghost -> change_speed(ghost -> speeds[1][0]);
        }
    }
    else if(level > 20)
    {
        if(ghost -> is_frightened() && !ghost -> is_eaten())
        {
            ghost -> change_speed(ghost -> speeds[2][1]);
        }
        else if(map.get_tile(ghost -> get_pos()).it_is_pink())
        {
            ghost -> change_speed(ghost -> speeds[2][2]);
        }
        else if(ghost -> is_eaten())
        {
            ghost -> change_speed(100);
        }
        else if(ghost -> is_in())
        {
            ghost -> change_speed(40);
        }
        else
        {
            ghost -> change_speed(ghost -> speeds[2][0]);
        }
    }
}

void Game::set_initial_state()
{
    for(auto & item: ghosts)
    {
        item -> set_pos(item -> get_home_pos());
        item -> make_eaten(false);
        item -> make_frightened(false);
        item -> make_in(true);
        item -> change_speed(30);
        item -> set_dir({0, -1});
        item -> s_c_timer = 0;
        item -> f_timer = 0;
        item -> time_index = 0;
        item -> f_time_index = 0;
        item -> change_mode(&scatter);
        item -> white_blue_index = 0;
        item -> set_initial_sprite();
    }
    pacman.set_pos(pacman.get_home_pos());
    pacman.set_dir({-1, 0});
    pacman.set_initial_sprite();
    ghosts[0] -> set_dir({-1, 0});
}

void Game::draw_fruits(sf::RenderTarget & window)
{
    for(const auto & item: fruits)
    {
        item -> draw(window);
    }
}

void Game::eat_fruits()
{
    int keep = 0;
    bool found = false;
    for(int index = 0; index < fruits.size(); ++index)
    {
        if(fruits[index] -> get_rect().intersects(pacman.get_rect()))
        {
            pacman.increase_score(fruits[index] -> get_score());
            keep = index;
            found = true;
            break;
        }
    }
    if(found)
    {
        delete fruits[keep];
        fruits.erase(fruits.begin() + keep);
        fruit_added = false;//so we can add another one later
    }
}

void Game::reset()
{
    for(auto & item: pellets)
    {
        delete item;
    }
    pellets.clear();
    pacman.reset_no_eaten_pellets();
    set_pellets();
    set_initial_state();
}

void Game::set_lives()
{
    for(int index = 0; index < pacman.get_no_lives() - 1; ++index)
    {
        lives.push_back(new Life);
        lives[index] -> set_pos(sf::Vector2f((index + 1) * 30, 34 * 20 + 5));
    }
}

void Game::draw_lives(sf::RenderTarget & window)
{
    for(const auto & item: lives)
    {
        item -> draw(window);
    }
}
void Game::pac_speed_manager()
{
    bool frightened_ghost = false;
    for(auto const & item: ghosts)
    {
        if(item -> is_frightened())
        {
            frightened_ghost = true;
            break;
        }
    }
    if(level >= 1 && level <= 4)
    {
        if(frightened_ghost)
        {
            pacman.change_mode(90);
        }
        else
        {
            pacman.change_mode(80);
        }
    }
    else if(level >= 5 && level <= 20)
    {
        if(frightened_ghost)
        {
            pacman.change_mode(95);
        }
        else
        {
            pacman.change_mode(90);
        }
    }
    else if(level >= 21 && level <= 32)
    {
        if(frightened_ghost)
        {
            pacman.change_mode(100);
        }
        else
        {
            pacman.change_mode(100);
        }
    }
    else if(level >= 33)
    {
        if(frightened_ghost)
        {
            //do nothing
        }
        else
        {
            pacman.change_mode(90);
        }
    }
}