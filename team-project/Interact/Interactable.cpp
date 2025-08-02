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
	sortingOrder = 1;
	SetOrigin(Origins::BR);
	animator.SetTarget(&body);
	hitBox.SetOrigin(Origins::BL);
	hitBox.rect.setSize({ 8,8 });
	boundBox.SetOrigin(Origins::MC);
	boundBox.rect.setSize({ 16,16 });

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
	animator.Update(dt);
	if (!GetActive())
		return;
	hitBox.rect.setPosition(GetPosition());
	boundBox.rect.setScale(GetScale());
	boundBox.rect.setPosition(GetPosition());
	
	
}
void Interactable::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitBox.Draw(window);
	boundBox.Draw(window);
}