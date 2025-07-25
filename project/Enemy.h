#pragma once
#include "stdafx.h"
#include "Character.h"
class Enemy :
    public Character
{
public:
    void Init(const std::string& texturePath, sf::Vector2f startPos) override
    {
        Character::Init(texturePath, startPos);
    }

    void Update(float dt) override
    {
        // 예시: 계속 오른쪽 이동
        position.x += speed * dt * 0.5f;
        sprite.setPosition(position);
    }

    // Character을(를) 통해 상속됨
    void Init() override;
    void Release() override;
    void Reset() override;
};

