#include "stdafx.h"
#include "SceneCastle.h"
#include "Player.h"
#include "TileMap.h"
#include "HitboxGenerator.h"
#include "JumpWall.h"
#include "BasicEnemy.h"
#include "Enemy.h"
#include "HUD.h"
SceneCastle::SceneCastle() :Scene(SceneIds::Castle)
{
	texIds.push_back("graphics/HUD.png");
	texIds.push_back("graphics/inventory.png");
	fontIds.push_back("fonts/Neo.ttf");
	player = nullptr;
	tileMapCastle = nullptr;
}

void SceneCastle::InitZones()
{
	castleZones.clear();

	castleZones.push_back({
		sf::FloatRect(-400, -230, 515, 464),
		1,
		[this]()
		  {
			  std::cout << "Zone 1 Enter" << std::endl;
			  SpawnEnemyAtTile(4, 8136, Enemy::Types::Basic);
		  },
		[this]()
		  {
			  std::cout << "Zone 1 Exit" << std::endl;
			  DeleteEnemy();
		  },
		false
		});
	castleZones.push_back({
		sf::FloatRect(188, -230, 242, 464),
		2,
		[this]()
		{
			std::cout << "Zone 2 Enter" << std::endl;
			SpawnEnemyAtTile(4, 8136, Enemy::Types::Basic);
		},
		[this]()
		{
			std::cout << "Zone 2 Exit" << std::endl;
			DeleteEnemy();
		},
		false
		});
	castleZones.push_back({
		sf::FloatRect(-584, -230, 363, 464),
		3,
		[this]()
		{
			std::cout << "Zone 3 Enter" << std::endl;
			SpawnEnemyAtTile(4, 8136, Enemy::Types::Basic);
		},
		[this]()
		{
			std::cout << "Zone 3 Exit" << std::endl;
			DeleteEnemy();
		},
		false
		});
}

