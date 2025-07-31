#include "stdafx.h"
#include "Heart.h"
#include "Player.h"
#include "HUD.h"
#include "SceneGame.h"

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
	SceneGame* sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	if (!sceneGame) return;

	Player* player = sceneGame->GetPlayer(); // 플레이어 가져오기
	HUD* hud = sceneGame->GetHUD();          // HUD 가져오기

	if (player)
	{
		player->Heal(2); // 체력 2 회복
	}

	if (hud)
	{
		hud->SetHeartCount(player->GetHp()); // HUD에 최신 HP 반영
	}

	SetActive(false); // 하트 아이템 비활성화 (사라지게)
	/*if (GetActive())
	{
		
		SetActive(false);
	}*/
}
