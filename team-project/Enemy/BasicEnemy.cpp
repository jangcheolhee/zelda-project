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

	// ���⼭ sprite texture �����ϱ�
	Enemy::Reset();
	body.setTexture(TEXTURE_MGR.Get("graphics/Enemy_sheet.png"));
	direction = (Direction)Utils::RandomRange(0, 4);
	ChangeSprite();

	
	state = EnemyState::Patrol;
	maxHp = 5;
	hp = maxHp;
	moveTimer = 0;
}

void BasicEnemy::Update(float dt)
{
	previousPosition = GetPosition();
	moveTimer += dt;

	if (Utils::Distance(player->GetPosition() ,GetPosition()) < 80)
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
			ChangeSprite();
			moveTimer = 0.f;
			body.setColor(sf::Color (0xffffffff));
		}
	}
	else if (state == EnemyState::Chase)
	{
		dir = Utils::GetNormal(player->GetPosition() - GetPosition());
		moveTimer = 0;
	}

	velocity = dir * speed;

	// Enemy::Update()���� canMove�� Ȯ���� �Ŀ��� ��ġ ������Ʈ
	// Enemy::Update() ȣ�� ���� velocity�� �����ؾ� ��
	Enemy::Update(dt);

	// Enemy::Update()���� �� �浹�� üũ�����Ƿ�, 
	// ���⼭�� velocity�� �����Ǿ��� �� ������ ���
	position += velocity * dt;
	SetPosition(position);

	hitBox.UpdateTransform(body, GetLocalBounds());
	hitBox.rect.setSize({ 20,26 });
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

