#ifndef _ANIMATION
#define _ANIMATION

#include <SFML/Graphics.hpp>

class Animation
{
    public:
        Animation();
        Animation(std::string, int, float, int, int, int, int);
        void ApplyToSprite(sf::Sprite &);
        void Update(float);
        void Advance();
        const sf::Texture & get_texture() const;
    private:
        int nFrames;//number of frames
        float holdTime;
        sf::Texture texture;
        std::vector<sf::IntRect> frames;
        int iFrame = 0;//index of frames
        float time = 0.f;
};

enum AnimationIndex
{
    walkingUp = 0,
    walkingDown,
    walkingLeft,
    walkingRight,
    count
};

enum AnimationColor
{
    BLUE = 0,
    WHITE,
    Count
};

#endif // _ANIMATION