void SceneCastle::UpdateZones()
{
	if (!player || castleZones.empty()) return;

	sf::Vector2f playerPos = player->GetGlobalBounds().getPosition();
	for (auto& zone : castleZones)
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
				SpawnSquareHitBox();
				SpawnFloorCovers();
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

void SceneCastle::UpdateBehaviorZone(float dt)
{
	float x = Utils::Clamp(player->GetGlobalBounds().getPosition().x, castleZones[zoneID - 1].bounds.left + worldView.getSize().x / 2, castleZones[zoneID - 1].bounds.left + castleZones[0].bounds.width - worldView.getSize().x / 2);
	float y = Utils::Clamp(player->GetGlobalBounds().getPosition().y, castleZones[zoneID - 1].bounds.top + worldView.getSize().y / 2, castleZones[zoneID - 1].bounds.top + castleZones[0].bounds.height - worldView.getSize().y / 2);
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
}

void SceneCastle::CheckCollison()
{
	if (!player) return;

	for (auto& obj : interactList)
	{
		if (Utils::CheckCollision(player->GetHitBox().rect, obj->GetHitBox().rect))
		{
			switch (obj->GetType())
			{
			case Interactable::Type::Throw:
			case Interactable::Type::Chest:
				if (player->WantsToInteract() && !player->IsInteract())
				{
					obj->OnInteract();
				}
				break;

			case Interactable::Type::Heart:
			case Interactable::Type::Rupee:
				obj->OnInteract();
				break;

			case Interactable::Type::JumpWall:
				player->SetPosition(player->GetPos());
				obj->OnInteract();
				break;
			}
		}
	}
}

void SceneCastle::SpawnSquareHitBox()
{
	HitboxGenerator::SpawnSquareHitBox(
		tileMapCastle,
		collisions,
		collisionBox,
		"Castle"
	);

	for (const auto& hitbox : collisions)
	{
		sf::FloatRect rect = hitbox.rect.getGlobalBounds();
		wallX = rect.left;
		wallY = rect.top;
		wallWithdh = rect.width;
		wallHeight = rect.height;

		Interactable* inter = nullptr;
		auto& pool = interactPool[Interactable::Type::JumpWall];
		if (!pool.empty())
		{
			inter = pool.front();
			pool.pop_front();
		}
		else inter = (Interactable*)AddGameObject(new JumpWall());
		inter->Init();
		if (dynamic_cast<JumpWall*>(inter))
		{
			dynamic_cast<JumpWall*>(inter)->SetBounds(wallX, wallY, wallWithdh, wallHeight);
		}
		inter->Reset();
		inter->SetActive(true);
		inter->SetPosition({ wallX,wallY });
		interactList.push_back(inter);
	}
}

void SceneCastle::SpawnFloorCovers()
{
	//floor 1 Door Path
	std::vector<sf::Vector2f> positions = tileMapCastle->getPositions(2,8125);
	for (const auto& pos : positions)
	{
		auto floor1DoorPathCover = new SpriteGo();
		floor1DoorPathCover->SetName("floor1DoorPathCovers");
		floor1DoorPathCover->Init();

		floor1DoorPathCover->GetSprite().setTexture(TEXTURE_MGR.Get("data/59984.png"));
		floor1DoorPathCover->GetSprite().setTextureRect({ 192, 216, 96, 32 });

		floor1DoorPathCover->SetActive(1);
		floor1DoorPathCover->SetOrigin(Origins::TL);
		floor1DoorPathCover->SetPosition(pos);
		AddGameObject(floor1DoorPathCover);
		floor1DoorPathCovers.push_back(floor1DoorPathCover);
	}

	//floor 2 Door Path
	sf::Vector2f floor2DoorPathPos = tileMapCastle->getPosition(2, 8205);

	auto floor2DoorPathCover = new SpriteGo();
	floor2DoorPathCover->SetName("floor1DoorPathCovers");
	floor2DoorPathCover->Init();

	floor2DoorPathCover->GetSprite().setTexture(TEXTURE_MGR.Get("data/59984.png"));
	floor2DoorPathCover->GetSprite().setTextureRect({ 216, 88, 48, 32 });

	floor2DoorPathCover->SetActive(1);
	floor2DoorPathCover->SetOrigin(Origins::TL);
	floor2DoorPathCover->SetPosition(floor2DoorPathPos);
	AddGameObject(floor2DoorPathCover);

	//second floor
	if (!LeftBridge)
	{
		//left Bridge
		sf::Vector2f LeftBridgePos = tileMapCastle->getPosition(3, 8354);

		LeftBridge = new SpriteGo();
		LeftBridge->SetName("LeftBridge");
		LeftBridge->Init();

		LeftBridge->GetSprite().setTexture(TEXTURE_MGR.Get("data/59984.png"));
		LeftBridge->GetSprite().setTextureRect({ 88, 88, 49, 64 });

		LeftBridge->SetActive(!isSecondFloor);
		LeftBridge->SetOrigin(Origins::TL);
		LeftBridge->SetPosition(LeftBridgePos);
		AddGameObject(LeftBridge);
	}
	//right Bridge
	if (!RightBridge)
	{
		sf::Vector2f RightBridgePos = tileMapCastle->getPosition(3, 8329);

		RightBridge = new SpriteGo();
		RightBridge->SetName("RightBridge");
		RightBridge->Init();

		RightBridge->GetSprite().setTexture(TEXTURE_MGR.Get("data/59984.png"));
		RightBridge->GetSprite().setTextureRect({ 928, 120, 64, 152 });

		RightBridge->SetActive(!isSecondFloor);
		RightBridge->SetOrigin(Origins::TL);
		RightBridge->SetPosition(RightBridgePos);
		AddGameObject(RightBridge);
	}
}

void SceneCastle::DeleteInteractables()
{
	auto it = interactables.begin();
	while (it != interactables.end())
	{
		RemoveGameObject(*it);
		it = interactables.erase(it);
	}
	interactables.clear();
}

void SceneCastle::RecycleEnemy(Enemy* enemy)
{
	if (enemy)
	{
		enemy->SetActive(false);
		enemyPools[enemy->GetType()].push_back(enemy);
	}
}

void SceneCastle::SpawnEnemy(sf::Vector2f pos, Enemy::Types type)
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

void SceneCastle::SpawnEnemyAtTile(int layerIndex, int targetGid, Enemy::Types type)
{
	std::vector<sf::Vector2f> positions = tileMapCastle->getPositions(layerIndex, targetGid);
	for (const auto& pos : positions)
	{
		SpawnEnemy(pos, type);
	}
}

void SceneCastle::DeleteEnemy()
{
	for (Enemy* e : enemyList)
	{
		RecycleEnemy(e);
	}
	enemyList.clear();
}

void SceneCastle::Init()
{
	texIds.push_back("graphics/Enemy_sheet.png");

	soundIds.push_back("effects/link hurt.wav");
	soundIds.push_back("effects/throw.wav");
	soundIds.push_back("effects/rupee.wav");
	soundIds.push_back("effects/heart.wav");
	soundIds.push_back("effects/enemy hit.wav");
	soundIds.push_back("effects/link dies.wav");
	soundIds.push_back("effects/sword.wav");
	texIds.push_back("data/59984.png");

	player = new Player("Player");
	tileMapCastle = new TileMap("TileMapCastle", "data/castleInner.tmj");
	tileMapCastle->Init();

	hud = new HUD("HUD");
	hud->Init();
	AddGameObject(hud);

	if (FindGameObject("InventoryUI") == nullptr)
	{
		inventoryUI = new InventoryUI("InventoryUI");
		inventoryUI->Init();
		AddGameObject(inventoryUI);
	}
	AddGameObject(player);
	AddGameObject(tileMapCastle);

	InitZones();

	std::vector<sf::Vector2f> fisrtPositions = tileMapCastle->getPositions(3, 8147);
	std::vector<sf::Vector2f> secondPositions = tileMapCastle->getPositions(3, 8182);
	for (const auto& fpos : fisrtPositions)
	{
		firstBound = sf::FloatRect(fpos.x - 16, fpos.y - 16, 32, 32);
		firstBounds.push_back(firstBound);
	}

	for (const auto& spos : secondPositions)
	{
		secondBound = sf::FloatRect(spos.x - 16, spos.y - 16, 32, 32);
		secondBounds.push_back(secondBound);
	}
	isSecondFloor = 1;

	endPos = tileMapCastle->getPosition(2, 8313);
	endHole = sf::FloatRect(endPos.x - 16, endPos.y - 16, 32, 32);

	

	Scene::Init();
}

void SceneCastle::Enter()
{
	player->Reset();
	auto size = FRAMEWORK.GetWindowSizeF();
	sf::Vector2f center{ size.x * 0.5f, size.y * 0.5f };
	uiView.setSize(size);
	uiView.setCenter(center);
	worldView.setSize({ size.x * .5f, size.y * .5f });
	if (inventoryUI)
	{
		inventoryUI->SetActive(false); // 처음 진입 시 숨김 처리
		inventoryUI->Reset();          // 필요 시 초기화 상태도 같이
	}

	Scene::Enter();

	sf::Vector2f startPos = tileMapCastle->getPosition(1, 7342);
	player->SetPosition({ startPos.x,startPos.y-30.f });
	GAME_MGR.playerHp = player->GetMaxHp();
	GAME_MGR.currentMapID = (int)SCENE_MGR.GetCurrentSceneId();
	GAME_MGR.playerSpawnPosition = startPos;

	GAME_MGR.Save();
	worldView.setCenter(player->GetGlobalBounds().getPosition());
}

void SceneCastle::Update(float dt)
{
	Scene::Update(dt);

	CheckCollison();
	UpdateZones();
	UpdateBehaviorZone(dt);

	for (const auto& fB : firstBounds)
	{
		if (fB.contains(player->GetGlobalBounds().getPosition()))
		{
			isSecondFloor = 0;
			LeftBridge->SetActive(1);
			RightBridge->SetActive(1);
		}
	}
	for (const auto& sB : secondBounds)
	{
		if (sB.contains(player->GetGlobalBounds().getPosition()))
		{
			isSecondFloor = 1;
			if (LeftBridge) LeftBridge->SetActive(0);
			if (RightBridge) RightBridge->SetActive(0);
		}
	}
	if (endHole.contains(player->GetGlobalBounds().getPosition()))
	{
		std::cout << "Boss" << std::endl;
		SCENE_MGR.ChangeScene(SceneIds::Boss);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::F1))
	{
		SCENE_MGR.ChangeScene(SceneIds::Game);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::F2))
	{
		SCENE_MGR.ChangeScene(SceneIds::Hidden);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::F3))
	{
		SCENE_MGR.ChangeScene(SceneIds::Castle);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::F4))
	{
		SCENE_MGR.ChangeScene(SceneIds::Boss);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::F5))
	{
		std::cout << "PlayerPosition" << player->GetPosition().x << ", " << player->GetPosition().y << ")" << std::endl;
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Tab))
	{
		if (inventoryUI)
		{
			bool active = inventoryUI->GetActive();
			inventoryUI->SetActive(!active);
		}
	}
}

void SceneCastle::Draw(sf::RenderWindow& window)
{
	window.setView(worldView);
	Scene::Draw(window);

	for (auto& col : collisions)
	{
		col.Draw(window);
	}
	
}