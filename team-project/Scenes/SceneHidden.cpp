#include "stdafx.h"
#include "SceneHidden.h"
#include "Player.h"
#include "TileMap.h"
#include "BasicEnemy.h"
#include "Enemy.h"
#include "Bush.h"
#include "Npc.h"
#include "Chest.h"
#include "Rupee.h"
#include "JumpWall.h"
#include <istream>
#include "SceneCastle.h"
#include "HitboxGenerator.h"

SceneHidden::SceneHidden() :Scene(SceneIds::Hidden)
{
	player = nullptr;
	//dad = nullptr;
	tileMapHidden = nullptr;
	texIds.push_back("graphics/HUD.png");
	texIds.push_back("data/HiddenPathToGarden.png");
	texIds.push_back("graphics/inventory.png");
	fontIds.push_back("fonts/Neo.ttf");
}

void SceneHidden::SetPlayer(Player* p) {

	player = p;
}

void SceneHidden::InitZones()
{
	hiddenZones.clear();

	// 화면 크기 기반으로 Zone 계산 (worldView가 50% 크기이므로) - 원래 설정 복원
	auto size = FRAMEWORK.GetWindowSizeF();
	float halfWidth = size.x * 0.25f;   // worldView 절반 너비
	float halfHeight = size.y * 0.25f;  // worldView 절반 높이

	// Zone 1: 왼쪽 아래 (y 좌표 수정)
	hiddenZones.push_back({
		sf::FloatRect(-halfWidth, 0, halfWidth * 2, halfHeight * 2),
		1,
		[this]() { std::cout << "Zone 1 Enter (Left Bottom)" << std::endl; },
		[this]() { std::cout << "Zone 1 Exit" << std::endl; },
		false
		});

	// Zone 2: 왼쪽 위 (y 좌표 수정)
	hiddenZones.push_back({
		sf::FloatRect(-halfWidth, -halfHeight * 2, halfWidth * 2, halfHeight * 2),
		2,
		[this]() { std::cout << "Zone 2 Enter (Left Top)" << std::endl; },
		[this]() { std::cout << "Zone 2 Exit" << std::endl; },
		false
		});

	// Zone 3: 오른쪽 아래 (y 좌표 수정)
	hiddenZones.push_back({
		sf::FloatRect(0, 0, halfWidth * 2, halfHeight * 2),
		3,
		[this]() { std::cout << "Zone 3 Enter (Right Bottom)" << std::endl; },
		[this]() { std::cout << "Zone 3 Exit" << std::endl; },
		false
		});

	// Zone 4: 오른쪽 위 (y 좌표 수정)
	hiddenZones.push_back({
		sf::FloatRect(0, -halfHeight * 2, halfWidth * 2, halfHeight * 2),
		4,
		[this]() { std::cout << "Zone 4 Enter (Right Top)" << std::endl; },
		[this]() { std::cout << "Zone 4 Exit" << std::endl; },
		false
		});
}

void SceneHidden::UpdateZones()
{
	if (!player || hiddenZones.empty()) return;

	sf::Vector2f playerPos = player->GetGlobalBounds().getPosition();
	for (auto& zone : hiddenZones)
	{
		bool nowInZone = zone.bounds.contains(playerPos);
		if (nowInZone && !zone.entered)
		{
			zone.entered = true;
			zoneID = zone.zoneId;
			if (zone.onEnter)
			{
				zone.onEnter();
				// Zone 변경 시에는 NPC와 특별한 오브젝트만 재생성
				DeleteZoneSpecificObjects();
				SpawnHiddenObject();
			}
		}
		else if (!nowInZone && zone.entered)
		{
			zone.entered = false;
			if (zone.onExit)
			{
				zone.onExit();
				// Zone을 벗어날 때 Zone별 오브젝트만 정리
				DeleteZoneSpecificObjects();
			}
		}
	}
}

