#pragma once
#include "Scene.h"
#include "SpriteGo.h"
#include "Enemy.h"
#include "Interactable.h"
class Player;
class TileMap;
class HitBox;

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
	TileMap* tileMapGame;

	std::vector<Interactable*> interactables;
	std::unordered_map<Enemy::Types, std::list<std::unique_ptr<Enemy>>> enemyPools;
	std::list<Enemy*> enemyList;

	std::vector<MapZone> mapZones;
	int zoneID = 1;

	sf::Vector2f endPos;
	sf::FloatRect endHole;

	std::vector<SpriteGo*> flowers;
	float flowerTimer;
	bool flowerBool;
	float flowerRate = 0.3;

	HitBox collision;
	std::vector<HitBox> collisions;
	sf::RectangleShape collisionBox;
	bool squareToggle = 0;

public:
	SceneGame();

	std::list<Enemy*> GetEnemy() { return enemyList; }
 	void InitZones();
	void UpdateZones();
	void UpdateBehaviorZone();
	void DeleteInteractables();

	Enemy* CreateOrReuseEnemy(Enemy::Types type);
	void RecycleEnemy(Enemy* enemy);
	void DeleteEnemy(); 
	void SpawnEnemy(sf::Vector2f pos, Enemy::Types type);
	void SpawnEnemyAtTile(int layerIndex, int targetGid, Enemy::Types type);
	
	void SpawnFlowers(sf::FloatRect zone);
	void FlowerBreath(float dt);

	void SpawnInteractableObject(sf::FloatRect zone);
	void CheckCollison();

	void SpawnSquareHitBox();

	void Init() override;
	void Exit() override;
	void Enter() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};
