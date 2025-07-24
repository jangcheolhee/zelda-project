#include "stdafx.h"
#include "Rupee.h"
#include "Player.h"
void Rupee::Init()
{
	

	Interactable::Init();
	
}

void Rupee::Reset()
{
	Interactable::Reset();

	body.setTexture(TEXTURE_MGR.Get("graphics/Overworld.png"));
	body.setTextureRect({ 304,74,8,8 });
	SetPosition({ -10.f,-10.f });
	type = Type::Item;
	value = 1;
}

void Rupee::OnInteract()
{
	player->AddRupee(value);
	SetActive(false);
}