void SceneHidden::UpdateBehaviorZone()
{
	if (!player || zoneID < 1 || zoneID > 4) return;

	auto size = FRAMEWORK.GetWindowSizeF();
	float halfWidth = size.x * 0.25f;   // worldView의 절반 너비
	float halfHeight = size.y * 0.25f;  // worldView의 절반 높이

	sf::Vector2f playerPos = player->GetGlobalBounds().getPosition();

	switch (zoneID)
	{
	case 1: // 왼쪽 아래
	{
		float x = Utils::Clamp(playerPos.x, -halfWidth, halfWidth);
		float y = Utils::Clamp(playerPos.y, 0, halfHeight * 2);
		worldView.setCenter({ x, y });
		break;
	}
	case 2: // 왼쪽 위
	{
		float x = Utils::Clamp(playerPos.x, -halfWidth, halfWidth);
		float y = Utils::Clamp(playerPos.y, -halfHeight * 2, 0);
		worldView.setCenter({ x, y });
		break;
	}
	case 3: // 오른쪽 아래
	{
		float x = Utils::Clamp(playerPos.x, 0, halfWidth * 2);
		float y = Utils::Clamp(playerPos.y, 0, halfHeight * 2);
		worldView.setCenter({ x, y });
		break;
	}
	case 4: // 오른쪽 위
	{
		float x = Utils::Clamp(playerPos.x, 0, halfWidth * 2);
		float y = Utils::Clamp(playerPos.y, -halfHeight * 2, 0);
		worldView.setCenter({ x, y });
		break;
	}
	}
}

void SceneHidden::CheckCollison()
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

			case Interactable::Type::Npc:
				// NPC 상호작용은 OnInteract에서 자동 처리
				break;
			}
		}
	}
}

