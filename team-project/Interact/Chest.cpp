#include "stdafx.h"
#include "Chest.h"

void Chest::OnInteract()
{
	available = false;

}

void Chest::Init()
{
	Interactable::Init();
	available = true;
}

void Chest::Reset()
{
	Interactable::Reset();

	body.setTexture(TEXTURE_MGR.Get("graphics/Overworld.png"));
	body.setTextureRect({ 304,74,16,16 });
	
}
