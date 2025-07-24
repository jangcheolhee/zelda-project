#include "stdafx.h"
#include "Chest.h"
#include "Player.h"

void Chest::OnInteract()
{

	if (available && player->GetDirection() ==Direction::Up)
	{
		// 행위 추가
		available = false;
		//std::cout << 456;
	}
	

}

void Chest::Init()
{
	Interactable::Init();
	
}

void Chest::Reset()
{
	Interactable::Reset();

	body.setTexture(TEXTURE_MGR.Get("graphics/Overworld.png"));
	body.setTextureRect({ 304,74,16,16 });
	available = true;


	
}
