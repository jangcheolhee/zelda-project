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

	type = Type::Rupee;
	value = 1;
}

void Rupee::OnInteract()
{
	if (GetActive())
	{
		player->AddRupee(value);
		GAME_MGR.playerRupee = player->GetRupee();
		SetActive(false);
	}
}

void Rupee::Update(float dt)
{
	Interactable::Update(dt);
	life += dt;
	if ((int)life % 3 == 0)
	{
		body.setTextureRect({ 51,249,10,14 });
	}
	else if ((int)life % 3 == 1)
	{
		body.setTextureRect({ 35,249,10,14 });
	}
	else
	{
		body.setTextureRect({ 67,249,10,14 });
	}
}
