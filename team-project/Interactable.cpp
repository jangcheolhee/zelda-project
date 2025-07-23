#include "stdafx.h"
#include "Interactable.h"
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
	
}

void Interactable::Release()
{
}

void Interactable::Reset()
{
}

void Interactable::Update(float dt)
{
}

void Interactable::Draw(sf::RenderWindow& window)
{
}