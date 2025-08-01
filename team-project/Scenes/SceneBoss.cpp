#include "stdafx.h"
#include "SceneBoss.h"
#include "Player.h"
#include "BossEnemy.h"
SceneBoss::SceneBoss()
	:Scene(SceneIds::Boss)
{
}

void SceneBoss::Init()
{
	texIds.push_back("graphics/Boss.png");

	player = new Player("Player");
	AddGameObject(player);
	

	Scene::Init();
}

void SceneBoss::Enter()
{
	auto size = FRAMEWORK.GetWindowSizeF();
	worldView.setSize({ size.x * .5f, size.y * .5f });
	worldView.setCenter({ 0,0 });
	points.resize(6);
	starts.resize(6);
	points.clear();
	starts.clear();

	starts.push_back({ -75,-75 });
	starts.push_back({ 0.f,-75 });
	starts.push_back({ 75,-75 });
	starts.push_back({ 75,0.f });
	starts.push_back({ -75,0.f });
	starts.push_back({ 0.f,0.f });
	
	points.push_back({ 0.f,-120 });
	points.push_back({ 75,-75 });
	points.push_back({ 75,0.f });
	points.push_back({ 0.f,45.f });
	points.push_back({ -75,-75 });
	points.push_back({ -75,0.f });
	for (int i = 0; i < 6; i++)
	{
		BossEnemy* b = new BossEnemy();
		AddGameObject(b);
		bosses.push_back(b);
		b->StartPos(starts[i]);
		b->DesPos(points[i]);
	
	}
	Scene::Enter();
	player->SetPosition({ 100,100 });
}

void SceneBoss::Update(float dt)
{
	Scene::Update(dt);
	
}
