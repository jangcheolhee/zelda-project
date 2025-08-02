#pragma once
#include "Scene.h"
#include "Enemy.h"
#include "Interactable.h"
#include "SpriteGo.h"
#include "HitBox.h"
#include "HUD.h"
class Player;
class TileMap;

class SceneBoss : public Scene
{
protected:
	Player* player;
	TileMap* tileMapBoss;
	HUD* hud;
	InventoryUI* inventoryUI;

	std::vector<Interactable*> interactables;

	std::unordered_map<Enemy::Types, std::list<Enemy*>> enemyPools;
	std::list<Enemy*> enemyList;

	std::unordered_map<Interactable::Type, std::list<Interactable*>> interactPool;
	std::list<Interactable*>interactList;

std::list<Enemy*> bosses;
	std::vector<sf::Vector2f> points;
	std::vector<sf::Vector2f> starts;

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
	sf::RectangleShape background;
	std::list<Enemy*> GetEnemy() { return bosses; }
	std::list<Interactable*> GetInteract() { return interactList; }

	void RecycleEnemy(Enemy* enemy);
	void DeleteEnemy();
	void SpawnEnemy(sf::Vector2f pos1, sf::Vector2f pos2, sf::Vector2f pos3, Enemy::Types type);

	void DeleteInteractables();

	void CheckCollison();
	void SpawnSquareHitBox();

	void Init() override;
	void Enter() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
	
};
