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
		UpdateBeHavior(dt);
		break;
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
