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
	std::cout << 1345;
}

void Enemy::OnDamage(int damage)
{

	hp = Utils::Clamp(hp - damage, 0, maxHp);
	if (hp == 0)
	{
		SetActive(false);
	}
}

void Enemy::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = -1;
	animator.SetTarget(&body);

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
	UpdateBehavior(dt);
	if (player->GetGlobalBounds().intersects(GetGlobalBounds()))
	{
		// Player한테 데미지 주는 곳 
		// ex) player->OnDamage(damage);
	}
	hitBox.UpdateTransform(body, GetLocalBounds());


}

void Enemy::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitBox.Draw(window);
}

