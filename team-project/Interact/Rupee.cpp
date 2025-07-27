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
	body.setTexture(TEXTURE_MGR.Get("graphics/Items.png"));
	body.setTextureRect({ 51,249,10,14 });

	type = Type::Item;
	value = 1;
}

void Rupee::OnInteract()
{
	player->AddRupee(value);
	SetActive(false);
}
