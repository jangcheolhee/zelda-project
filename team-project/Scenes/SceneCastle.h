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
	// 추가 정보
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
	Player* player;
	TileMap* tileMapCastle;

	std::vector<Interactable*> interactables;
	std::unordered_map<Enemy::Types, std::list<std::unique_ptr<Enemy>>> enemyPools;
	std::list<Enemy*> enemyList;

	std::vector<CastleZone> castleZones;
	int zoneID = 1;

	sf::Vector2f endPos;
	sf::FloatRect endHole;
public:
	SceneCastle();

	void InitZones();
	void UpdateZones();
	void UpdateBehaviorZone();

	void DeleteInteractables();

	void CheckCollison();

	void Init() override;
	void Enter() override;
	void Update(float dt) override;
};