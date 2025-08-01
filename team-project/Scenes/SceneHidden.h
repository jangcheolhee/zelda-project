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
	// �߰� ����
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

	std::vector<Interactable*> interactables;
	std::unordered_map<Enemy::Types, std::list<std::unique_ptr<Enemy>>> enemyPools;
	std::list<Enemy*> enemyList;

	std::unordered_map<Interactable::Type, std::list<Interactable*>> interactPool;
	std::list<Interactable*>interactList;


	std::vector<HiddenZone> hiddenZones;
	int zoneID = 1;

	sf::Vector2f endPos;
	sf::FloatRect endHole;

	std::vector<HitBox> collisions;
	sf::RectangleShape collisionBox;
	float wallX = 0;
	float wallY = 0;
	float wallWithdh = 0;
	float wallHeight = 0;
	bool squareToggle = false;

	SpriteGo* hiddenPathCover;

	SpriteGo* dad;
	bool dadSay = 0;
	int sayCount = 0;

public:
	SceneHidden();
	void SetPlayer(Player* p);
	
	std::list<Interactable*> GetInteract() { return interactList; }
	std::list<Enemy*> GetEnemy() { return enemyList; }
	void InitZones();
	void UpdateZones();
	void UpdateBehaviorZone();

	void DeleteInteractables();

	void CheckCollison();
	void SpawnSquareHitBox();

	void SpawnHiddenObject();

	void Init() override;
	void Enter() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};