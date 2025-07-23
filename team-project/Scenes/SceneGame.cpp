#include "stdafx.h"
#include "SceneGame.h"
#include "Player.h"
#include "TileMap.h"

SceneGame::SceneGame()
	:Scene(SceneIds::Game)
{
}

void SceneGame::Init()
{
	//texIds.push_back("graphics/sprite_sheet.png");
	//fontIds.push_back("fonts/DS-DIGIT.ttf");

	//ANI_CLIP_MGR.Load("animations/idle.csv");
	//ANI_CLIP_MGR.Load("animations/run.csv");
	//ANI_CLIP_MGR.Load("animations/jump.csv");


	player = new Player();
	tileMap = new TileMap("TileMap");
	tileMap->Init();
	tileMap->Reset();

	AddGameObject(player);
	AddGameObject(tileMap);
	
	Scene::Init();
}

void SceneGame::Enter()
{
	auto size = FRAMEWORK.GetWindowSizeF();
	sf::Vector2f center{ size.x * 0.5f, size.y * 0.5f };
	uiView.setSize(size);
	uiView.setCenter(center);
	worldView.setSize(size);
	worldView.setCenter({ 0.f, -200.f });
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);
} 