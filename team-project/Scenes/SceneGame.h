#pragma once
#include "Scene.h"
#include "Enemy.h"
class Player;

class SceneGame : public Scene
{
protected:
	Player* player;
	std::unordered_map<Enemy::Types, std::list<std::unique_ptr<Enemy>>> enemyPools;
	std::list<Enemy*> enemyList;

public:
	SceneGame();

	Enemy* CreateOrReuseEnemy(Enemy::Types type);
	void RecycleEnemy(Enemy* enemy);

	void DeleteEnemy(); // 리사이클 포함
	void SpawnEnemy(sf::Vector2f pos, Enemy::Types type);

	void Init() override;
	void Enter() override;
	
	void Update(float dt) override;
};

