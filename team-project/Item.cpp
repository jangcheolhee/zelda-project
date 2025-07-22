#include "stdafx.h"
#include "Item.h"
Item::Item(const std::string& name)
	: GameObject(name)
{
}

void Item::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	body.setPosition(pos);
}

void Item::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	body.setRotation(rot);
}

void Item::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	body.setScale(s);
}

void Item::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	body.setOrigin(o);
}

void Item::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(body, preset);
	}
}

void Item::Init()
{
}

void Item::Release()
{
}

void Item::Reset()
{
}

void Item::Update(float dt)
{
}

void Item::Draw(sf::RenderWindow& window)
{
}