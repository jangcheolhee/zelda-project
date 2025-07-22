#include "stdafx.h"
#include "Player.h"

Player::Player(const std::string& name)
	: GameObject(name)
{
}

void Player::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	body.setPosition(pos);
}

void Player::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	body.setRotation(rot);
}

void Player::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	body.setScale(s);
}

void Player::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	body.setOrigin(o);
}

void Player::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(body, preset);
	}
}

void Player::Init()
{
}

void Player::Release()
{
	GAME_MGR.SetPlayerData(hp, position);
}

void Player::Reset()
{

	hp = GAME_MGR.GetPlayerData().hp;
	SetPosition(GAME_MGR.GetPlayerData().position);

}
void Player::Update(float dt)
{
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	
}