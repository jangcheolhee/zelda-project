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
	hitBox.UpdateTransform(body, body.getLocalBounds());
}

void Player::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	body.setRotation(rot);
	hitBox.UpdateTransform(body, body.getLocalBounds());
}

void Player::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	body.setScale(s);
	hitBox.UpdateTransform(body, body.getLocalBounds());
}

void Player::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	body.setOrigin(o);
	hitBox.UpdateTransform(body, body.getLocalBounds());
}

void Player::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(body, preset);
	}
	hitBox.UpdateTransform(body, body.getLocalBounds());
}

void Player::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
	
	std::string texPath = "graphics/Link.png";
	if (!TEXTURE_MGR.Exists(texPath))
	{
		TEXTURE_MGR.Load(texPath);
	}
	texture = &TEXTURE_MGR.Get(texPath);
	body.setTexture(*texture);

	// Down
	animations[Direction::Down] =
	{
			sf::IntRect(0, 0, 16, 32),
			sf::IntRect(20, 0, 16, 32),
			sf::IntRect(35, 0, 16, 32)
	};
	// Left
	animations[Direction::Left] =
	{
		sf::IntRect(20, 58, -20,27),
		sf::IntRect(53, 58, -16,23),              
		sf::IntRect(35, 58, -15,28)
	};
	// Right
	animations[Direction::Right] =
	{
		sf::IntRect(0, 58, 19, 25),
		sf::IntRect(22, 58, 15, 23),
		sf::IntRect(37, 58, 16, 23)
	};
	// Up
	animations[Direction::Up] =
	{
		sf::IntRect(-1, 113, 20, 19),
		sf::IntRect(17, 113, 19, 26),
		sf::IntRect(173, 113, 15, 25)
	};
	body.setScale(2.0f, 2.0f); // ũ�� ����
	body.setTextureRect(animations[currentDirection][currentFrame]);
	// ��Ʈ�ڽ� �ʱ�ȭ
	hitBox.UpdateTransform(body, body.getLocalBounds());
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
	
	sf::Vector2f movement(0.f, 0.f);
	bool moving = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
	{
		hitBox.visible = !hitBox.visible;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		movement.y -= speed * dt;
		currentDirection = Direction::Up;
		moving = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		movement.y += speed * dt;
		currentDirection = Direction::Down;
		moving = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
	{
		movement.x -= speed * dt;
		currentDirection = Direction::Left;
		moving = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
	{
		movement.x += speed * dt;
		currentDirection = Direction::Right;
		moving = true;
	}

	if (moving) {
		elapsedTime += dt;
		if (elapsedTime >= frameTime) 
		{
			currentFrame = (currentFrame + 1) % animations[currentDirection].size();
			body.setTextureRect(animations[currentDirection][currentFrame]);
			elapsedTime = 0.f;
		}
		body.move(movement);
		hitBox.UpdateTransform(body, body.getLocalBounds());
	}
	else {
		// �̵� ���ϸ� ù ������ ����
		currentFrame = 0;
		body.setTextureRect(animations[currentDirection][currentFrame]);
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitBox.Draw(window);
	
}

bool Player::checkCollision(const HitBox& other)
{
	return hitBox.rect.getGlobalBounds().intersects(other.rect.getGlobalBounds());
}
