#include "stdafx.h"
#include "Enemy.h"
#include "SceneGame.h"
#include "Player.h"
Enemy::Enemy(const std::string& name)
	: GameObject(name)
{
}

void Enemy::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	body.setPosition(pos);
}

void Enemy::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	body.setRotation(rot);
}

void Enemy::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	body.setScale(s);
}

void Enemy::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	body.setOrigin(o);
}

void Enemy::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(body, preset);
	}
}



void Enemy::OnCollide(Direction direction)
{
	switch (direction)
	{
	case Direction::Left : 
		position.x += 0.2;
		break;
	case Direction::Right:
		position.x -= 0.2;
		break;
	case Direction::Up: 
		position.y += 0.2;
		break;
	case Direction::Down:
		position.y -= 0.2;
		break;
	}
}

void Enemy::OnCollide(Player* player)
{
	//std::cout << 1345;
}



void Enemy::OnDamage(int damage)
{
	
	hp = Utils::Clamp(hp - damage, 0, maxHp);
	if (hp == 0)
	{
		DeathAnimation();
	}
	std::cout << 34;
}

void Enemy::Init()
{
	// 히트박스를 작게 설정
	sf::FloatRect bodyBounds = body.getLocalBounds();
	sf::Vector2f hitBoxSize(bodyBounds.width * 0.6f, bodyBounds.height * 0.6f); // 60% 크기
	sf::Vector2f hitBoxOffset((bodyBounds.width - hitBoxSize.x) / 2.f, (bodyBounds.height - hitBoxSize.y) / 2.f);
	
	


	sortingLayer = SortingLayers::Foreground;
	sortingOrder = -1;
	animator.SetTarget(&body);
	SetOrigin(Origins::TL);
	boundBox.rect.setSize({ 16,24 });
	boundBox.SetOrigin(Origins::TL);
	hitBox.rect.setSize({8,12});
	hitBox.SetOrigin(Origins::TL);
	
	animator.AddEvent("Death", 6,
		[this]()
		{
			std::cout << "!!" << std::endl;
			animator.Stop();
			SetActive(false);
		}
	);
}

void Enemy::Release()
{
}

void Enemy::Reset()
{
	if (SCENE_MGR.GetCurrentSceneId() == SceneIds::Game)
	{
		sceneGame = (SceneGame*)SCENE_MGR.GetCurrentScene(); //�ٿ� ĳ���� 
	}
	else
	{
		sceneGame = nullptr;
	}
	player = (Player*)SCENE_MGR.GetCurrentScene()->FindGameObject("Player");

	SetActive(true);
	SetPosition(initPosition);
}

void Enemy::Update(float dt)
{
	animator.Update(dt);
	LastHit += dt;
	// 매 프레임마다 피격 가능 상태로 초기화
	isHitThisFrame = false;
	
	if (!player->IsAttacking() && player->checkCollision(hitBox))
	{
		player->TakeDamageIfPossible(1);
	}
	
	
	hitBox.rect.setScale(GetScale());
	hitBox.rect.setPosition(GetPosition() + sf::Vector2f{4 * GetScale().x,6*GetScale().y});
	boundBox.rect.setScale(GetScale());
	boundBox.rect.setPosition(GetPosition());
}

void Enemy::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitBox.Draw(window);
	boundBox.Draw(window);
}

void Enemy::OnCollideBySword()//책임 분산을 위해 함수 사용
{
	if (LastHit < hitCooldown)
		return;//공격 쿨타임이 남아 있으면 리턴
		
	std::cout << "[Enemy] sword -1 damage" << std::endl;
	OnHit(1);//데미지 처리
	LastHit = 0.f;
	
}

void Enemy::OnHit(int damage)
{
	hp -= damage;
	if (hp <= 0)
	{
		SetActive(false);
	}
}

void Enemy::DeathAnimation()
{
	SetPosition(pastPosition);
	animator.Play("animations/EnemyDeath.csv");
}

