#pragma once
#include "Scene.h"
#include "Enemy.h"
#include "Interactable.h"
#include "SpriteGo.h"
class Player;
class TileMap;

struct CastleZone
{
	sf::FloatRect bounds;
	int zoneId;
	// Ãß°¡ Á¤º¸
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

	std::vector<Interactable*> interactables;
	std::unordered_map<Enemy::Types, std::list<std::unique_ptr<Enemy>>> enemyPools;
	std::list<Enemy*> enemyList;

	std::vector<CastleZone> castleZones;
	int zoneID = 1;

	sf::Vector2f endPos;
	sf::FloatRect endHole;

	//floor
	std::vector<SpriteGo*> floor1DoorPathCovers; //1Ãþ 2°³
	SpriteGo* floor2DoorPathCover; //2Ãþ 1°³
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

	void InitZones();
	void UpdateZones();
	void UpdateBehaviorZone(float dt);

	void DeleteInteractables();

	void CheckCollison();

	void SpawnFloorCovers();

	void Init() override;
	void Enter() override;
	void Update(float dt) override;
};