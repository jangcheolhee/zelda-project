#include "stdafx.h"
#include "SceneTest.h"
#include "Player.h"
SceneTest::SceneTest()
	:Scene(SceneIds::Test)
{
}
void SceneTest::Release()
{
	Scene::Release();
}

void SceneTest::Init()
{
	player = new Player();
	AddGameObject(player);
	Scene::Init();
}

void SceneTest::Enter()
{
	auto size = FRAMEWORK.GetWindowSizeF();
	sf::Vector2f center{ size.x * 0.5f, size.y * 0.5f };
	uiView.setSize(size);
	uiView.setCenter(center);
	worldView.setSize({ size.x * .5f, size.y * .5f });
	worldView.setCenter(player->GetGlobalBounds().getPosition());
	
	Scene::Enter();
	player->SetPosition({ 0, 0 });
}

void SceneTest::Update(float dt)
{
	Scene::Update(dt);
}
