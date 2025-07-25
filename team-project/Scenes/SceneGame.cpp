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

void SceneGame::InitZones()
{
	mapZones.clear();

	mapZones.push_back({
	  //zone1 origin
	  sf::FloatRect(0, 200, 512, 550), //zone 1_confirm
	  1,
	  [this]()
		{
			std::cout << "Zone 1 Enter" << std::endl;
		},
	  [this]()
		{
			std::cout << "Zone 1 Exit" << std::endl;
		},
	  false
		});

	// Zone 2 origin_left1
	mapZones.push_back({
		sf::FloatRect(0, 200, 512, 550), 
		2,
		[this]()
		{
			std::cout << "Zone 2 Enter" << std::endl;
		},
		[this]()
		{
			std::cout << "Zone 2 Exit" << std::endl;
			DeleteEnemy();
		},
		false
		});

	// Zone 3 origin_up1
	mapZones.push_back({
		sf::FloatRect(0, 200, 512, 550), 
		3,
		[this]()
		{
			std::cout << "Zone 3 Enter" << std::endl;
		},
		[this]()
		{
			std::cout << "Zone 3 Exit" << std::endl;
			DeleteEnemy();
		},
		false
		});
}

void SceneGame::UpdateZones()
{
		sf::Vector2f playerPos = player->GetGlobalBounds().getPosition();
		for (auto& zone : mapZones)
		{
			bool nowInZone = zone.bounds.contains(playerPos);
			if (nowInZone && !zone.entered)
			{
				zone.entered = true; 
				zoneID = zone.zoneId; 
				if (zone.onEnter)
				{
					zone.onEnter();
					SpawnInteractableObject(zone.bounds);
				}
			}
			else if (!nowInZone && zone.entered)
			{
				zone.entered = false;
				if (zone.onExit) zone.onExit();
				DeleteInteractables();
			}
		}
}

void SceneGame::UpdateBehaviorZone()
{
	switch (zoneID)
	{//player 기준
		case 1:
		{
			float x = Utils::Clamp(player->GetGlobalBounds().getPosition().x, 128, 384);
			float y = Utils::Clamp(player->GetGlobalBounds().getPosition().y, 384, 768);
			worldView.setCenter({ x, y });
			break;
		}
		case 2:
		{
			float x = Utils::Clamp(player->GetGlobalBounds().getPosition().x, 0, 128);
			float y = Utils::Clamp(player->GetGlobalBounds().getPosition().y, 384, 768);
			worldView.setCenter({ x, y });
			break;
		}
		case 3:
		{
			float x = Utils::Clamp(player->GetGlobalBounds().getPosition().x, 0, 128);
			float y = Utils::Clamp(player->GetGlobalBounds().getPosition().y, 0, 384);
			worldView.setCenter({ x, y });
			break;
		}
	}
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
			// 플레이어가 obj가 충돌한 방향으로는 움직일 수 없게 하기
			switch (obj->GetType())
			{
			case Interactable::Type::Throw: case Interactable::Type::Chest:
				if (player->WantsToInteract() && !player->IsInteract())
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

// Scene 종료시 Interatables 비우거나 pool로 변경하거나 하는 수정 필요
void SceneGame::SpawnInteractableObject(sf::FloatRect zone)
{	
	//layer 1 : bush
	int layer1Gid[] = { 24670, 24590 };
	for (int id : layer1Gid)
	{
		std::vector <sf::Vector2f> positions = tileMap->getPositions(1, id);

		for (const auto& pos : positions)
		{	
			if((pos.x>=zone.left && pos.x<=(zone.left+zone.width))&&(pos.y>=zone.top&&pos.y<=zone.top+zone.height))
			{//bush
				auto bush = new Bush;
				std::cout << "spawnZone2test" << pos.x << std::endl;
				AddGameObject(bush);
				interactables.push_back(bush);
				bush->Reset();
				bush->SetPosition(pos);
				if (id == 24670)
				{
					std::cout << "spawnZone2test" << pos.x << std::endl;
					bush->SetOrigin(Origins::ML);
				}
				else if (id == 24590) //hole
				{
					bush->SetOrigin(Origins::TL);
				}
			}
		}
	}
	
	//layer 2 : npc
	int layer2Gid[] = { 24638 };
	for (int id : layer2Gid)
	{	
		std::vector <sf::Vector2f> positions = tileMap->getPositions(2, id);
		for (const auto& pos : positions)
		{//npc
			if ((pos.x >= zone.left && pos.x <= (zone.left + zone.width)) && (pos.y >= zone.top && pos.y <= zone.top + zone.height))
			{
				auto npc = new Npc();
				AddGameObject(npc);
				interactables.push_back(npc);
				npc->SetScale({ 0.5f, 0.5f });
				npc->Reset();
				npc->SetPosition(pos);
			}
		}
	}	
	
	//layer3: collision
	int gid[] = { 25075,25067,24699,25068,24592 };
	for (int id : gid)
	{
		std::vector <sf::Vector2f> positions = tileMap->getPositions(3, id);
		for (const auto& pos : positions)
		{
			if ((pos.x >= zone.left && pos.x <= (zone.left + zone.width)) && (pos.y >= zone.top && pos.y <= zone.top + zone.height))
			{
				auto inter = new JumpWall();
				switch (id)
				{
				case 25075:
					inter->SetDirection(Direction::Down);
					break;
				case 25067:
					inter->SetDirection(Direction::Up);
					break;
				case 24699:
					inter->SetDirection(Direction::Left);
					break;
				case 25068: case 24592:
					inter->SetDirection(Direction::None);
					break;
				default:
					break;
				}
				AddGameObject(inter);
				interactables.push_back(inter);
				inter->SetOrigin(Origins::TC);
				inter->Reset();
				inter->SetPosition(pos);
			}
		}
	}
}

void SceneGame::DeleteInteractables()
{
	auto it = interactables.begin();
	while (it != interactables.end())
	{
		RemoveGameObject(*it);
		it = interactables.erase(it);
	}
	interactables.clear();
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

	InitZones();

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
	Scene::Enter();
	player->SetPosition(startPos);
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);
	
	CheckCollison();
	UpdateZones();
	UpdateBehaviorZone();
	if (InputMgr::GetKeyDown(sf::Keyboard::F1))
	{
		SCENE_MGR.ChangeScene(SceneIds::Game);
	}
}