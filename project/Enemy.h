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
        // ����: ��� ������ �̵�
        position.x += speed * dt * 0.5f;
        sprite.setPosition(position);
    }

    // Character��(��) ���� ��ӵ�
    void Init() override;
    void Release() override;
    void Reset() override;
};

