#pragma once
#include "Scene.h"
#include "Enemy.h"
#include "Interactable.h"
#include "SpriteGo.h"
#include "HitBox.h" 
class Player;
class TileMap;

class SceneBoss : public Scene
{
protected:
	Player* player;
	TileMap* tileMapBoss;

	std::vector<Interactable*> interactables;

	std::unordered_map<Enemy::Types, std::list<std::unique_ptr<Enemy>>> enemyPools;
	std::list<Enemy*> enemyList;

	std::unordered_map<Interactable::Type, std::list<Interactable*>> interactPool;
	std::list<Interactable*>interactList;

	sf::Vector2f endPos;
	sf::FloatRect endHole;

	std::vector<HitBox> collisions;
	sf::RectangleShape collisionBox;
	float wallX = 0;
	float wallY = 0;
	float wallWithdh = 0;
	float wallHeight = 0;
	bool squareToggle = false;

public:
	SceneBoss();
	void SetPlayer(Player* p);

	void DeleteInteractables();

	void CheckCollison();
	void SpawnSquareHitBox();

	void Init() override;
	void Enter() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};