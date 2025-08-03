#include "stdafx.h"
#include "Enemy.h"
#include "SceneGame.h"
#include "SceneHidden.h"
#include "SceneBoss.h"
#include "SceneCastle.h"
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




void Enemy::Init()
{
	// 히트박스를 작게 설정
	sf::FloatRect bodyBounds = body.getLocalBounds();
	sf::Vector2f hitBoxSize(bodyBounds.width * 0.6f, bodyBounds.height * 0.6f); // 60% 크기
	sf::Vector2f hitBoxOffset((bodyBounds.width - hitBoxSize.x) / 2.f, (bodyBounds.height - hitBoxSize.y) / 2.f);




	sortingLayer = SortingLayers::Enemy;
	sortingOrder = 3;
	animator.SetTarget(&body);

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
	
	timer += dt;

	switch (SCENE_MGR.GetCurrentSceneId())
	{
	case SceneIds::Game:
		interList = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->GetInteract();
		break;
	case SceneIds::Hidden:
		interList = dynamic_cast<SceneHidden*>(SCENE_MGR.GetCurrentScene())->GetInteract();
		break;
	case SceneIds::Castle:
		interList = dynamic_cast<SceneCastle*>(SCENE_MGR.GetCurrentScene())->GetInteract();
		break;
	case SceneIds::Boss:
		interList = dynamic_cast<SceneBoss*>(SCENE_MGR.GetCurrentScene())->GetInteract();
		break;
	}
	
	

	animator.Update(dt);
	LastHit += dt;
	// 매 프레임마다 피격 가능 상태로 초기화
	isHitThisFrame = false;

	if (!player->IsAttacking() && player->checkCollision(hitBox) && !isDie)
	{
		player->TakeDamageIfPossible(1);
	}

	
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
	OnDamage(1);//데미지 처리
	LastHit = 0.f;

}

void Enemy::CheckCollide(HitBox box)
{
	
	sf::Vector2f position = GetPosition();
	sf::Vector2f newPosition = position;

	// X축 이동 먼저 시도
	newPosition.x = previousPosition.x;
	SetPosition({ newPosition.x, position.y });
	hitBox.UpdateTransform(body, body.getLocalBounds());
	hitBox.rect.setSize({ 20, 26 });

	if (Utils::CheckCollision(hitBox.rect, box.rect)) {
		newPosition.x = previousPosition.x;
	}

	// Y축 이동 시도
	newPosition.y = previousPosition.y;
	SetPosition({ newPosition.x, newPosition.y });
	hitBox.UpdateTransform(body, body.getLocalBounds());
	hitBox.rect.setSize({ 20, 26 });

	if (Utils::CheckCollision(hitBox.rect, box.rect)) {
		newPosition.y = previousPosition.y;
	}

	// 최종 위치 반영
	SetPosition(newPosition);
	hitBox.UpdateTransform(body, body.getLocalBounds());
	hitBox.rect.setSize({ 20, 26 });
}



void Enemy::DeathAnimation()
{
	hitBox.rect.setSize({ 0,0 });
	SetPosition(previousPosition);
	animator.Play("animations/EnemyDeath.csv");
}

void Enemy::OnDamage(int damage)
{
	
	if (timer > 1)
	{
		hp = Utils::Clamp(hp - damage, 0, maxHp);

		SOUND_MGR.PlaySfx(SOUNDBUFFER_MGR.Get("effects/enemy hit.wav"));
		if (hp == 0)
		{
			isDie = true;
			DeathAnimation();
		}
		timer = 0;
	}
	
}
