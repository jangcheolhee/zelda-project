#pragma once
#include "Scene.h"
#include "SpriteGo.h"
#include "Enemy.h"
#include "Interactable.h"
#include "InventoryUI.h"
#include "HUD.h"
class Player;
class TileMap;
class HitBox;
class TextGo;

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
	bool changeZone = false;
	
	Player* player;
	HUD* hud;
	TileMap* tileMapGame;
	InventoryUI* inventoryUI;
	
	std::unordered_map<Interactable::Type, std::list<Interactable*>> interactPool;
	std::list<Interactable*>interactList;
	std::vector<Interactable*> interactables;

	std::unordered_map<Enemy::Types, std::list<Enemy*>> enemyPools;
	std::list<Enemy*> enemyList;

	std::vector<MapZone> mapZones;
	int zoneID = 1;
	bool showInventory=false;

	sf::Vector2f endPos;
	sf::FloatRect endHole;
	bool endPosActivated = false;
	Interactable* holeBush = nullptr;
	bool holeBushInteracted = false; 
	float holeBushTimer = 0.0f;   
	const float holeBushDelay = 2.0f;

	std::vector<SpriteGo*> flowers;
	float flowerTimer;
	bool flowerBool;
	float flowerRate = 0.3;

	HitBox collision;
	std::vector<HitBox> collisions;
	sf::RectangleShape collisionBox;
	float wallX =  0;
	float wallY = 0;
	float wallWithdh = 0;
	float wallHeight = 0;

public:
	SceneGame();
	const std::list<GameObject*>& GetGameObjects() const
	{
		return gameObjects;
	}
	std::vector<Interactable*> GetInteractables() const
	{
		return std::vector<Interactable*>(interactList.begin(), interactList.end());
	}

	std::list<Interactable*> GetInteract(){return interactList;}
	std::list<Enemy*> GetEnemy() { return enemyList; }
 	void InitZones();
	void UpdateZones();
	void ResetZones();
	void UpdateBehaviorZone(float dt);
	void DeleteInteractables();
	void ForceUpdateZones();
	
	void SpawnFlowers(sf::FloatRect zone);
	void FlowerBreath(float dt);

	void SpawnInteractableObject(sf::FloatRect zone);
	void SpawnInteractable(sf::Vector2f pos, Interactable::Type type);
	void CheckCollison();
	void SpawnSquareHitBox();

	void SpawnEnemy(sf::Vector2f pos, Enemy::Types type);
	void SpawnEnemyAtTile(int layerIndex, int targetGid, Enemy::Types type);
	void RecycleEnemy(Enemy* enemy);
	void DeleteEnemies();

	void Init() override;
	void Exit() override;
	void Enter() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window)override;
	Player* GetPlayer() const { return player; }
	HUD* GetHUD() const { return hud; }
	const std::list<Interactable*>& GetInteractablesList() const;
};