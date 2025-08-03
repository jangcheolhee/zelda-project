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
	
	soundIds.push_back("bgm/ending.flac");
	soundIds.push_back("effects/cursor.wav");
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
	SOUND_MGR.PlayBgm(SOUNDBUFFER_MGR.Get("bgm/ending.flac"));
}

void SceneGameOver::Update(float dt)
{
	Scene::Update(dt);
}
