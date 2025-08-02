#include "stdafx.h"
#include "BasicEnemy.h""
#include "Player.h"

void BasicEnemy::Init()
{
	Enemy::Init();
	type = Types::Basic;
	speed = 40.f;
	hp = 10;


}

void BasicEnemy::Reset()
{

	// 여기서 sprite texture 변경하기
	Enemy::Reset();
	body.setTexture(TEXTURE_MGR.Get("graphics/Enemy_sheet.png"));
	direction = (Direction)Utils::RandomRange(0, 4);
	ChangeSprite();

	state = EnemyState::Patrol;
	maxHp = 5;
	hp = maxHp;
}

void BasicEnemy::Update(float dt)
{
	pastPosition = GetPosition();
	moveTimer += dt;

	//if (moveTimer > 3)
	//{
	//	direction = (Direction)Utils::RandomRange(0, 4);
	//	moveTimer = 0;
	//	ChangeSprite();
	//}
	dir = Utils::GetNormal(player->GetPosition() - GetPosition());

	position += dir * dt * 30.f;
	SetPosition(position);

	hitBox.UpdateTransform(body, GetLocalBounds());
	Enemy::Update(dt);
	for (auto& obj : interList)
	{
		if (Utils::CheckCollision(hitBox.rect, obj->GetHitBox().rect))
		{
			CheckCollide(obj->GetHitBox());

		}
	}

}

void BasicEnemy::ChangeSprite()
{
	velocity = { 0,0 };

	switch (direction)
	{
	case  Direction::Up:
		body.setTextureRect({ 7,924,22,28 });
		dir = { 0.f,-1.f };
		break;
	case Direction::Down:
		body.setTextureRect({ 12,728,22,28 });
		dir = { 0.f,1.f };
		break;
	case Direction::Right:
		body.setTextureRect({ 12,825,22,28 });
		dir = { 1.f,0.f };
		SetScale({ 1.f,1.f });
		break;
	case Direction::Left:
		body.setTextureRect({ 12,825,22,28 });
		dir = { -1.f,0.f };
		SetScale({ -1.f,1.f });
		break;
	}

}

