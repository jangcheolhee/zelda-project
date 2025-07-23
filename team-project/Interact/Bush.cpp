#include "stdafx.h"
#include "Bush.h"

void Bush::OnInteract()
{
	// 플레이어에게 부쉬 들었다고 전달하기 -> bool타입으로 전달 ?
	// 플레이어는 부쉬 든 상태를 해결
	SetActive(false);
}

void Bush::Init()
{
	Interactable::Init();
}

void Bush::Reset()
{
	Interactable::Reset();
	body.setTexture(TEXTURE_MGR.Get("graphics/sprite_sheet.png"));
	body.setTextureRect({ 70,30,18,26 });

}
