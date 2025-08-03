#include "stdafx.h"
#include "SceneCastle.h"
#include "Player.h"
#include "TileMap.h"
#include "HitboxGenerator.h"
#include "JumpWall.h"
#include "BasicEnemy.h"
#include "Enemy.h"
#include "HUD.h"
#include "InventoryUI.h"

SceneCastle::SceneCastle() :Scene(SceneIds::Castle)
{
	texIds.push_back("graphics/HUD.png");
	texIds.push_back("graphics/inventory.png");
	fontIds.push_back("fonts/Neo.ttf");
	player = nullptr;
	tileMapCastle = nullptr;
	hud = nullptr;
	inventoryUI = nullptr;
	floor2DoorPathCover = nullptr;
	LeftBridge = nullptr;
	RightBridge = nullptr;
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
			DeleteZoneSpecificObjects();
			SpawnEnemyAtTile(4, 8136, Enemy::Types::Basic);
		},
		[this]()
		{
			std::cout << "Zone 1 Exit" << std::endl;
			DeleteZoneSpecificObjects();
		},
		false
		});

	castleZones.push_back({
		sf::FloatRect(188, -230, 242, 464),
		2,
		[this]()
		{
			std::cout << "Zone 2 Enter" << std::endl;
			DeleteZoneSpecificObjects();
			SpawnEnemyAtTile(4, 8136, Enemy::Types::Basic);
		},
		[this]()
		{
			std::cout << "Zone 2 Exit" << std::endl;
			DeleteZoneSpecificObjects();
		},
		false
		});

	castleZones.push_back({
		sf::FloatRect(-584, -230, 363, 464),
		3,
		[this]()
		{
			std::cout << "Zone 3 Enter" << std::endl;
			DeleteZoneSpecificObjects();
			SpawnEnemyAtTile(4, 8136, Enemy::Types::Basic);
		},
		[this]()
		{
			std::cout << "Zone 3 Exit" << std::endl;
			DeleteZoneSpecificObjects();
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
			changeZone = true;
			if (zone.onEnter)
			{
				zone.onEnter();
			}
		}
		else if (!nowInZone && zone.entered)
		{
			zone.entered = false;
			if (zone.onExit)
			{
				zone.onExit();
			}
		}
	}
}

