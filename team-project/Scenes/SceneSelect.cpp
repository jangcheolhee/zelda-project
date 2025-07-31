#include "stdafx.h"
#include "SceneSelect.h"
#include "Player.h"
#include "TextGo.h"
#include "SaveSlotUI.h"
SceneSelect::SceneSelect()
	: Scene(SceneIds::Select)
{
}

void SceneSelect::Init()
{
	texIds.push_back("graphics/Items.png");

	fontIds.push_back("fonts/DS-DIGIT.ttf");
	
	saveSloatUI = (SaveSlotUI*)AddGameObject(new SaveSlotUI());

	saveSloatUI->onSlotSelected = [](int slotIndex)
		{
			GAME_MGR.LoadFromSlot(slotIndex);
			SCENE_MGR.ChangeScene((SceneIds)GAME_MGR.currentMapID);
		};

	saveSloatUI->onSlotDelete = [](int slotIndex)
		{
			GAME_MGR.DeleteSaveSlot(slotIndex);
		};

	Scene::Init();
}

void SceneSelect::Enter()
{
	auto size = FRAMEWORK.GetWindowSizeF();
	sf::Vector2f center{ size.x * 0.5f, size.y * 0.5f };
	worldView.setSize({ 0,0 });
	uiView.setSize(size);
	uiView.setCenter(center);

	Scene::Enter();
}

void SceneSelect::Update(float dt)
{
	Scene::Update(dt);
}
