#include "stdafx.h"
#include "Npc.h"

void Npc::OnInteract()
{
}

void Npc::Init()
{
	Interactable::Init();
}

void Npc::Reset()
{
	body.setTexture(TEXTURE_MGR.Get("graphics/npc.png"));
	SetOrigin(Origins::ML);
	Interactable::Reset();
}