#include "stdafx.h"
#include "SceneHidden.h"
#include "Player.h"
#include "TileMap.h"
#include "BasicEnemy.h"
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
	dad = nullptr;
	tileMapHidden = nullptr;
}

void SceneHidden::SetPlayer(Player* p) {
	
	player = p;
}

void SceneHidden::InitZones()
{
	hiddenZones.clear();

	hiddenZones.push_back({
		sf::FloatRect(-96, 128, 192, 256),
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
	hiddenZones.push_back({
		sf::FloatRect(-96, -128, 192, 256),
		2,
		[this]()
		{
			std::cout << "Zone 2 Enter" << std::endl;
		},
		[this]()
		{
			std::cout << "Zone 2 Exit" << std::endl;
		},
		false
		});
	hiddenZones.push_back({
		sf::FloatRect(96, 384, 128, 384),
		3,
		[this]()
		{
			std::cout << "Zone 3 Enter" << std::endl;
		},
		[this]()
		{
			std::cout << "Zone 3 Exit" << std::endl;
		},
		false
		});
	hiddenZones.push_back({
		sf::FloatRect(128, -128, 256, 256),
		4,
		[this]()
		{
			std::cout << "Zone 4 Enter" << std::endl;
		},
		[this]()
		{
			std::cout << "Zone 4 Exit" << std::endl;
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
				SpawnSquareHitBox();
				SpawnHiddenObject();
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


void SceneHidden::UpdateBehaviorZone()
{
	if (!player || zoneID < 1 || zoneID > 4) return;

	switch (zoneID)
	{//player 기준
	case 1:
	{
		float x = Utils::Clamp(player->GetGlobalBounds().getPosition().x, -96, 96);
		float y = Utils::Clamp(player->GetGlobalBounds().getPosition().y, 128, 384);
		worldView.setCenter({ x, y });
	}
	break;
	case 2:
	{
		float x = Utils::Clamp(player->GetGlobalBounds().getPosition().x, -96, 96);
		float y = Utils::Clamp(player->GetGlobalBounds().getPosition().y, -128, 128);
		worldView.setCenter({ x, y });
	}
	break;
	case 3:
	{
		float x = Utils::Clamp(player->GetGlobalBounds().getPosition().x, 96, 384);
		float y = Utils::Clamp(player->GetGlobalBounds().getPosition().y, 128, 384);
		worldView.setCenter({ x, y });
	}
	break;
	case 4:
	{
		float x = Utils::Clamp(player->GetGlobalBounds().getPosition().x, 128, 384);
		float y = Utils::Clamp(player->GetGlobalBounds().getPosition().y, -128, 128);
		worldView.setCenter({ x, y });
	}
	break;
	}
}

void SceneHidden::CheckCollison()
{
	if (!player) return;

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
	else dadInteractable = (Interactable*)AddGameObject(new Npc());

	if (auto npc = dynamic_cast<Npc*>(dadInteractable))
	{
		npc->SetNpcType(Npc::Type::Dad);
		npc->SetPlayer(player);

		dadInteractable->Init();
		dadInteractable->Reset();

		if (auto npc = dynamic_cast<Npc*>(dadInteractable))
		{
			npc->DaddySprite();
		}

		dadInteractable->SetActive(true);
		dadInteractable->SetPosition(dadPos);
		interactList.push_back(dadInteractable);
	}
}

void SceneHidden::DeleteInteractables()
{
	auto it = interactables.begin();
	while (it != interactables.end())
	{
		RemoveGameObject(*it);
		it = interactables.erase(it);
	}
	interactables.clear();
}

void SceneHidden::Init()
{
	texIds.push_back("data/HiddenPathToGarden.png");

	player = new Player("Player");
	tileMapHidden = new TileMap("TileMapHidden", "data/hiddenPath.tmj");
	tileMapHidden->Init();

	AddGameObject(player);
	AddGameObject(tileMapHidden);

	InitZones();

	endPos = tileMapHidden->getPosition(1, 5680);
	endHole = sf::FloatRect(endPos.x - 16, endPos.y - 16, 32, 32);

	Scene::Init();
}

void SceneHidden::Enter()
{
	player->Reset();
	auto size = FRAMEWORK.GetWindowSizeF();
	sf::Vector2f center{ size.x * 0.5f, size.y * 0.5f };
	uiView.setSize(size);
	uiView.setCenter(center);
	worldView.setSize({ size.x * .5f, size.y * .5f });

	Scene::Enter();

	sf::Vector2f startPos = tileMapHidden->getPosition(1, 6206);
	player->SetPosition(startPos);
	GAME_MGR.playerHp = player->GetMaxHp();
	GAME_MGR.currentMapID = (int)SCENE_MGR.GetCurrentSceneId();
	GAME_MGR.playerSpawnPosition = startPos;
	GAME_MGR.Save();
	worldView.setCenter(player->GetGlobalBounds().getPosition());
}

void SceneHidden::Update(float dt)
{
	Scene::Update(dt);

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
}

void SceneHidden::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
	window.setView(worldView);

	for (auto& col : collisions)
	{
		col.Draw(window);
	}
}