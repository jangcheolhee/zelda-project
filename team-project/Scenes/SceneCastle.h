#pragma once
#include "Scene.h"
#include "Enemy.h"
#include "Interactable.h"
#include "SpriteGo.h"
#include "HUD.h"
class Player;
class TileMap;

struct CastleZone
{
	sf::FloatRect bounds;
	int zoneId;
	std::function<void()> onEnter;
	std::function<void()> onExit;
	bool entered = false;

	CastleZone(const sf::FloatRect& b, int id,
		std::function<void()> enter,
		std::function<void()> exit,
		bool e = false)
		: bounds(b), zoneId(id), onEnter(enter), onExit(exit), entered(e) {
	}
};

class SceneCastle :public Scene
{
protected:
	bool changeZone = false;

	Player* player;
	TileMap* tileMapCastle;
	HUD* hud = nullptr;
	InventoryUI* inventoryUI = nullptr;

	//inter
	std::vector<Interactable*> interactables;
	std::list<Interactable*>interactList;
	std::unordered_map<Interactable::Type, std::list<Interactable*>> interactPool;
	//enemy
	std::unordered_map<Enemy::Types, std::list<Enemy*>> enemyPools;
	std::list<Enemy*> enemyList;
	//zone
	std::vector<CastleZone> castleZones;
	int zoneID = 1;
	//pos
	sf::Vector2f endPos;
	sf::FloatRect endHole;
	//collision
	std::vector<HitBox> collisions;
	sf::RectangleShape collisionBox;
	float wallX = 0;
	float wallY = 0;
	float wallWithdh = 0;
	float wallHeight = 0;
	bool squareToggle = false;
	//floor
	std::vector<SpriteGo*> floor1DoorPathCovers; //1占쏙옙 2占쏙옙
	SpriteGo* floor2DoorPathCover; //2占쏙옙 1占쏙옙
	SpriteGo* LeftBridge;
	SpriteGo* RightBridge;
	bool isSecondFloor = 0;
	//2floor checkpoint
	std::vector<sf::Vector2f> secondPos;
	std::vector<sf::FloatRect> secondBounds;
	sf::FloatRect secondBound;
	//1floor checkpoint
	std::vector<sf::Vector2f> firstPos;
	std::vector<sf::FloatRect> firstBounds;
	sf::FloatRect firstBound;

public:
	SceneCastle();
	void SpawnFloorCovers();
	//Enemy
	std::list<Enemy*> GetEnemy() { return enemyList; }
	void RecycleEnemy(Enemy* enemy);
	void DeleteEnemy();
	void DeleteEnemies();
	void DeleteZoneEnemies();
	void SpawnEnemy(sf::Vector2f pos, Enemy::Types type);
	void SpawnEnemyAtTile(int layerIndex, int targetGid, Enemy::Types type);

	//zone
	void InitZones();
	void UpdateZones();
	void UpdateBehaviorZone(float dt);
	void DeleteZoneSpecificObjects();
	//interact
	std::list<Interactable*> GetInteract() { return interactList; }
	void DeleteInteractables();
	//hitbox
	void CheckCollison();
	void SpawnSquareHitBox();
	void DeleteHitboxes();
	//Scene
	void Exit() override;
	void Init() override;
	void Enter() override;
	void Update(float dt) override;
	
	void Draw(sf::RenderWindow& window) override;
};