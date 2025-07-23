#pragma once
#include "stdafx.h"
#include "AnimationPlayer.h"

class Character:public GameObject
{
protected:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f position;
    float speed = 100.f;
    AnimationPlayer* animPlayer = nullptr;
    AnimationClip currentClip;
public:
    virtual void Init(const std::string& texturePath, sf::Vector2f startPos)
    {
        texture.loadFromFile(texturePath);
        sprite.setTexture(texture);
        position = startPos;
        sprite.setPosition(GetPosition());

        currentClip.name = "default_clip";
        currentClip.loop = true;
        animPlayer = new AnimationPlayer(&currentClip);
    }

    virtual void Update(float dt)
    {
        if (animPlayer)
        {
            animPlayer->Update(dt);
            sprite.setTextureRect(animPlayer->GetCurrentFrameRect());
        }
    }

    virtual ~Character()
    {
       // delete animPlayer;
    }
    virtual void Draw(sf::RenderWindow& window)
    {
        window.draw(sprite);
    }
};

