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

	if (!GetActive())
		return;

	switch (type)
	{
	case Interactable::Type::None:
		break;
	case Interactable::Type::Throw:
		if (player->IsInteract())
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
				position += speed * dt * direction;
				SetPosition(position);
				if (shootTimer > 1)
				{
					SetActive(false);
				}
			}
		}
	case Interactable::Type::Chest:
		break;
	default:
		break;
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
		direction = { 0.f, 1.f };
		break;
	case Direction::Up:
		direction = { 0.f, -1.f };
		break;
	case Direction::Left:
		direction = { -1.f,0.f };
		break;
	case Direction::Right:
		direction = { 1.f, 0.f };
		break;
	}

}