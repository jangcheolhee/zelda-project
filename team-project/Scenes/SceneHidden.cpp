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
#include "HUD.h"
#include "InventoryUI.h"
#include "SceneBoss.h"

SceneHidden::SceneHidden() :Scene(SceneIds::Hidden)
{
	player = nullptr;
	tileMapHidden = nullptr;
	hud = nullptr;
	inventoryUI = nullptr;
	key = nullptr;  
	hasKey = false;

	
	texIds.push_back("graphics/HUD.png");
	texIds.push_back("data/HiddenPathToGarden.png");
	texIds.push_back("graphics/inventory.png");
	texIds.push_back("graphics/Items.png");
	fontIds.push_back("fonts/Neo.ttf");
}


void SceneHidden::InitZones()
{
	hiddenZones.clear();

	auto size = FRAMEWORK.GetWindowSizeF();
	float halfWidth = size.x * 0.25f;
	float halfHeight = size.y * 0.25f;

	// Zone 1: 왼쪽 아래
	hiddenZones.push_back({
		sf::FloatRect(-halfWidth, 0, halfWidth * 2, halfHeight * 2),
		1,
		[this]() {
			std::cout << "Zone 1 Enter" << std::endl;
			DeleteZoneSpecificObjects();
			SpawnHiddenObject();
		},
		[this]() {
			std::cout << "Zone 1 Exit" << std::endl;
			DeleteZoneSpecificObjects();
		},
		false
		});

	// Zone 2: 왼쪽 위
	hiddenZones.push_back({
		sf::FloatRect(-halfWidth, -halfHeight * 2, halfWidth * 2, halfHeight * 1.8f), 
		2,
		[this]() {
			std::cout << "Zone 2 Enter" << std::endl;
			DeleteZoneSpecificObjects();
			SpawnHiddenObject();
		},
		[this]() {
			std::cout << "Zone 2 Exit" << std::endl;
			DeleteZoneSpecificObjects();
		},
		false
		});

	// Zone 3: 오른쪽 아래
	hiddenZones.push_back({
		sf::FloatRect(0, 0, halfWidth * 2, halfHeight * 2),
		3,
		[this]() {
			std::cout << "Zone 3 Enter" << std::endl;
			DeleteZoneSpecificObjects();
			SpawnHiddenObject();
		},
		[this]() {
			std::cout << "Zone 3 Exit" << std::endl;
			DeleteZoneSpecificObjects();
		},
		false
		});

	// Zone 4: 오른쪽 위
	hiddenZones.push_back({
		sf::FloatRect(0, -halfHeight * 2, halfWidth * 2, halfHeight * 2),
		4,
		[this]() {
			std::cout << "Zone 4 Enter" << std::endl;
			DeleteZoneSpecificObjects();
			SpawnHiddenObject();
		},
		[this]() {
			std::cout << "Zone 4 Exit" << std::endl;
			DeleteZoneSpecificObjects();
		},
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
			}
			std::cout << "Entered Zone " << zone.zoneId << std::endl;
		}
		else if (!nowInZone && zone.entered)
		{
			zone.entered = false;
			if (zone.onExit)
			{
				zone.onExit();
			}
			std::cout << "Exited Zone " << zone.zoneId << std::endl;
		}
	}
}

