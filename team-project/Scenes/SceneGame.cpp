#include "stdafx.h"
#include "SceneGame.h"
#include "Player.h"
#include "TileMap.h"
#include "BasicEnemy.h"
#include "Bush.h"
#include "Npc.h"
#include "Chest.h"
#include "Rupee.h"
#include "JumpWall.h"
#include <istream>

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

	if (newEnemy != nullptr) newEnemy->Init();

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
	std::vector<sf::Vector2f> positions = tileMap->getPositions(layerIndex, targetGid);
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
			player->SetMovable(false);
			// 플레이어가 obj가 충돌한 방향으로는 움지길수 없게 하기
			switch (obj->GetType())
			{
			case Interactable::Type::Throw: case Interactable::Type::Chest:
				if (player->WantsToInteract())
				{
					obj->OnInteract();
				}
				break;

			case Interactable::Type::Item: case Interactable::Type::JumpWall:

				obj->OnInteract();
				break;
			}
		}player->SetMovable(true);
	}
}

void SceneGame::SpawnBushesAtTile(int layerIndex, int targetGid, std::string name)
{
	std::vector <sf::Vector2f> positions = tileMap->getPositions(layerIndex, targetGid);

	for (const auto& pos : positions)
	{
		auto bush = new Bush;
		AddGameObject(bush);
		interactables.push_back(bush);
		bush->Reset();
		bush->SetPosition(pos);
		if (name == "bush")
		{
			bush->SetOrigin(Origins::ML);
		}
		else if (name == "hole")
		{
			bush->SetOrigin(Origins::TL);
		}
	}
}

void SceneGame::SpawnJumpAtTile(int layerIndex, int targetGid)
{
	std::vector <sf::Vector2f> positions = tileMap->getPositions(layerIndex, targetGid);

	for (const auto& pos : positions)
	{
		auto inter = new JumpWall();
		
		switch (targetGid)
		{
		case 25075:
			inter->SetDirection(Direction::Down);
			break;
		default:
			break;
		}
		AddGameObject(inter);
		interactables.push_back(inter);
		inter->SetOrigin(Origins::TC);
		inter->SetPosition(pos);
	}
}

void SceneGame::SpawnNpcAtTile(int layerIndex, int targetGid)
{
    std::vector <sf::Vector2f> positions = tileMap->getPositions(layerIndex, targetGid);

    for (const auto& pos : positions)
    {
        auto npc = new Npc;
        AddGameObject(npc);
        interactables.push_back(npc);
        npc->SetScale({ 0.5f, 0.5f });
        npc->SetPosition(pos);
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
	texIds.push_back("graphics/sprite_sheet.png");
	texIds.push_back("graphics/bush.png");
	texIds.push_back("graphics/Overworld.png");
    texIds.push_back("graphics/npc.png");
	texIds.push_back("graphics/Enemy_sheet.png");
	//fontIds.push_back("fonts/DS-DIGIT.ttf");
	//ANI_CLIP_MGR.Load("animations/idle.csv");
	//ANI_CLIP_MGR.Load("animations/run.csv");
	//ANI_CLIP_MGR.Load("animations/jump.csv");

	player = new Player("Player");
	tileMap = new TileMap("TileMap");
    player->Init();
    // 3) 타일맵도 만들고 Init()
    tileMap = new TileMap("TileMap");
    tileMap->Init();

	AddGameObject(player);
	AddGameObject(tileMap);

	Scene::Init();
}

void SceneGame::Enter()
{
    player->Reset();
	auto size = FRAMEWORK.GetWindowSizeF();
	sf::Vector2f center{ size.x * 0.5f, size.y * 0.5f };
	uiView.setSize(size);
	uiView.setCenter(center);
	worldView.setSize({ size.x * .5f, size.y * .5f });
	worldView.setCenter(player->GetGlobalBounds().getPosition());
	sf::Vector2f startPos = tileMap->getPosition(2, 18585);

	SpawnBushesAtTile(1, 24670, "bush");
    SpawnBushesAtTile(1, 24590, "hole"); 
    SpawnNpcAtTile(2, 24638); 
	SpawnJumpAtTile(3, 25075);

	Scene::Enter();
	player->SetPosition(startPos);
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);
	worldView.setCenter(player->GetGlobalBounds().getPosition());
	CheckCollison();
}