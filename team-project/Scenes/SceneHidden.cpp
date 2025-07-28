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


SceneHidden::SceneHidden() :Scene(SceneIds::Hidden)
{
	player = nullptr;
	tileMapHidden = nullptr;
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
	player = new Player("Player");
	tileMapHidden = new TileMap("TileMapHidden", "data/hiddenPath.tmj");
	tileMapHidden->Init();
	auto dad = new SpriteGo();
	dad->Init();
	dad->GetSprite().setTexture(TEXTURE_MGR.Get("data/HiddenPathToGarden.png"));
	//dad->GetSprite().getTextureRect({220,103,30,30});
	//dad->SetActive

	AddGameObject(player);
	AddGameObject(tileMapHidden);

	InitZones();

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
	worldView.setCenter(player->GetGlobalBounds().getPosition());
}

void SceneHidden::Update(float dt)
{
	Scene::Update(dt);

	CheckCollison();
	UpdateZones();
	UpdateBehaviorZone();
	if (InputMgr::GetKeyDown(sf::Keyboard::F1))
	{
		GAME_MGR.SetPlayerData(player->GetHp(), sf::Vector2f{ 0,0 });
		SCENE_MGR.ChangeScene(SceneIds::Game);
	}
}