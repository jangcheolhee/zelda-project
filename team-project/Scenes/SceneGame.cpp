#include "stdafx.h"
#include "SceneGame.h"
#include "Player.h"


SceneGame::SceneGame()
	:Scene(SceneIds::Game)
{
}

void SceneGame::Init()
{
	
	player = new Player();
	AddGameObject(player);
	

	Scene::Init();
}

void SceneGame::Enter()
{
	auto size = FRAMEWORK.GetWindowSizeF();
	sf::Vector2f center{ size.x * 0.5f, size.y * 0.5f };
	uiView.setSize(size);
	uiView.setCenter(center);
	worldView.setSize(size);
	worldView.setCenter({ 0.f, 0.f });
	player->SetPosition({ -500.f, -300.f });

}



void SceneGame::Update(float dt)
{
	Scene::Update(dt);
} 