#include "stdafx.h"
#include "SceneGameOver.h"
#include "GameOverUI.h"

SceneGameOver::SceneGameOver()
	:Scene(SceneIds::GameOver)
{
}

void SceneGameOver::Init()
{
	texIds.push_back("graphics/Items.png");

	fontIds.push_back("fonts/DS-DIGIT.ttf");
	

	
	AddGameObject(new GameOverUI("GameOverUI"));

	Scene::Init();
}

void SceneGameOver::Enter()
{
	auto size = FRAMEWORK.GetWindowSizeF();
	sf::Vector2f center{ size.x * 0.5f, size.y * 0.5f };
	uiView.setSize(size);
	uiView.setCenter(center);

	Scene::Enter();
}

void SceneGameOver::Update(float dt)
{
	Scene::Update(dt);
}
