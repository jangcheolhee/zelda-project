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
#include "HitboxGenerator.h"
#include <istream>
#include "InventoryUI.h"


SceneGame::SceneGame()
	:Scene(SceneIds::Game)
{
}

void SceneGame::InitZones()
{
	mapZones.clear();

	mapZones.push_back({
		//zone1 origin
		sf::FloatRect(0, 260, 511, 499), //zone 1_confirm
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
			  DeleteEnemy();
		  },
		false
		});
	// Zone 2 origin_left1
	mapZones.push_back({
		sf::FloatRect(-512, 260, 511,499),
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
		sf::FloatRect(0, -240, 511, 499),
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
		sf::FloatRect(0, -740, 511, 499),
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
				zone.entered = true;
				zoneID = zone.zoneId;
				changeZone = true;
				zone.onEnter();
				SpawnInteractableObject(zone.bounds);
				SpawnFlowers(zone.bounds);
				SpawnSquareHitBox();

			}
		}
		else if (!nowInZone && zone.entered)
		{
			zone.entered = false;
			if (zone.onExit)
			{
				zone.onExit();
			}
			DeleteInteractables();

			for (auto f : flowers)
			{
				RemoveGameObject(f);
			}
			flowers.clear();
		}
	}
}
void SceneGame::UpdateBehaviorZone(float dt)
{
	float x = Utils::Clamp(player->GetGlobalBounds().getPosition().x, mapZones[zoneID - 1].bounds.left + worldView.getSize().x / 2, mapZones[zoneID - 1].bounds.left + mapZones[0].bounds.width - worldView.getSize().x / 2);
	float y = Utils::Clamp(player->GetGlobalBounds().getPosition().y, mapZones[zoneID - 1].bounds.top + worldView.getSize().y / 2, mapZones[zoneID - 1].bounds.top + mapZones[0].bounds.height - worldView.getSize().y / 2);
	if (changeZone)
	{
		worldView.setCenter(Utils::Lerp(worldView.getCenter(), { x,y }, dt * 2));
		if (Utils::Distance(worldView.getCenter(), { x,y }) > 1 && Utils::Distance(worldView.getCenter(), { x,y }) < 5)
		{
			//std::cout << Utils::Distance(worldView.getCenter(), { x,y }) << std::endl;
			changeZone = false;
		}
	}
	else
	{
		worldView.setCenter({ x, y });
	}
	//switch (zoneID)
	//{//player 기준
	//case 1:
	//break;
	//}
}

void SceneGame::DeleteInteractables()
{
	for (Interactable* inter : interactList)
	{
		inter->SetActive(false);
		interactPool[inter->GetType()].push_back(inter);
	}
	interactList.clear();
}
void SceneGame::RecycleEnemy(Enemy* enemy)
{
	if (enemy)
	{
		enemy->SetActive(false);
		enemyPools[enemy->GetType()].push_back(enemy);
	}
}

void SceneGame::SpawnEnemy(sf::Vector2f pos, Enemy::Types type)
{
	Enemy* enemy = nullptr;

	auto& pool = enemyPools[type];
	if (!pool.empty())
	{
		enemy = pool.front();
		pool.pop_front();
	}
	else
	{
		switch (type)
		{
		case Enemy::Types::Basic:
			enemy = (Enemy*)AddGameObject(new BasicEnemy());
			break;
		case Enemy::Types::Count:
			break;
		default:
			break;
		}
		enemy->Init();
	}
	enemy->Reset();
	enemy->SetPosition(pos);
	enemy->SetActive(true);

	enemyList.push_back(enemy);
}

