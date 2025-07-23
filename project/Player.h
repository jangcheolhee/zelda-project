#pragma once
#include "stdafx.h"
#include "Character.h"
#include "AnimationApp.h"
#include "AnimationClip.h"
#include "GameObject.h"

class Player :
    public Character
{
private:
    std::string currentDirection = "Down";
   
public:
    void Init(const std::string& texturePath, sf::Vector2f startPos) override
    {
        Character::Init(texturePath, startPos);
      
    }

    void SetDirection(const std::string& dir)
    {
        currentDirection = dir;
    }

    void Update(float dt) override
    {
        bool moved = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            position.x -= speed * dt;
            SetDirection("Left");
            moved = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            position.x += speed * dt;
            SetDirection("Right");
            moved = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            position.y -= speed * dt;
            SetDirection("Up");
            moved = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            position.y += speed * dt;
            SetDirection("Down");
            moved = true;
        }

        sprite.setPosition(position);
        if (moved)
        {
            if (!animPlayer->IsPlaying())
                animPlayer->Play();
        }
        else
        {
            animPlayer->Stop();
        }

        animPlayer->Update(dt);
        sprite.setTextureRect(animPlayer->GetCurrentFrameRect());
    }


    // Character을(를) 통해 상속됨
    void Init() override;

    void Release() override;

    void Reset() override;

};

