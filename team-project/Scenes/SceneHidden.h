#pragma once
#include "Scene.h"
#include "Enemy.h"
#include "Interactable.h"
#include "SpriteGo.h"
class Player;
class TileMap;

struct HiddenZone
{
	sf::FloatRect bounds;
	int zoneId;
	// 추가 정보
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
	SpriteGo* Dad;

	std::vector<Interactable*> interactables;
	std::unordered_map<Enemy::Types, std::list<std::unique_ptr<Enemy>>> enemyPools;
	std::list<Enemy*> enemyList;

	std::vector<HiddenZone> hiddenZones;
	int zoneID = 1;
public:
	SceneHidden();

	void InitZones();
	void UpdateZones();
	void UpdateBehaviorZone();

	void DeleteInteractables();

	void CheckCollison();

	void Init() override;
	void Enter() override;
	void Update(float dt) override;

};