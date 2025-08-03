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

	if (Utils::Distance(GetPosition(), player->GetPosition()) < 50)
	{
		state = EnemyState::Chase;
	}
	else
	{
		state = EnemyState::Patrol;
	}

	if (state == EnemyState::Patrol)
	{
		moveTimer += dt;

		if (moveTimer > 3)
		{
			direction = (Direction)Utils::RandomRange(0, 4);
			moveTimer = 0;
			ChangeSprite();
		}
	}
	else if (state == EnemyState::Chase)
	{
		dir = Utils::GetNormal(player->GetPosition() - GetPosition());
		moveTimer = 0;
	}

	velocity = dir * speed;

	// Enemy::Update()에서 canMove를 확인한 후에만 위치 업데이트
	// Enemy::Update() 호출 전에 velocity를 설정해야 함
	Enemy::Update(dt);

	// Enemy::Update()에서 벽 충돌을 체크했으므로, 
	// 여기서는 velocity가 수정되었을 수 있음을 고려
	position += velocity * dt;
	SetPosition(position);
	boundBox.rect.setPosition(GetPosition());
}

void BasicEnemy::ChangeSprite()
{
	velocity = { 0,0 };

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

