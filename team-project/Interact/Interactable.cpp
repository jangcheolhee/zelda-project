#include "stdafx.h"
#include "Interactable.h"
#include "Player.h"

Interactable::Interactable(const std::string& name)
	: GameObject(name)
{
}

void Interactable::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	body.setPosition(pos);
}

void Interactable::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	body.setRotation(rot);
}

void Interactable::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	body.setScale(s);
}

void Interactable::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	body.setOrigin(o);
}

void Interactable::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(body, preset);
	}
}

void Interactable::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
	SetOrigin(Origins::BR);
}

void Interactable::Release()
{
}

void Interactable::Reset()
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
	isShoot = false; 
	shootTimer = 0.f;
}

void Interactable::Update(float dt)
{
	if (!GetActive()) return;

	sf::FloatRect playerBounds = player->GetGlobalBounds();
	playerBounds.left -= 8.f;
	playerBounds.top -= 8.f;
	playerBounds.width += 16.f;
	playerBounds.height += 16.f;

	if (playerBounds.intersects(GetGlobalBounds() ) && !isShoot)
	{
		if (InputMgr::GetKeyDown(sf::Keyboard::X))
		{
			OnInteract(); // 상태 변경, 파괴, 대화 등
			return;
		}
	}
	if (player->IsInteract() )
	{
		if (isShoot)
		{
			sf::Vector2f pos = player->GetGlobalBounds().getPosition();
			pos.x += player->GetLocalBounds().width * 0.5f;
			pos.y -= player->GetLocalBounds().height;
			SetPosition(pos);
			if (InputMgr::GetKeyDown(sf::Keyboard::X) || InputMgr::GetKeyDown(sf::Keyboard::Z))
			{
				Shoot(); // 상태 변경, 파괴, 대화 등
			}
		}
		else
		{
			shootTimer += dt;
			if (shootTimer > 1) SetActive(false);
		}
	}
	hitBox.UpdateTransform(body, GetLocalBounds());
}

void Interactable::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitBox.Draw(window);
}

void Interactable::Shoot()
{
	isShoot = false;
	switch (player->GetDirection())
	{
	case Direction::Down:
		break;
	case Direction::Up:
		break;
	case Direction::Left:
		break;
	case Direction ::Right:
		break;
	default:
		break;
	}
}