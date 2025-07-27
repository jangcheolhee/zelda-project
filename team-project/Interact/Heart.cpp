#include "stdafx.h"
#include "Heart.h"
#include "Player.h"

void Heart::Init()
{
	Interactable::Init();
}

void Heart::Reset()
{
	Interactable::Reset();

	body.setTexture(TEXTURE_MGR.Get("graphics/Items.png"));
	body.setTextureRect({ 104,280,8,8 });
	SetOrigin(Origins::BL);
	type = Type::Item;
	
}

void Heart::OnInteract()
{
	if (GetActive())
	{
		//player->AddRupee(value);
		SetActive(false);
	}
}
