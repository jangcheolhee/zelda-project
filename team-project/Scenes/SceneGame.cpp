#include "stdafx.h"
#include "SceneGame.h"
#include "Player.h"
#include "TileMap.h"
#include "BasicEnemy.h"
#include "Bush.h"

SceneGame::SceneGame()
	:Scene(SceneIds::Game)
{
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

    if (newEnemy != nullptr)
        newEnemy->Init();

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
    enemy->SetInitPosition(pos);
    
    enemy->Reset();
    enemy->SetActive(false);

    AddGameObject(enemy);
    enemyList.push_back(enemy);
}

void SceneGame::SpawnEnemyAtTile(int layerIndex, int targetGid, Enemy::Types type)
{
    std::vector<sf::Vector2f> positions = tileMap->getPositions(layerIndex, targetGid);
    for (const auto& pos : positions)
    {
        SpawnEnemy(pos, type);
    }
}

void SceneGame::SpawnBushesAtTile(int layerIndex, int targetGid)
{
    std::vector<sf::Vector2f> positions = tileMap->getPositions(layerIndex, targetGid);
    for (const auto& pos : positions)
    {
        auto bush = new Bush;
        AddGameObject(bush);
        interactables.push_back(bush); // 따로 관리
    }
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
	//fontIds.push_back("fonts/DS-DIGIT.ttf");

	//ANI_CLIP_MGR.Load("animations/idle.csv");
	//ANI_CLIP_MGR.Load("animations/run.csv");
	//ANI_CLIP_MGR.Load("animations/jump.csv");

	player = new Player("Player");
	tileMap = new TileMap("TileMap");
   
	AddGameObject(player);
	AddGameObject(tileMap);
	
	Scene::Init();
}

void SceneGame::Enter()
{
	auto size = FRAMEWORK.GetWindowSizeF();
    sf::Vector2f center{ size.x * 0.5f, size.y * 0.5f };
	uiView.setSize(size);
	uiView.setCenter(center);
    worldView.setSize({size.x *.5f, size.y *.5f});
    worldView.setCenter(player->GetGlobalBounds().getPosition());
    sf::Vector2f startPos = tileMap->getPosition(1, 506);
    
    //SpawnEnemyAtTile(2, 94, Enemy::Types::Basic);
    SpawnBushesAtTile(2, 94);


	Scene::Enter();
    player->SetPosition(startPos);
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);
    worldView.setCenter(player->GetGlobalBounds().getPosition());
}