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

	body.setTexture(TEXTURE_MGR.Get("graphics/Overworld.png"));
	body.setTextureRect({ 304,57,8,8 });
	sf::Color color = body.getColor();
	color.a = 0;
	body.setColor(color);

	dir = { 0.f,0.f };

}