void SceneCastle::UpdateBehaviorZone(float dt)
{
	if (zoneID < 1 || zoneID > castleZones.size()) return;

	float x = Utils::Clamp(player->GetGlobalBounds().getPosition().x,
		castleZones[zoneID - 1].bounds.left + worldView.getSize().x / 2,
		castleZones[zoneID - 1].bounds.left + castleZones[zoneID - 1].bounds.width - worldView.getSize().x / 2);
	float y = Utils::Clamp(player->GetGlobalBounds().getPosition().y,
		castleZones[zoneID - 1].bounds.top + worldView.getSize().y / 2,
		castleZones[zoneID - 1].bounds.top + castleZones[zoneID - 1].bounds.height - worldView.getSize().y / 2);

	if (changeZone)
	{
		worldView.setCenter(Utils::Lerp(worldView.getCenter(), { x,y }, dt * 2));
		if (Utils::Distance(worldView.getCenter(), { x,y }) > 1 && Utils::Distance(worldView.getCenter(), { x,y }) < 5)
		{
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
	// 1층 문 경로 커버
	std::vector<sf::Vector2f> positions = tileMapCastle->getPositions(2, 8125);
	for (const auto& pos : positions)
	{
		auto floor1DoorPathCover = new SpriteGo();
		floor1DoorPathCover->SetName("floor1DoorPathCovers");
		floor1DoorPathCover->Init();

		floor1DoorPathCover->GetSprite().setTexture(TEXTURE_MGR.Get("data/59984.png"));
		floor1DoorPathCover->GetSprite().setTextureRect({ 192, 216, 96, 32 });

		floor1DoorPathCover->SetActive(true);
		floor1DoorPathCover->SetOrigin(Origins::TL);
		floor1DoorPathCover->SetPosition(pos);
		AddGameObject(floor1DoorPathCover);
		floor1DoorPathCovers.push_back(floor1DoorPathCover);
	}

	// 2층 문 경로
	sf::Vector2f floor2DoorPathPos = tileMapCastle->getPosition(2, 8205);

	if (!floor2DoorPathCover)
	{
		floor2DoorPathCover = new SpriteGo();
		floor2DoorPathCover->SetName("floor2DoorPathCover");
		floor2DoorPathCover->Init();

		floor2DoorPathCover->GetSprite().setTexture(TEXTURE_MGR.Get("data/59984.png"));
		floor2DoorPathCover->GetSprite().setTextureRect({ 216, 88, 48, 32 });

		floor2DoorPathCover->SetActive(true);
		floor2DoorPathCover->SetOrigin(Origins::TL);
		floor2DoorPathCover->SetPosition(floor2DoorPathPos);
		AddGameObject(floor2DoorPathCover);
	}

	// 2층 다리들
	if (!LeftBridge)
	{
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

void SceneCastle::DeleteHitboxes()
{
	collisions.clear();
}

void SceneCastle::DeleteInteractables()
{
	// interactList 정리 (JumpWall 포함)
	for (Interactable* inter : interactList)
	{
		inter->SetActive(false);
		interactPool[inter->GetType()].push_back(inter);
	}
	interactList.clear();

	// interactables 정리 (기존 코드 유지)
	auto it = interactables.begin();
	while (it != interactables.end())
	{
		RemoveGameObject(*it);
		it = interactables.erase(it);
	}
	interactables.clear();
}

void SceneCastle::DeleteZoneSpecificObjects()
{
	for (Enemy* e : enemyList)
	{
		RecycleEnemy(e);
	}
	enemyList.clear();
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
		if (enemy)
		{
			enemy->Init();
		}
	}

	if (enemy)
	{
		enemy->Reset();
		enemy->SetPosition(pos);
		enemy->SetActive(true);
		enemyList.push_back(enemy);
	}
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

void SceneCastle::DeleteEnemies()
{
	DeleteEnemy(); // 중복 제거, DeleteEnemy()를 호출
}

void SceneCastle::DeleteZoneEnemies()
{
	DeleteZoneSpecificObjects(); // 중복 제거, 기존 함수 사용
}

void SceneCastle::Init()
{
	texIds.push_back("graphics/Enemy_sheet.png");
	texIds.push_back("data/59984.png");

	soundIds.push_back("effects/link hurt.wav");
	soundIds.push_back("effects/throw.wav");
	soundIds.push_back("effects/rupee.wav");
	soundIds.push_back("effects/heart.wav");
	soundIds.push_back("effects/enemy hit.wav");
	soundIds.push_back("effects/link dies.wav");
	soundIds.push_back("effects/sword.wav");

	Scene::Init(); // 부모 클래스 Init 먼저 호출

	player = new Player("Player");
	tileMapCastle = new TileMap("TileMapCastle", "data/castleInner.tmj");
	if (tileMapCastle)
	{
		tileMapCastle->Init();
		AddGameObject(tileMapCastle);
	}

	if (player)
	{
		AddGameObject(player);
	}

	hud = new HUD("HUD");
	if (hud)
	{
		hud->Init();
		AddGameObject(hud);
	}

	if (FindGameObject("InventoryUI") == nullptr)
	{
		inventoryUI = new InventoryUI("InventoryUI");
		if (inventoryUI)
		{
			inventoryUI->Init();
			AddGameObject(inventoryUI);
		}
	}

	InitZones();

	// 체크포인트 설정
	std::vector<sf::Vector2f> firstPositions = tileMapCastle->getPositions(3, 8147);
	std::vector<sf::Vector2f> secondPositions = tileMapCastle->getPositions(3, 8182);

	firstBounds.clear();
	for (const auto& fpos : firstPositions)
	{
		firstBound = sf::FloatRect(fpos.x - 16, fpos.y - 16, 32, 32);
		firstBounds.push_back(firstBound);
	}

	secondBounds.clear();
	for (const auto& spos : secondPositions)
	{
		secondBound = sf::FloatRect(spos.x - 16, spos.y - 16, 32, 32);
		secondBounds.push_back(secondBound);
	}

	isSecondFloor = true;

	endPos = sf::Vector2f(-70.7919f, -232.579f);
	endHole = sf::FloatRect(endPos.x - 16, endPos.y - 16, 32, 32);
}

void SceneCastle::Enter()
{
	SpawnSquareHitBox();
	SpawnFloorCovers();
	Scene::Enter();
}

void SceneCastle::Update(float dt)
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

	CheckCollison();
	UpdateZones();
	UpdateBehaviorZone(dt);

	if (player && endHole.contains(player->GetGlobalBounds().getPosition()))
	{
		SCENE_MGR.ChangeScene(SceneIds::Castle);
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
		if (player)
		{
			std::cout << "PlayerPosition(" << player->GetPosition().x << ", " << player->GetPosition().y << ")" << std::endl;
		}
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

void SceneCastle::Exit()
{
	DeleteInteractables();
	DeleteHitboxes();
	DeleteEnemy();

	for (auto& zone : castleZones)
	{
		zone.entered = false;
	}
	zoneID = 0;
	changeZone = false;

	Scene::Exit();
}

void SceneCastle::Draw(sf::RenderWindow& window)
{
	window.setView(worldView);
	Scene::Draw(window);
}