void SceneGame::SpawnInteractable(sf::Vector2f pos, Interactable::Type type)
{
	Interactable* inter = nullptr;
	auto& pool = interactPool[type];
	if (!pool.empty())
	{
		inter = pool.front();
		pool.pop_front();
	}
	else
	{
		switch (type)
		{
		case Interactable::Type::None:
			break;
		case Interactable::Type::Throw:
			inter = (Bush*)AddGameObject(new Bush());
			break;
		case Interactable::Type::Chest:
			break;
		case Interactable::Type::JumpWall:
			inter = (JumpWall*)AddGameObject(new JumpWall());
			break;
		case Interactable::Type::Heart:
			inter = (Heart*)AddGameObject(new Heart());
			break;
		case Interactable::Type::Rupee:
			inter = (Rupee*)AddGameObject(new Rupee());
			break;
		case Interactable::Type::Npc:
			inter = (Npc*)AddGameObject(new Npc());
			break;
		default:
			break;
		}
		inter->Init();

	}
	if (dynamic_cast<JumpWall*>(inter))
	{
		dynamic_cast<JumpWall*>(inter)->SetBounds(wallX, wallY, wallWithdh, wallHeight);
	}
	inter->Reset();
	inter->SetActive(true);
	inter->SetPosition(pos);
	interactList.push_back(inter);

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
		if (player->GetGlobalBounds().intersects(enemy->GetGlobalBounds()))
		{
			player->OnCollide(enemy);
			enemy->OnCollide(player);
		}
	}


	for (auto& obj : interactList)
	{
		for (auto& enemy : enemyList)
		{
			if (Utils::CheckCollision(obj->GetHitBox().rect, enemy->GetBoundBox().rect))
			{
				//enemy->SetPosition(enemy->GetPos());

				sf::FloatRect objRect = obj->GetHitBox().rect.getGlobalBounds();
				sf::FloatRect enemyRect = enemy->GetBoundBox().rect.getGlobalBounds();

				float objX = objRect.left + objRect.width / 2.f;
				float objY = objRect.top + objRect.height / 2.f;

				float enemyX = enemyRect.left + enemyRect.width / 2.f;
				float enemyY = enemyRect.top + enemyRect.height / 2.f;

				float dx = objX - enemyX;
				float dy = objY - enemyY;

				float combinedHalfWidth = (objRect.width + enemyRect.width) / 2.f;
				float combinedHalfHeight = (objRect.height + enemyRect.height) / 2.f;

				float overlapX = combinedHalfWidth - std::abs(dx);
				float overlapY = combinedHalfHeight - std::abs(dy);

				if (overlapX < overlapY)
				{

					if (dx < 0)
					{
						//왼쪽
						enemy->OnCollide(Direction::Left);
					}

					else
					{
						enemy->OnCollide(Direction::Right);
						//오른쪽
					}
					if (dy < 0)
					{
						//왼쪽
						enemy->OnCollide(Direction::Up);
					}

					else
					{
						enemy->OnCollide(Direction::Down);
						//오른쪽
					}

				}
			}
		}

		if (dynamic_cast<Bush*> (obj))
		{
			if (Utils::CheckCollision(player->GetHitBox().rect, obj->GetBoundBox().rect))
			{
				if (player->WantsToInteract() && !player->IsInteract())
				{
					int r = Utils::RandomRange(0, 4);
					switch (r)
					{
					case 0:
					{
						SpawnInteractable(obj->GetPosition() + sf::Vector2f({ 3,-16 }), Interactable::Type::Rupee);
						break;
					}
					case 1:
					{
						SpawnInteractable(obj->GetPosition() + sf::Vector2f({ 4,-4 }), Interactable::Type::Heart);
						break;
					}
					}
					obj->OnInteract();
					continue;
				}
			}
		}

		if (Utils::CheckCollision(player->GetHitBox().rect, obj->GetHitBox().rect))
		{
			Rupee* rupee = dynamic_cast<Rupee*>(obj);
			if (rupee != nullptr)
			{
				obj->OnInteract();
				HUD* hud = dynamic_cast<HUD*>(FindGameObject("HUD")); // 또는 멤버 변수로 접근 가능
				if (hud != nullptr)
				{
					hud->AddRupee(1);  // 루피 1 증가
				}
				continue;
			}
			Heart* heart = dynamic_cast<Heart*>(obj);
			if (heart != nullptr)
			{
				obj->OnInteract();
				if (player != nullptr)
				{
					player->Heal(1); // ❤️ Player가 체력 회복
				}
				obj->OnInteract(); // 아이템 제거
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


void SceneGame::SpawnSquareHitBox()
{
	HitboxGenerator::SpawnSquareHitBox(
		tileMapGame,
		collisions,
		collisionBox
	);

	for (const auto& hitbox : collisions)
	{
		sf::FloatRect rect = hitbox.rect.getGlobalBounds();
		wallX = rect.left;
		wallY = rect.top;
		wallWithdh = rect.width;
		wallHeight = rect.height;

		SpawnInteractable(sf::Vector2f{ wallX, wallY }, Interactable::Type::JumpWall);
	}
}

void SceneGame::SpawnInteractableObject(sf::FloatRect zone)
{
	//layer 1 : bush
	int layer1Gid[] = { 24670, 24590 };
	for (int id : layer1Gid)
	{
		//layer 1 : bush
		std::vector <sf::Vector2f> positions = tileMapGame->getPositions(1, id);
		for (const auto& pos : positions)
		{
			if ((pos.x >= zone.left && pos.x <= (zone.left + zone.width)) && (pos.y >= zone.top && pos.y <= zone.top + zone.height))
			{//bush
				SpawnInteractable(pos, Interactable::Type::Throw);
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
				SpawnInteractable(pos, Interactable::Type::Npc);
			}
		}
	}
	//layer3: collision
	int gid[] = { 25075,25067,24699,25068,24592 };
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
				//AddGameObject(inter);
				//interactList.push_back(inter);
				//inter->SetOrigin(Origins::TC);
				//inter->Reset();
				//inter->SetPosition(pos);

				//HitBox hitbox;
				//sf::FloatRect collisionRect(pos.x - 8, pos.y - 8, 16, 16);
				//hitbox.UpdateTransformCollision(collisionBox, collisionRect, pos);
				//collisions.push_back(hitbox);
			}
		}
	}
}

