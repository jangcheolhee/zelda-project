#include "stdafx.h"
#include "SceneGame.h"
#include "Player.h"
#include "TileMap.h"
#include "BasicEnemy.h"
#include "Bush.h"
#include "Chest.h"
#include "Rupee.h"
#include "JumpWall.h"

SceneGame::SceneGame()
	:Scene(SceneIds::Game)
{
}

Enemy* SceneGame::CreateOrReuseEnemy(Enemy::Types type)
{
	auto& pool = enemyPools[type];
	if (!pool.empty())
	{
		Enemy* reused = pool.front().release();
		pool.pop_front();
		return reused;
	}

	Enemy* newEnemy = nullptr;
	switch (type)
	{
	case Enemy::Types::Basic:
		newEnemy = new BasicEnemy();
		break;

	default:
		break;
	}

	if (newEnemy != nullptr)
		newEnemy->Init();

	return newEnemy;
}


void SceneGame::RecycleEnemy(Enemy* enemy)
{
	if (enemy)
	{
		enemy->SetActive(false);
		enemyPools[enemy->GetType()].push_back(std::unique_ptr<Enemy>(enemy));
	}
}


void SceneGame::SpawnEnemy(sf::Vector2f pos, Enemy::Types type)
{
	Enemy* enemy = CreateOrReuseEnemy(type);
	enemy->SetInitPosition(pos);

	enemy->Reset();
	enemy->SetActive(false);

	AddGameObject(enemy);
	enemyList.push_back(enemy);
}

void SceneGame::SpawnEnemyAtTile(int layerIndex, int targetGid, Enemy::Types type)
{
	std::vector<sf::Vector2f> positions = tileMap->getPosition(layerIndex, targetGid);
	for (const auto& pos : positions)
	{
		SpawnEnemy(pos, type);
	}
}
void SceneGame::CheckCollison()
{
	for (auto& enemy : enemyList)
	{
		if (player->GetGlobalBounds().intersects(enemy->GetGlobalBounds()))
		{
			player->OnCollide(enemy);
			enemy->OnCollide(player);
		}
	}

	for (auto& obj : interactables)
	{
		if (player->GetGlobalBounds().intersects(obj->GetGlobalBounds()))
		{
			// 플레이어가 obj가 충돌한 방향으로는 움지길수 없게 하기
			switch (obj->GetType())
			{
			case Interactable::Type::Throw:
				if (player->WantsToInteract())
				{
					obj->OnInteract();
				}
				break;
			case Interactable::Type::Chest:
				if (player->WantsToInteract())
				{
					obj->OnInteract();
				}
				break;
			case Interactable::Type::Item:

				obj->OnInteract();

				break;
			case Interactable::Type::JumpWall:

				obj->OnInteract();

				break;
			}
			

		}
	}
}

// 🔸 Enemy 삭제 (→ 풀에 리사이클)
void SceneGame::DeleteEnemy()
{
	for (Enemy* e : enemyList)
	{
		RemoveGameObject(e);
		RecycleEnemy(e);
	}
	enemyList.clear();
}

void SceneGame::Init()
{

	texIds.push_back("graphics/Overworld.png");
	texIds.push_back("graphics/Enemy_sheet.png");
	//fontIds.push_back("fonts/DS-DIGIT.ttf");

	//ANI_CLIP_MGR.Load("animations/idle.csv");
	//ANI_CLIP_MGR.Load("animations/run.csv");
	//ANI_CLIP_MGR.Load("animations/jump.csv");


	player = new Player("Player");
	tileMap = new TileMap("TileMap");


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
	worldView.setSize({ size.x * .5f, size.y * .5f });
	worldView.setCenter({ 0.f,0.f });
	//SpawnEnemy({ 20,20 }, Enemy::Types::Basic);
	// Enmy
	// pos.x, pos,y
	// type 
	SpawnEnemyAtTile(2, 290, Enemy::Types::Basic);
	// I
	auto bush = new Bush();
	AddGameObject(bush);

	auto chest = new Chest();
	AddGameObject(chest);

	auto rupee = new Rupee();
	AddGameObject(rupee);
	auto wall = new JumpWall();
	AddGameObject(wall);
	interactables.push_back(bush); // 따로 관리
	interactables.push_back(chest); // 따로 관리
	interactables.push_back(rupee); // 따로 관리
	interactables.push_back(wall); // 따로 관리
	Scene::Enter();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);
	CheckCollison();

}