#pragma once
#include "Scene.h"
#include "Enemy.h"
class Player;
class TileMap;
class SceneGame : public Scene
{
protected:
	Player* player;
	TileMap* tileMap;
	std::unordered_map<Enemy::Types, std::list<std::unique_ptr<Enemy>>> enemyPools;
	std::list<Enemy*> enemyList;

public:
	SceneGame();

	Enemy* CreateOrReuseEnemy(Enemy::Types type);
	void RecycleEnemy(Enemy* enemy);

	void DeleteEnemy(); // ������Ŭ ����
	void SpawnEnemy(sf::Vector2f pos, Enemy::Types type);

	void Init() override;
	void Enter() override;
	
	void Update(float dt) override;
};

