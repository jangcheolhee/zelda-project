#include "stdafx.h"
#include "HUD.h"
HUD::HUD(const std::string& name)
	: GameObject(name)
{
}

void HUD::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	body.setPosition(pos);
}

void HUD::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	body.setRotation(rot);
}

void HUD::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	body.setScale(s);
}

void HUD::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	body.setOrigin(o);
}

void HUD::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(body, preset);
	}
}

void HUD::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 0;
}

void HUD::Release()
{
}

void HUD::Reset()
{
	body.setTexture(TEXTURE_MGR.Get("graphics/HUD.png"));
	body.setTextureRect({0,0,256,224});
	SetScale({ 2.f,2.f });
}

void HUD::Update(float dt)
{
}

void HUD::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}