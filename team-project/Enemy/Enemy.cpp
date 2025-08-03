#include "stdafx.h"
#include "Enemy.h"
#include "BasicEnemy.h"
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

void Enemy::OnCollide(Direction direction)
{
	switch (direction)
	{
	case Direction::Left:
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



void Enemy::Init()
{
	sf::FloatRect bodyBounds = body.getLocalBounds();
	sf::Vector2f hitBoxSize(bodyBounds.width * 0.6f, bodyBounds.height * 0.6f);
	sf::Vector2f hitBoxOffset((bodyBounds.width - hitBoxSize.x) / 2.f, (bodyBounds.height - hitBoxSize.y) / 2.f);

	sortingLayer = SortingLayers::Enemy;
	sortingOrder = 3;
	animator.SetTarget(&body);
	SetOrigin(Origins::TL);
	boundBox.rect.setSize({ 16, 24 });
	boundBox.SetOrigin(Origins::TL);
	hitBox.rect.setSize({ 8, 12 }); 
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

	// JumpWall과의 충돌 체크 (움직이기 전에)
	bool canMove = true;
	for (auto& obj : interList)
	{
		if (obj->GetType() == Interactable::Type::JumpWall)
		{
			// 다음 위치에서 충돌하는지 미리 체크
			sf::Vector2f nextPosition = GetPosition() + velocity * dt;
			sf::RectangleShape nextBounds = GetBoundBox().rect;
			nextBounds.setPosition(nextPosition);

			if (Utils::CheckCollision(obj->GetHitBox().rect, nextBounds))
			{
				canMove = false;
				// BasicEnemy의 경우 방향 바꾸기
				if (auto basicEnemy = dynamic_cast<BasicEnemy*>(this))
				{
					// 현재 방향의 반대 방향으로 설정
					switch (direction)
					{
					case Direction::Up:
						direction = Direction::Down;
						break;
					case Direction::Down:
						direction = Direction::Up;
						break;
					case Direction::Left:
						direction = Direction::Right;
						break;
					case Direction::Right:
						direction = Direction::Left;
						break;
					}
					basicEnemy->ChangeSprite();
				}
				break;
			}
		}
	}

	// 다른 오브젝트들과의 기존 충돌 처리
	for (auto& obj : interList)
	{
		if (obj->GetType() != Interactable::Type::JumpWall &&
			Utils::CheckCollision(obj->GetHitBox().rect, GetBoundBox().rect))
		{
			sf::FloatRect objRect = obj->GetHitBox().rect.getGlobalBounds();
			sf::FloatRect enemyRect = GetBoundBox().rect.getGlobalBounds();

			float objX = objRect.left + objRect.width / 2.f;
			float objY = objRect.top + objRect.height / 2.f;

			float enemyX = enemyRect.left + enemyRect.width / 2.f;
			float enemyY = enemyRect.top + enemyRect.height / 2.f;

			float dx = objX - enemyX;
			float dy = objY - enemyY;

			float combinedHalfWidth = (objRect.width + enemyRect.width) / 2.f;
			float combinedHalfHeight = (objRect.height + enemyRect.height) / 2.f;

			float overlapX = combinedHalfWidth - std::abs(dx);
			float overlapY = combinedHalfHeight - std::abs(dy);

			if (overlapX < overlapY)
			{
				if (dx < 0)
				{
					OnCollide(Direction::Left);
				}
				else
				{
					OnCollide(Direction::Right);
				}
				if (dy < 0)
				{
					OnCollide(Direction::Up);
				}
				else
				{
					OnCollide(Direction::Down);
				}
			}
		}
	}

	animator.Update(dt);
	LastHit += dt;
	// 매 프레임마다 피격 가능 상태로 초기화
	isHitThisFrame = false;

	if (!player->IsAttacking() && player->checkCollision(hitBox) && !isDie)
	{
		player->TakeDamageIfPossible(1);
	}

	hitBox.rect.setScale(GetScale());
	hitBox.rect.setPosition(GetPosition() + sf::Vector2f{ 4 * GetScale().x,6 * GetScale().y });
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
	OnDamage(1);//데미지 처리
	LastHit = 0.f;

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
