#pragma once
#include "Scene.h"
#include "Enemy.h"
#include "Interactable.h"
class Player;
class TileMap;

struct MapZone
{
	sf::FloatRect bounds; 
	int zoneId;           
	// 추가 정보
	std::function<void()> onEnter; 
	std::function<void()> onExit;  
	bool entered = false; 

	MapZone(const sf::FloatRect& b, int id,
		std::function<void()> enter,
		std::function<void()> exit,
		bool e = false)
		: bounds(b), zoneId(id), onEnter(enter), onExit(exit), entered(e) {
	}
};

class SceneGame : public Scene
{
protected:
	Player* player;
	TileMap* tileMap;
	std::vector<Interactable*> interactables;
	std::unordered_map<Enemy::Types, std::list<std::unique_ptr<Enemy>>> enemyPools;
	std::list<Enemy*> enemyList;

	std::vector<MapZone> mapZones;
	int zoneID = 1;

public:
	SceneGame();

	void InitZones();
	void UpdateZones();
	void UpdateBehaviorZone();

	Enemy* CreateOrReuseEnemy(Enemy::Types type);
	void RecycleEnemy(Enemy* enemy);
	void DeleteEnemy(); 
	void SpawnEnemy(sf::Vector2f pos, Enemy::Types type); //�� ���� ����(Ȱ��)
	void SpawnEnemyAtTile(int layerIndex, int targetGid, Enemy::Types type); //�ϰ� ����
	void SpawnBushesAtTile(int layerIndex, int targetGid, std::string="");
	void SpawnJumpAtTile(int layerIndex, int targetGid);
	void SpawnNpcAtTile(int layerIndex, int targetGid);

	void CheckCollison();

	void Init() override;
	void Enter() override;
	void Update(float dt) override;
};