void SceneHidden::SpawnSquareHitBox()
{
	HitboxGenerator::SpawnSquareHitBox(
		tileMapHidden,
		collisions,
		collisionBox,
		"Hidden"
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

void SceneHidden::SpawnHiddenObject()
{
	//Hidden Door Path
	sf::Vector2f hiddenPathPos = tileMapHidden->getPosition(1, 6168);

	auto hiddenPathCover = new SpriteGo();
	hiddenPathCover->SetName("floor1DoorPathCovers");
	hiddenPathCover->Init();

	hiddenPathCover->GetSprite().setTexture(TEXTURE_MGR.Get("data/HiddenPathToGarden.png"));
	hiddenPathCover->GetSprite().setTextureRect({ 112, 208, 32, 112 });

	hiddenPathCover->SetActive(1);
	hiddenPathCover->SetOrigin(Origins::TL);
	hiddenPathCover->SetPosition(hiddenPathPos);
	AddGameObject(hiddenPathCover);

	//Dad
	sf::Vector2f dadPos = tileMapHidden->getPosition(1, 1174);

	Interactable* dadInteractable = nullptr;
	auto& pool = interactPool[Interactable::Type::Npc];
	if (!pool.empty())
	{
		dadInteractable = pool.front();
		pool.pop_front();
	}
	else
	{
		dadInteractable = (Interactable*)AddGameObject(new Npc());
		dadInteractable->Init();
	}

	if (auto npc = dynamic_cast<Npc*>(dadInteractable))
	{
		npc->SetNpcType(Npc::Type::Dad);
		npc->SetPlayer(player);
		npc->Reset();
		npc->SetActive(true);
		npc->SetPosition({ dadPos.x + 20.f, dadPos.y + 20.f });
		npc->sortingLayer = SortingLayers::Background;

		interactList.push_back(dadInteractable);
	}
}

void SceneHidden::DeleteHitboxes()
{
	collisions.clear();
}

void SceneHidden::DeleteInteractables()
{
	// Scene 종료 시에만 모든 인터랙터블 삭제
	for (Interactable* inter : interactList)
	{
		inter->SetActive(false);
		interactPool[inter->GetType()].push_back(inter);
	}
	interactList.clear();
}

void SceneHidden::DeleteZoneSpecificObjects()
{
	// NPC와 특별한 Zone 오브젝트만 삭제 (JumpWall은 유지)
	auto it = interactList.begin();
	while (it != interactList.end())
	{
		if ((*it)->GetType() == Interactable::Type::Npc)
		{
			(*it)->SetActive(false);
			auto& pool = interactPool[(*it)->GetType()];
			pool.push_back(*it);
			it = interactList.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void SceneHidden::RecycleEnemy(Enemy* enemy)
{
	if (enemy)
	{
		enemy->SetActive(false);
		enemyPools[enemy->GetType()].push_back(enemy);
	}
}

void SceneHidden::SpawnEnemy(sf::Vector2f pos, Enemy::Types type)
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

void SceneHidden::SpawnEnemyAtTile(int layerIndex, int targetGid, Enemy::Types type)
{
	std::vector<sf::Vector2f> positions = tileMapHidden->getPositions(layerIndex, targetGid);
	for (const auto& pos : positions)
	{
		SpawnEnemy(pos, type);
	}
}

void SceneHidden::Init()
{
	texIds.push_back("graphics/Enemy_sheet.png");
	texIds.push_back("data/HiddenPathToGarden.png");
	texIds.push_back("graphics/conversation.png");

	fontIds.push_back("fonts/DS-DIGIT.ttf");
	fontIds.push_back("fonts/Neo.ttf");
	fontIds.push_back("fonts/DungGeunMo.ttf");

	soundIds.push_back("bgm/Cave.flac");

	ANI_CLIP_MGR.Load("animations/bush2.csv");
	ANI_CLIP_MGR.Load("animations/EnemyDeath.csv");

	player = new Player("Player");
	tileMapHidden = new TileMap("TileMapHidden", "data/hiddenPath.tmj");
	tileMapHidden->Init();
	player = new Player("Player");
	player->Init();
	TEXTURE_MGR.Load("graphics/Link.png");

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
	AddGameObject(tileMapHidden);

	InitZones();

	endPos = tileMapHidden->getPosition(1, 5680);
	endHole = sf::FloatRect(endPos.x - 16, endPos.y - 16, 32, 32);


	if (FindGameObject("InventoryUI") == nullptr)
	{
		inventoryUI = new InventoryUI("InventoryUI");
		inventoryUI->Init();
		AddGameObject(inventoryUI);
	}
}

void SceneHidden::Enter()
{
	player->Reset();
	auto size = FRAMEWORK.GetWindowSizeF();
	sf::Vector2f center{ size.x * 0.5f, size.y * 0.5f };
	uiView.setSize(size);
	uiView.setCenter(center);
	worldView.setSize({ size.x * .5f, size.y * .5f });

	if (inventoryUI)
	{
		inventoryUI->SetActive(false);
		inventoryUI->Reset();
	}

	Scene::Enter();
	SOUND_MGR.PlayBgm(SOUNDBUFFER_MGR.Get("bgm/Cave.flac"));
	sf::Vector2f startPos = tileMapHidden->getPosition(1, 6206);
	player->SetPosition(startPos);
	GAME_MGR.playerHp = player->GetMaxHp();
	GAME_MGR.currentMapID = (int)SCENE_MGR.GetCurrentSceneId();
	GAME_MGR.playerSpawnPosition = startPos;

	GAME_MGR.Save();
	worldView.setCenter(player->GetGlobalBounds().getPosition());

	// Scene 진입 시 한 번만 히트박스 생성 (전체 맵에 대해)
	SpawnSquareHitBox();

	// 적 스폰
	SpawnEnemyAtTile(1, 6149, Enemy::Types::Basic);

	// 초기 Zone ID 설정 및 모든 Zone을 비활성화 상태로 초기화
	zoneID = 0;
	for (auto& zone : hiddenZones)
	{
		zone.entered = false;
	}
}

void SceneHidden::Update(float dt)
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
		else ++it;
	}

	CheckCollison();
	UpdateZones();
	UpdateBehaviorZone();

	if (endHole.contains(player->GetGlobalBounds().getPosition()))
	{
		std::cout << "Castle" << std::endl;
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

void SceneHidden::Exit()
{
	// Scene 종료 시에만 모든 히트박스, 인터랙터블, 적 정리
	DeleteInteractables();
	DeleteHitboxes();

	// Zone 상태 초기화
	for (auto& zone : hiddenZones)
	{
		zone.entered = false;
	}
	zoneID = 0;

	Scene::Exit();
}

void SceneHidden::Draw(sf::RenderWindow& window)
{
	window.setView(worldView);
	Scene::Draw(window);

	if (hud) hud->Draw(window);
	if (inventoryUI && inventoryUI->GetActive()) 
	{
		inventoryUI->Draw(window);

	}

}