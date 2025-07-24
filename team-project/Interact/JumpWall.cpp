#include "stdafx.h"
#include "JumpWall.h"
#include "Player.h"
void JumpWall::OnInteract()
{
	
	if (direction == player->GetDirection())
	{

		player->SetPosition(player->GetPosition() + dir * GetLocalBounds().width);
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
	dir = { -1.f,0.f };
	direction = Direction::Left;



	body.setTexture(TEXTURE_MGR.Get("graphics/Overworld.png"));
	body.setTextureRect({ 304,57,16,16 });
	sf::Color color = body.getColor();
	color.a = 0;
	body.setColor(color);

	SetPosition({ -40.f,-40.f });
}
