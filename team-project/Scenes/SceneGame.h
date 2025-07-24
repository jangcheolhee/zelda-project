#pragma once
#include "Scene.h"
#include "Enemy.h"
#include "Interactable.h"
class Player;
class TileMap;
class SceneGame : public Scene
{
protected:
	Player* player;
	TileMap* tileMap;
	std::vector<Interactable*> interactables;
	std::unordered_map<Enemy::Types, std::list<std::unique_ptr<Enemy>>> enemyPools;
	std::list<Enemy*> enemyList;
public:
	SceneGame();

	Enemy* CreateOrReuseEnemy(Enemy::Types type);
	void RecycleEnemy(Enemy* enemy);
	void DeleteEnemy(); 
	void SpawnEnemy(sf::Vector2f pos, Enemy::Types type); //�� ���� ����(Ȱ��)
	void SpawnEnemyAtTile(int layerIndex, int targetGid, Enemy::Types type); //�ϰ� ����
	void SpawnBushesAtTile(int layerIndex, int targetGid); 
	void SpawnJumpAtTile(int layerIndex, int targetGid);
	void SpawnNpcAtTile(int layerIndex, int targetGid);

	void CheckCollison();

	void Init() override;
	void Enter() override;
	
	void Update(float dt) override;
};