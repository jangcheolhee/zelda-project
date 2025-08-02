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
#include "BossEnemy.h"

SceneBoss::SceneBoss() :Scene(SceneIds::Boss)
{
	player = nullptr;
	tileMapBoss = nullptr;
}

void SceneBoss::SetPlayer(Player* p) {

	player = p;
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

	soundIds.push_back("effects/link hurt.wav");
	soundIds.push_back("effects/throw.wav");
	soundIds.push_back("effects/rupee.wav");
	soundIds.push_back("effects/heart.wav");
	soundIds.push_back("effects/enemy hit.wav");
	soundIds.push_back("effects/link dies.wav");
	soundIds.push_back("effects/sword.wav");
	soundIds.push_back("bgm/boss.flac");
	soundIds.push_back("effects/boss hit.wav");
	soundIds.push_back("effects/boss dies.wav");
	texIds.push_back("graphics/Boss.png");

	ANI_CLIP_MGR.Load("animations/bossDie.csv");
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
	auto size = FRAMEWORK.GetWindowSizeF();
	worldView.setSize({ size.x * .5f, size.y * .5f });
	worldView.setCenter({ 0,0 });
	points.resize(6);
	starts.resize(6);
	points.clear();
	starts.clear();

	starts.push_back({ -55,-55 });
	starts.push_back({ 0.f,-55 });
	starts.push_back({ 55,-55 });
	starts.push_back({ 55,0.f });
	starts.push_back({ -55,0.f });
	starts.push_back({ 0.f,0.f });

	points.push_back({ 0.f,-100 });
	points.push_back({ 55,-55 });
	points.push_back({ 55,0.f });
	points.push_back({ 0.f,45.f });
	points.push_back({ -55,-55 });
	points.push_back({ -55,0.f });
	for (int i = 0; i < 6; i++)
	{
		SpawnEnemy(starts[i], points[i], sf::Vector2f(-75 + 35 * i, -65), Enemy::Types::Boss);

	}
	sf::Vector2f startPos = tileMapBoss->getPosition(1, 1086);
	GAME_MGR.playerHp = player->GetMaxHp();
	GAME_MGR.currentMapID = (int)SCENE_MGR.GetCurrentSceneId();
	GAME_MGR.playerSpawnPosition = { -12, 85 };
	GAME_MGR.Save();
	player->SetPosition({ -12, 85 });
	worldView.setCenter({ 0, 0 });
	Scene::Enter();
	SOUND_MGR.PlayBgm(SOUNDBUFFER_MGR.Get("bgm/boss.flac"));
	player->SetHp(10);
	SpawnSquareHitBox();
}

void SceneBoss::Update(float dt)
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
	if (enemyList.size() == 3)
	{
		for (auto enemy : enemyList)
		{
			if (dynamic_cast<BossEnemy*>(enemy)->GetState() != BossState::Skill1)
			{
				dynamic_cast<BossEnemy*>(enemy)->SetPage1(true);
			}
		}
	}
	else if (enemyList.size() == 1)
		for (auto enemy : enemyList)
		{
			if (dynamic_cast<BossEnemy*>(enemy)->GetState() != BossState::Berserk)
			{
				dynamic_cast<BossEnemy*>(enemy)->SetPage2(true);
			}
		}

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

void SceneBoss::RecycleEnemy(Enemy* enemy)
{
	if (enemy)
	{
		enemy->SetActive(false);
		enemyPools[enemy->GetType()].push_back(enemy);
	}
}
void SceneBoss::DeleteEnemy()
{
	for (Enemy* e : enemyList)
	{
		RecycleEnemy(e);
	}
	enemyList.clear();
}

void SceneBoss::SpawnEnemy(sf::Vector2f pos1, sf::Vector2f pos2, sf::Vector2f pos3, Enemy::Types type)
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
		case Enemy::Types::Boss:
			enemy = (Enemy*)AddGameObject(new BossEnemy());
			break;
		default:
			break;
		}
		enemy->Init();
	}
	enemy->Reset();
	enemy->SetPosition(pos1);
	enemy->SetActive(true);
	if (dynamic_cast<BossEnemy*>(enemy))
	{
		dynamic_cast<BossEnemy*>(enemy)->StartPos(pos1);
		dynamic_cast<BossEnemy*>(enemy)->DesPos(pos2);
		dynamic_cast<BossEnemy*>(enemy)->SetPoint1(pos3);
	}

	enemyList.push_back(enemy);
}
void SceneBoss::CheckCollison()
{
	if (!player) return;

	for (auto& enemy : enemyList)
	{
		player->OnCollide(enemy);
		if (player->GetGlobalBounds().intersects(enemy->GetGlobalBounds()))
		{
			player->TakeDamageIfPossible(0);
		}
	}

	for (auto& obj : interactList)
	{
		if (Utils::CheckCollision(player->GetHitBox().rect, obj->GetHitBox().rect))
		{
			switch (obj->GetType())
			{
			case Interactable::Type::JumpWall:
				player->SetPosition(player->GetPos());
				obj->OnInteract();
				break;
			}
		}
	}
}