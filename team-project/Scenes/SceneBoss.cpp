#include "stdafx.h"
#include "SceneBoss.h"
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

SceneBoss::SceneBoss() :Scene(SceneIds::Boss)
{
	player = nullptr;
	tileMapBoss = nullptr;
}

void SceneBoss::SetPlayer(Player* p) {

	player = p;
}

void SceneBoss::CheckCollison()
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

void SceneBoss::SpawnSquareHitBox()
{
	std::cout << "SpawnSquareHitBox() called" << std::endl;

	HitboxGenerator::SpawnSquareHitBox(
		tileMapBoss,
		collisions,
		collisionBox,
		"Boss"
	);

	std::cout << "After SpawnSquareHitBox, collisions size: " << collisions.size() << std::endl;

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

void SceneBoss::DeleteInteractables()
{
	auto it = interactables.begin();
	while (it != interactables.end())
	{
		RemoveGameObject(*it);
		it = interactables.erase(it);
	}
	interactables.clear();
}

void SceneBoss::Init()
{
	player = new Player("Player");
	tileMapBoss = new TileMap("TileMapBoss", "data/boss.tmj");
	tileMapBoss->Init();

	AddGameObject(player);
	AddGameObject(tileMapBoss);

	//endPos = tileMapBoss->getPosition(1, 5680);
	//endHole = sf::FloatRect(endPos.x - 16, endPos.y - 16, 32, 32);

	Scene::Init();
}

void SceneBoss::Enter()
{
	player->Reset();
	auto size = FRAMEWORK.GetWindowSizeF();
	sf::Vector2f center{ size.x * 0.5f, size.y * 0.5f };
	uiView.setSize(size);
	uiView.setCenter(center);
	worldView.setSize({ size.x * 0.5f, size.y * 0.5f });

	Scene::Enter();

	sf::Vector2f startPos = tileMapBoss->getPosition(1, 1086);
	player->SetPosition(startPos);
	GAME_MGR.playerHp = player->GetMaxHp();
	GAME_MGR.currentMapID = (int)SCENE_MGR.GetCurrentSceneId();
	GAME_MGR.playerSpawnPosition = startPos;
	GAME_MGR.Save();
	worldView.setCenter({player->GetGlobalBounds().getPosition().x+5.f, player->GetGlobalBounds().getPosition().y - 90.f});

	SpawnSquareHitBox();
}

void SceneBoss::Update(float dt)
{
	Scene::Update(dt);

	CheckCollison();
	//if (endHole.contains(player->GetGlobalBounds().getPosition()))
	//{
	//	std::cout << "Castle" << std::endl;
	//	SCENE_MGR.ChangeScene(SceneIds::Castle);
	//}
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

void SceneBoss::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
	window.setView(worldView);

	for (auto& col : collisions)
	{
		col.Draw(window);
	}
}