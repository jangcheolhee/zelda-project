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
}

void Enemy::Init()
{
	// 히트박스를 작게 설정
	sf::FloatRect bodyBounds = body.getLocalBounds();
	sf::Vector2f hitBoxSize(bodyBounds.width * 0.6f, bodyBounds.height * 0.6f); // 60% 크기
	sf::Vector2f hitBoxOffset((bodyBounds.width - hitBoxSize.x) / 2.f, (bodyBounds.height - hitBoxSize.y) / 2.f);

	hitBox.rect.setSize(hitBoxSize);
	hitBox.rect.setOrigin(hitBoxSize / 2.f);
	hitBox.rect.setPosition(body.getPosition() + hitBoxOffset);
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = -1;
	animator.SetTarget(&body);
	SetOrigin(Origins::BC);
	
	animator.AddEvent("Death", 6,
		[this]()
		{
			std::cout << "!!" << std::endl;
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
	SetScale({ 1.f,1.f });

	SetOrigin(Origins::BC);


}

void Enemy::Update(float dt)
{
	animator.Update(dt);
	LastHit += dt;
	// 매 프레임마다 피격 가능 상태로 초기화
	isHitThisFrame = false;

	animator.Update(dt);
	UpdateBehavior(dt);
	if (!player->IsAttacking() && player->checkCollision(hitBox))
	{
		player->TakeDamageIfPossible(1);
	}
	
	hitBox.UpdateTransform(body, GetLocalBounds());
	
}

void Enemy::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitBox.Draw(window);
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
	animator.Play("animations/EnemyDeath.csv");
}

