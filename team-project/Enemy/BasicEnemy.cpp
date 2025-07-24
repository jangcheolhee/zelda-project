#include "stdafx.h"
#include "BasicEnemy.h""

void BasicEnemy::Init()
{
	Enemy::Init();
	type = Types::Basic;
	speed = 40.f;
	hp = 40;

	
}

void BasicEnemy::Reset()
{
	Enemy::Reset();
	// 여기서 sprite texture 변경하기
	body.setTexture(TEXTURE_MGR.Get("graphics/Enemy_sheet.png"));
	direction = (Direction) Utils::RandomRange(0, 4);
	ChangeSprite();
}

void BasicEnemy::UpdateBehavior(float dt)
{
	moveTimer += dt;
	if (moveTimer > 2)
	{
		dir = { 0.f,0.f };
		if (moveTimer > 3)
		{
			direction = (Direction)Utils::RandomRange(0, 4);
			moveTimer = 0;
			ChangeSprite();
		}
		
	}
	position += dir * speed * dt;
	SetPosition(position);
}

void BasicEnemy::ChangeSprite()
{
	switch (direction)
	{
	case  Direction::Up:
		
		body.setTextureRect({ 7,924,24,25 });
		dir = { 0.f,-1.f };
		break;
	case Direction::Down:
		body.setTextureRect({ 12,728,22,26 });
		dir = { 0.f,1.f };
		break;
	case Direction::Right:
		body.setTextureRect({ 12,825,18,28 });
		dir = { 1.f,0.f };
		SetScale({ 1.f,1.f });
		break;
	case Direction::Left:
		body.setTextureRect({ 12,825,18,28 });
		dir = { -1.f,0.f };
		SetScale({ -1.f,1.f });
		break;
	}
}
