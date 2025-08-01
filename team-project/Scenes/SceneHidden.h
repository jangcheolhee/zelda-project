#pragma once
#include "Scene.h"
#include "Enemy.h"
#include "Interactable.h"
#include "SpriteGo.h"
#include "HitBox.h" 
class Player;
class TileMap;

struct HiddenZone
{
	sf::FloatRect bounds;
	int zoneId;

	std::function<void()> onEnter;
	std::function<void()> onExit;
	bool entered = false;

	HiddenZone(const sf::FloatRect& b, int id,
		std::function<void()> enter,
		std::function<void()> exit,
		bool e = false)
		: bounds(b), zoneId(id), onEnter(enter), onExit(exit), entered(e) {
	}
};

class SceneHidden : public Scene
{
protected:	
	Player* player;
	TileMap* tileMapHidden;
	SpriteGo* hiddenPathCover;

	//enemy
	std::unordered_map<Enemy::Types, std::list<Enemy*>> enemyPools;
	std::list<Enemy*> enemyList;

	//interact
	std::unordered_map<Interactable::Type, std::list<Interactable*>> interactPool;
	std::list<Interactable*>interactList;
	std::vector<Interactable*> interactables;

	//zone
	std::vector<HiddenZone> hiddenZones;
	int zoneID = 1;

	//pos
	sf::Vector2f endPos;
	sf::FloatRect endHole;

	//hitbox_collision
	std::vector<HitBox> collisions;
	sf::RectangleShape collisionBox;
	float wallX = 0;
	float wallY = 0;
	float wallWithdh = 0;
	float wallHeight = 0;
	bool squareToggle = false;

	//SpriteGo* dad;
	//bool dadSay = 0;
	//int sayCount = 0;

public:
	SceneHidden();
	void SetPlayer(Player* p);

	//Enemy
	std::list<Enemy*> GetEnemy() { return enemyList; }
	void RecycleEnemy(Enemy* enemy);
	void DeleteEnemy();
	void SpawnEnemy(sf::Vector2f pos, Enemy::Types type);
	void SpawnEnemyAtTile(int layerIndex, int targetGid, Enemy::Types type);

	//zone
	void InitZones();
	void UpdateZones();
	void UpdateBehaviorZone();

	//hitbox_collision
	std::list<Interactable*> GetInteract() { return interactList; }
	void DeleteInteractables();
	void CheckCollison();
	void SpawnSquareHitBox();
	void SpawnHiddenObject();

	//Scene
	void Init() override;
	void Enter() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};