void SceneGame::DeleteEnemy()
{
	for (Enemy* e : enemyList)
	{
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
	texIds.push_back("graphics/inventory.png");
	//texIds.push_back("graphics/Heart.png");
	texIds.push_back("graphics/Effects.png");
	texIds.push_back("graphics/Death.png");
	texIds.push_back("graphics/conversation.png");

	//fontIds.push_back("fonts/DS-DIGIT.ttf");
	ANI_CLIP_MGR.Load("animations/bush2.csv");
	ANI_CLIP_MGR.Load("animations/EnemyDeath.csv");
	//ANI_CLIP_MGR.Load("animations/run.csv");
	//ANI_CLIP_MGR.Load("animations/jump.csv");

	hud = new HUD("HUD");
	AddGameObject(hud);
	player = new Player("Player");
	player->SetHUD(hud);
	// 3) 타일맵도 만들고 Init()
	tileMapGame = new TileMap("TileMap", "data/originalMap.tmj");
	tileMapGame->Init();
	TEXTURE_MGR.Load("graphics/Heart.png");
	TEXTURE_MGR.Load("graphics/Heart_empty.png");

	inventoryUI = new InventoryUI();
	inventoryUI->Init();
	inventoryUI->sortingLayer = SortingLayers::UI;
	inventoryUI->sortingOrder = 10;//우선순위높음
	AddGameObject(inventoryUI);
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
	GAME_MGR.playerHp = player->GetHp();
	GAME_MGR.playerSpawnPosition = sf::Vector2f{ 0,0 };

	//GAME_MGR.SaveGame("data/data.json");
	for (Enemy* enemy : enemyList)
	{
		enemy->SetActive(false);
		enemyPools[enemy->GetType()].push_back(enemy);
	}
	enemyList.clear();
	Scene::Exit();
}

void SceneGame::Enter()
{
	auto size = FRAMEWORK.GetWindowSizeF();
	sf::Vector2f center{ size.x * 0.5f, size.y * 0.5f };
	uiView.setSize(size);
	uiView.setCenter(center);
	worldView.setSize({ size.x * .5f, size.y * .5f });
	worldView.setCenter(player->GetGlobalBounds().getPosition());
	sf::Vector2f startPos = tileMapGame->getPosition(2, 18585);
	Scene::Enter();
	GAME_MGR.playerHp = player->GetMaxHp();
	GAME_MGR.currentMapID = (int)SCENE_MGR.GetCurrentSceneId();
	GAME_MGR.playerSpawnPosition = startPos;
	GAME_MGR.Save();
	player->SetPosition(startPos);
}




void SceneGame::Update(float dt)
{
	Scene::Update(dt);
	auto it = enemyList.begin();
	while (it != enemyList.end())
	{
		if (!(*it)->GetActive())
		{
			RecycleEnemy(*it);
			it = enemyList.erase(it);
		}
		else
		{
			++it;
		}
	}
	auto it1 = interactList.begin();
	while (it1 != interactList.end())
	{
		if (!(*it1)->GetActive())
		{
			(*it1)->SetActive(false);
			interactPool[(*it1)->GetType()].push_back(*it1);
			it1 = interactList.erase(it1);
		}
		else
		{
			++it1;
		}
	}
	CheckCollison();
	UpdateZones();
	UpdateBehaviorZone(dt);
	FlowerBreath(dt);


	if (endHole.contains(player->GetGlobalBounds().getPosition()))
	{
		std::cout << "Hidden" << std::endl;
		SCENE_MGR.ChangeScene(SceneIds::Hidden);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Tab))
	{
		bool toggle = !inventoryUI->IsVisible();
		inventoryUI->SetActive(toggle);
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

void SceneGame::Draw(sf::RenderWindow& window)
{
	window.setView(window.getDefaultView());
	hud->Draw(window);
	// 💡 UI는 기본 뷰로 바꿔서 그리기
	if (inventoryUI->IsActive())
	{
		inventoryUI->Draw(window);
	}
	sf::View prev = window.getView();
	window.setView(prev); // 원래 뷰로 복구
	Scene::Draw(window);


}
