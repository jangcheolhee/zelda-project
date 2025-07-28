#include "stdafx.h"
#include "JumpWall.h"
#include "Player.h"
void JumpWall::OnInteract()
{

	if (direction == player->GetDirection())
	{
		switch (direction)
		{
		case Direction::Down:
			dir = { 0.f,1.f };
			break;
		case Direction::Left:
			dir = { -1.f,0.f };
			break;
		case Direction::Right:
			dir = { 1.f,0.f };
			break;
		case Direction::Up:
			dir = { 0.f,-1.f };
			break;
		
		default:
			break;
		}
		player->SetPosition(player->GetGlobalBounds().getPosition() + dir * GetLocalBounds().width);
	}

}

void JumpWall::Init()
{
	Interactable::Init();
}

void JumpWall::Reset()
{
	Interactable::Reset();
	type = Type::JumpWall;
	dir = { 0.f,0.f };
	direction = Direction::None;
	body.setFillColor(sf::Color(0, 255, 0, 80));
	body.setOutlineColor(sf::Color::Green);
	body.setOutlineThickness(1.f);
	
	

}

void JumpWall::SetBounds(float x, float y, float width, float height)
{
	body.setSize({ width,height });
	body.setPosition({ x,y });
	
	
}

void JumpWall::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}
