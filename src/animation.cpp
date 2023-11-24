#include "animation.hpp"

Animation::Animation() = default;

Animation::Animation(std::string theTexture, int no_frames, float hold_time, int x, int y, int width, int height)
{
    if(!texture.loadFromFile(theTexture))
    {
        throw std::logic_error("Couldn't load the Image!\n");
    }
    nFrames = no_frames;
    holdTime= hold_time;
    frames.resize(nFrames);

    for(int index = 0; index < nFrames; ++index)
    {
        frames[index] = sf::IntRect(x + index * width, y, width, height);
    }
}

void Animation::ApplyToSprite(sf::Sprite & sprite)
{
    sprite.setTexture(texture);
    sprite.setTextureRect(frames[iFrame]);
}

void Animation::Update(float dt)
{
    time += dt;

    if(time >= holdTime)
    {
        time = 0;
        Advance();
    }
}

void Animation::Advance()
{
    if(++iFrame == nFrames)
    {
        iFrame = 0;
    }
}

const sf::Texture & Animation::get_texture() const
{
    return texture;
}