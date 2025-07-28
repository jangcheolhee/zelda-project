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
#include "Heart.h"
#include "HUD.h"
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
			sf::Vector2f enemyPos = tileMapGame->getPosition(2, 18585);
			SpawnEnemy(enemyPos, Enemy::Types::Basic);

		},
	  [this]()
		{
			std::cout << "Zone 1 Exit" << std::endl;
			DeleteInteractables();

		},
	  false
		});
	// Zone 2 origin_left1
	mapZones.push_back({
		sf::FloatRect(-512, 200, 512, 550), 
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
		sf::FloatRect(0, -350, 512, 550), 
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
	// Zone 4 origin_up2
	mapZones.push_back({
		sf::FloatRect(0, -900, 512, 550),
		4,
		[this]()
		{
			std::cout << "Zone 4 Enter" << std::endl;
		},
		[this]()
		{
			std::cout << "Zone 4 Exit" << std::endl;
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
					SpawnFlowers(zone.bounds);
					JumpWall* wall = new JumpWall();
					wall->SetBounds(185, 560, 100, 100);
					AddGameObject(wall);
					interactables.push_back(wall);
					wall->Reset();
				}
			}
			else if (!nowInZone && zone.entered)
			{
				zone.entered = false;
				if (zone.onExit) zone.onExit();
				DeleteInteractables();
				for (auto f : flowers)
				{
					RemoveGameObject(f);
				}
				flowers.clear();
			}
		}
}

void SceneGame::UpdateBehaviorZone()
{
	switch (zoneID)
	{//player 기준
		case 1:
		{
			float x = Utils::Clamp(player->GetGlobalBounds().getPosition().x, 128,384);
			float y = Utils::Clamp(player->GetGlobalBounds().getPosition().y, 384, 566);
			worldView.setCenter({ x, y });
		}
			break;
		case 2:
		{
			float x = Utils::Clamp(player->GetGlobalBounds().getPosition().x, -384, -128);
			float y = Utils::Clamp(player->GetGlobalBounds().getPosition().y, 384, 566);
			worldView.setCenter({ x, y });
		}
			break;
		case 3:
		{
			float x = Utils::Clamp(player->GetGlobalBounds().getPosition().x, 128, 384);
			float y = Utils::Clamp(player->GetGlobalBounds().getPosition().y, -250, 100);
			worldView.setCenter({ x, y });
		}
			break;
		case 4:
		{
			float x = Utils::Clamp(player->GetGlobalBounds().getPosition().x, 128, 384);
			float y = Utils::Clamp(player->GetGlobalBounds().getPosition().y, -750, -400);
			worldView.setCenter({ x, y });
		}
		break;
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

	enemy->Init();
	enemy->SetInitPosition(pos);
	enemy->Reset();
	enemy->SetActive(true);

	AddGameObject(enemy);
	enemyList.push_back(enemy);
}

void SceneGame::SpawnEnemyAtTile(int layerIndex, int targetGid, Enemy::Types type)
{
	std::vector<sf::Vector2f> positions = tileMapGame->getPositions(layerIndex, targetGid);
	for (const auto& pos : positions)
	{
		SpawnEnemy(pos, type);
	}
}

void SceneGame::SpawnFlowers(sf::FloatRect zone)
{
	std::vector<sf::Vector2f> positions = tileMapGame->getPositions(4, 24696);
	for (const auto& pos : positions)
	{
		if (zone.contains(pos))
		{
			auto flower = new SpriteGo();
			flower->SetName("Flower");
			flower->Init();

			flower->GetSprite().setTexture(TEXTURE_MGR.Get("graphics/Overworld.png"));
			flower->GetSprite().setTextureRect({ 760, 41, 8, 8 }); 

			flower->SetActive(flowerBool);
			flower->SetOrigin(Origins::TL);
			flower->SetPosition(pos);
			AddGameObject(flower);
			flowers.push_back(flower);

		}
	}
}

void SceneGame::FlowerBreath(float dt)
{
	flowerTimer += dt;
	if (flowerTimer >= flowerRate)
	{
		flowerBool = !flowerBool;
		for (auto& flower : flowers)
		{
			flower->SetActive(flowerBool);
		}
		flowerTimer = 0.f;
	}
}

void SceneGame::CheckCollison()
{
	for (auto& enemy : enemyList)
	{
		player->OnCollide(enemy);
		if (player->GetGlobalBounds().intersects(enemy->GetGlobalBounds()))
		{
			enemy->OnCollide(player);
		}
	}

	sf::FloatRect rect = player->GetGlobalBounds();
	rect.left -= 2.f;
	rect.top -= 2.f;
	rect.width += 4.f;
	rect.height += 4.f;
	for (auto& obj : interactables)
	{

		if (obj->GetActive())
		{
			if (obj->GetType() == Interactable::Type::Throw || obj->GetType() == Interactable::Type::Chest)
			{
				if (rect.intersects(obj->GetGlobalBounds()))
				{
					if (player->WantsToInteract() && !player->IsInteract())
					{
						int r = Utils::RandomRange(0, 3);

						switch (r)
						{
						case 0:
						{
							Rupee* rupee = new Rupee();
							rupee->SetPosition(obj->GetPosition());
							rupee->Reset();
							AddGameObject(rupee);
							interactables.push_back(rupee);

							break;
						}

						case 1:
						{
							Heart* heart = new Heart();
							heart->SetPosition(obj->GetPosition());
							heart->Reset();
							AddGameObject(heart);
							interactables.push_back(heart);

							break;
						}

						}
						obj->OnInteract();
					}

				}
			}

			if (player->GetGlobalBounds().intersects(obj->GetGlobalBounds()))
			{
				if (obj->GetType() == Interactable::Type::Item)
				{
					obj->OnInteract();

					continue;
				}
				player->SetPosition(player->GetPos());
				if (obj->GetType() == Interactable::Type::Chest || obj->GetType() == Interactable::Type::JumpWall)
				{

					obj->OnInteract();
				}
				

			}
		}
		}
		
}

// Scene 종료시 Interatables 비우거나 pool로 변경하거나 하는 수정 필요
void SceneGame::SpawnInteractableObject(sf::FloatRect zone)
{	
	//layer 1 : bush
	int layer1Gid[] = { 24670, 24590 };
	for (int id : layer1Gid)
	{
		std::vector <sf::Vector2f> positions = tileMapGame->getPositions(1, id);
		for (const auto& pos : positions)
		{	
			if((pos.x>=zone.left && pos.x<=(zone.left+zone.width))&&(pos.y>=zone.top&&pos.y<=zone.top+zone.height))
			{//bush
				auto bush = new Bush;
				AddGameObject(bush);
				interactables.push_back(bush);
				bush->Reset();
				bush->SetPosition(pos);
				if (id == 24670)
				{
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
		std::vector <sf::Vector2f> positions = tileMapGame->getPositions(2, id);
		for (const auto& pos : positions)
		{//npc
			if ((pos.x >= zone.left && pos.x <= (zone.left + zone.width)) && (pos.y >= zone.top && pos.y <= zone.top + zone.height))
			{
				auto npc = new Npc();
				npc->SetPlayer(player);
				AddGameObject(npc);
				interactables.push_back(npc);
				npc->SetScale({ 0.5f, 0.5f });
				npc->Reset();
				npc->SetPosition(pos);
			}
		}
	}	
	
	//layer3: collision
	/*int gid[] = { 25075,25067,24699,25068,24592 };
	for (int id : gid)
	{
		std::vector <sf::Vector2f> positions = tileMapGame->getPositions(3, id);
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
	}*/
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
	texIds.push_back("graphics/Items.png");
	texIds.push_back("graphics/HUD.png");
	texIds.push_back("graphics/flower.png");
	//fontIds.push_back("fonts/DS-DIGIT.ttf");
	//ANI_CLIP_MGR.Load("animations/idle.csv");
	//ANI_CLIP_MGR.Load("animations/run.csv");
	//ANI_CLIP_MGR.Load("animations/jump.csv");

	player = new Player("Player");
	// 3) 타일맵도 만들고 Init()
	tileMapGame = new TileMap("TileMap", "data/originalMap.tmj");
	tileMapGame->Init();
	AddGameObject(new HUD());
	
	AddGameObject(player);
	AddGameObject(tileMapGame);

	InitZones();
	
	endPos = tileMapGame->getPosition(4, 24590);
	endHole = sf::FloatRect(endPos.x - 16, endPos.y - 16, 32, 32);

	flowerTimer = 0.f;
	flowerBool = true;

	Scene::Init();
}

void SceneGame::Exit()
{
	GAME_MGR.SetPlayerData(player->GetHp(), player->GetPosition());
	//GAME_MGR.SaveGame("data/data.json");
	Scene::Exit();
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
	sf::Vector2f startPos = tileMapGame->getPosition(2, 18585);
	Scene::Enter();
	player->SetPosition(startPos);
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);
	
	CheckCollison();
	UpdateZones();
	UpdateBehaviorZone();
	FlowerBreath(dt);
	if (endHole.contains(player->GetGlobalBounds().getPosition()))
	{
		std::cout << "Hidden" << std::endl;
		SCENE_MGR.ChangeScene(SceneIds::Hidden);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::F2))
	{
		std::cout << "Hidden" << std::endl;
		SCENE_MGR.ChangeScene(SceneIds::Hidden);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::F1))
	{
		SCENE_MGR.ChangeScene(SceneIds::Game);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::F3))
	{
		std::cout << "PlayerPosition" << player->GetPosition().x << ", " << player->GetPosition().y << ")" << std::endl;
	}
}