void SceneHidden::UpdateBehaviorZone()
{
	if (!player || zoneID < 1 || zoneID > 4) return;

	auto size = FRAMEWORK.GetWindowSizeF();
	float halfWidth = size.x * 0.25f;
	float halfHeight = size.y * 0.25f;

	sf::Vector2f playerPos = player->GetGlobalBounds().getPosition();

	sf::Vector2f viewSize = worldView.getSize();
    float viewHalfWidth = viewSize.x * 0.5f;
    float viewHalfHeight = viewSize.y * 0.5f;

	switch (zoneID)
	{
	case 1:
	{
		float x = Utils::Clamp(playerPos.x, -halfWidth, halfWidth);
		float y = Utils::Clamp(playerPos.y, 0, halfHeight * 2);
		worldView.setCenter({ x, y });
		break;
	}
	case 2:
	{
		float x = Utils::Clamp(playerPos.x, -halfWidth, halfWidth);
		float y = Utils::Clamp(playerPos.y, -halfHeight * 2, 0);
		worldView.setCenter({ x, y });

		if (playerPos.y >= 0)
		{
			zoneID = 1; 
			for (auto& zone : hiddenZones)
			{
				if (zone.zoneId == 1 && !zone.entered)
				{
					zone.entered = true;
					if (zone.onEnter)
					{
						zone.onEnter();
					}
				}
				else if (zone.zoneId == 2 && zone.entered)
				{
					zone.entered = false;
					if (zone.onExit)
					{
						zone.onExit();
					}
				}
			}
			float x1 = Utils::Clamp(playerPos.x, -halfWidth, halfWidth);
			float y1 = Utils::Clamp(playerPos.y, 0, halfHeight * 2);
			worldView.setCenter({ x1, y1 });
		}
		break;
	}
	case 3:
	{
		float x = Utils::Clamp(playerPos.x, 0, halfWidth * 2);
		float y = Utils::Clamp(playerPos.y, 0, halfHeight * 2);
		worldView.setCenter({ x, y });
		break;
	}
	case 4:
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

	if (key && key->GetActive() && !hasKey)
	{
		sf::Vector2f playerPos = player->GetPosition();
		sf::Vector2f keyPosition = key->GetPosition();
		float distance = sqrt(pow(playerPos.x - keyPosition.x, 2) + pow(playerPos.y - keyPosition.y, 2));
		if (distance < 25.0f)
		{
			SOUND_MGR.PlaySfx(SOUNDBUFFER_MGR.Get("effects/key.wav"));
			hasKey = true;
			key->SetActive(false);
			key->GetSprite().setColor(sf::Color::Transparent);
			key->SetPosition({ -1000, -1000 });
			endPosActive = true;
		}
	}

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

		if (inter)
		{
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
}

void SceneHidden::SpawnHiddenObject()
{
	//Hidden Door Path
	sf::Vector2f hiddenPathPos = tileMapHidden->getPosition(1, 6168);

	auto hiddenPathCover = new SpriteGo();
	hiddenPathCover->SetName("hiddenPathCover");
	hiddenPathCover->Init();

	hiddenPathCover->GetSprite().setTexture(TEXTURE_MGR.Get("data/HiddenPathToGarden.png"));
	hiddenPathCover->GetSprite().setTextureRect({ 112, 208, 32, 112 });

	hiddenPathCover->SetActive(true);
	hiddenPathCover->SetOrigin(Origins::TL);
	hiddenPathCover->SetPosition(hiddenPathPos);
	AddGameObject(hiddenPathCover);

	// Key 
	if (!hasKey && key == nullptr)
	{
		keyPos = tileMapHidden->getPosition(1, 6200);

		key = new SpriteGo();
		key->SetName("Key");
		key->Init();

		key->GetSprite().setTexture(TEXTURE_MGR.Get("graphics/Items.png"));
		key->GetSprite().setTextureRect({ 228, 190, 15, 17 });

		key->SetActive(true);
		key->SetOrigin(Origins::MC);
		key->SetPosition(keyPos);
		key->sortingLayer = SortingLayers::Foreground;
		AddGameObject(key);
	}

	//Dad NPC
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
		if (dadInteractable)
		{
			dadInteractable->Init();
		}
	}

	if (dadInteractable)
	{
		if (auto npc = dynamic_cast<Npc*>(dadInteractable))
		{
			npc->SetNpcType(Npc::Type::Dad);
			npc->SetPlayer(player);
		}
		dadInteractable->Reset();
		dadInteractable->SetActive(true);
		dadInteractable->SetPosition({ dadPos.x + 20.f, dadPos.y + 20.f });
		dadInteractable->sortingLayer = SortingLayers::Background;
		interactList.push_back(dadInteractable);
	}
}

void SceneHidden::DeleteHitboxes()
{
	collisions.clear();
}

void SceneHidden::DeleteInteractables()
{
	for (Interactable* inter : interactList)
	{
		inter->SetActive(false);
		interactPool[inter->GetType()].push_back(inter);
	}
	interactList.clear();
}

void SceneHidden::DeleteZoneSpecificObjects()
{
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

	for (auto& obj : zoneObjects)
	{
		if (obj)
		{
			RemoveGameObject(obj);
		}
	}
	zoneObjects.clear();
}

void SceneHidden::DeleteEnemies()
{
	for (Enemy* enemy : enemyList)
	{
		if (enemy)
		{
			enemy->SetActive(false);
			enemyPools[enemy->GetType()].push_back(enemy);
		}
	}
	enemyList.clear();
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

void SceneHidden::SpawnEnemyAtTile(int layerIndex, int targetGid, Enemy::Types type)
{
	if (tileMapHidden)
	{
		std::vector<sf::Vector2f> positions = tileMapHidden->getPositions(layerIndex, targetGid);
		for (const auto& pos : positions)
		{
			SpawnEnemy(pos, type);
		}
	}
}

void SceneHidden::Init()
{
	texIds.push_back("graphics/Enemy_sheet.png");
	texIds.push_back("data/HiddenPathToGarden.png");
	texIds.push_back("graphics/conversation.png");
	texIds.push_back("graphics/Items.png"); 

	fontIds.push_back("fonts/DS-DIGIT.ttf");
	fontIds.push_back("fonts/Neo.ttf");
	fontIds.push_back("fonts/DungGeunMo.ttf");

	soundIds.push_back("bgm/Cave.flac");
	TEXTURE_MGR.Load("graphics/sword_get.png");
	soundIds.push_back("effects/key.wav");
	soundIds.push_back("effects/message.wav");
	soundIds.push_back("effects/item get 1.wav");
	soundIds.push_back("effects/enemy hit.wav");
	ANI_CLIP_MGR.Load("animations/bush2.csv");
	ANI_CLIP_MGR.Load("animations/EnemyDeath.csv");

	Scene::Init();

	

	tileMapHidden = new TileMap("TileMapHidden", "data/hiddenPath.tmj");
	if (tileMapHidden)
	{
		tileMapHidden->Init();
		AddGameObject(tileMapHidden);
		mapBounds = tileMapHidden->GetGlobalBounds();
	}

	//hud = new HUD("HUD");
	if (hud)
	{
		hud->Init();
		//AddGameObject(hud);
		hud->SetRupee(GAME_MGR.playerRupee);
		hud->SetHeartCount(player->GetHp());
	}

	//player = new Player("Player");
	//if (player)
	//{
	//	/*player->Init();
	//	TEXTURE_MGR.Load("graphics/Link.png");
	//	AddGameObject(player);
	//	player->SetHUD(hud);*/
	//	player->SetRupee(GAME_MGR.playerRupee);
	//}
	/*if (FindGameObject("InventoryUI") == nullptr)
	{
		inventoryUI = new InventoryUI("InventoryUI");
		if (inventoryUI)
		{
			inventoryUI->Init();
			AddGameObject(inventoryUI);
		}
	}*/

	player = GAME_MGR.player;
	hud = GAME_MGR.hud;
	inventoryUI = GAME_MGR.inventoryUI;

	// 무조건 Init()!
	if (player) {
		player->Init();
		player->SetRupee(GAME_MGR.playerRupee);
	}
	if (hud) hud->SetRupee(GAME_MGR.playerRupee);


	// 이 객체들을 Scene에 등록 (중복 Add 막기!)
	if (player && !FindGameObject("Player"))
		AddGameObject(player);
	if (hud && !FindGameObject("HUD"))
		AddGameObject(hud);
	if (inventoryUI && !FindGameObject("InventoryUI"))
		AddGameObject(inventoryUI);
	player->SetRupee(GAME_MGR.playerRupee);
	hud->SetRupee(GAME_MGR.playerRupee);
	InitZones();

	if (tileMapHidden)
	{
		endPos = tileMapHidden->getPosition(1, 5680);
		endHole = sf::FloatRect(endPos.x - 16, endPos.y - 16, 32, 32);
	}

	key = nullptr;
	hasKey = false;
	endPosActive = false;
}

void SceneHidden::Enter()
{
	DeleteEnemies();
	DeleteZoneSpecificObjects();
	DeleteHitboxes();
	player = GAME_MGR.player;
	hud = GAME_MGR.hud;
	if (player && hud) {
		player->SetHUD(hud);
	}
	hud->SetHeartCount(player->GetHp());
	if (player)
	{
		player->Init(); 
		player->SetRupee(GAME_MGR.playerRupee);
		player->Reset();
	}

	auto size = FRAMEWORK.GetWindowSizeF();
	sf::Vector2f center{ size.x * 0.5f, size.y * 0.5f };
	uiView.setSize(size);
	uiView.setCenter(center);
	worldView.setSize({ size.x * .5f, size.y * .5f });

	if (player) player->SetRupee(GAME_MGR.playerRupee);
	if (hud) hud->SetRupee(GAME_MGR.playerRupee);

	if (inventoryUI)
	{
		inventoryUI->SetActive(false);
		inventoryUI->Reset();
	}
	if(hud) 
	{
		hud->Init(); 
		hud->SetRupee(GAME_MGR.playerRupee);
		hud->SetHeartCount(player ? player->GetHp() : GAME_MGR.playerHp);
	}
	Scene::Enter();
	SOUND_MGR.PlayBgm(SOUNDBUFFER_MGR.Get("bgm/Cave.flac"));

	if (tileMapHidden && player)
	{
		sf::Vector2f startPos = tileMapHidden->getPosition(1, 6206);
		player->SetPosition(startPos);
		GAME_MGR.playerHp = player->GetMaxHp();
		GAME_MGR.currentMapID = (int)SCENE_MGR.GetCurrentSceneId();
		GAME_MGR.playerSpawnPosition = startPos;
		worldView.setCenter(player->GetGlobalBounds().getPosition());
	}

	GAME_MGR.Save();

	SpawnSquareHitBox();
	SpawnEnemyAtTile(1, 6149, Enemy::Types::Basic);

	// Key
	hasKey = false;
	endPosActive = false;

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
		else
		{
			++it;
		}
	}

	CheckCollison();
	UpdateZones();
	UpdateBehaviorZone();

	if (player && hasKey && endHole.contains(player->GetGlobalBounds().getPosition()))
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

void SceneHidden::Exit()
{
	DeleteInteractables();
	DeleteEnemies();
	DeleteHitboxes();

	for (auto& obj : zoneObjects)
	{
		if (obj)
		{
			RemoveGameObject(obj);
		}
	}
	zoneObjects.clear();

	if (key)
	{
		RemoveGameObject(key);
		key = nullptr;
	}

	for (auto& zone : hiddenZones)
	{
		zone.entered = false;
	}
	zoneID = 0;
	Scene::Exit();
	GAME_MGR.playerRupee = player->GetRupee();//루피 기록
}

void SceneHidden::Draw(sf::RenderWindow& window)
{
	window.setView(worldView);
	Scene::Draw(window);

	window.setView(uiView);
	if (hud)
	{
		hud->Draw(window);
	}
	if (inventoryUI && inventoryUI->GetActive())
	{
		inventoryUI->Draw(window);
	}

}

void SceneHidden::SetPlayer(Player* p) { player = p; }
void SceneHidden::SetHUD(HUD* h) { hud = h; }
void SceneHidden::SetInventoryUI(InventoryUI* inv) { inventoryUI = inv; }
