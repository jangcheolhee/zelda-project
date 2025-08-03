#pragma once
#include "Singleton.h"
#include "Player.h"
#include "HUD.h"
#include "Inventory.h"

class Player;
class HUD;
class InventoryUI;

class GameMgr : public Singleton<GameMgr>
{
private:
	

public:
	void Init();
	void Update(float dt);
	void Draw(sf::RenderWindow& window);
	void Release();

	int playerRupee = 0;
	int currentMapID = 1;
	sf::Vector2f playerSpawnPosition = {184,536};
	int playerHp = 5;
	int slotIdx = -1;

	int currentMapID2 = 1;
	sf::Vector2f playerSpawnPosition2 = { 184,536 };
	int playerHp2 = 5;

	Player* player = nullptr;
	HUD* hud = nullptr;
	InventoryUI* inventoryUI = nullptr;

	bool CheckSlot(int index);
	void Save() { SaveToSlot(slotIdx); }
	void SaveToSlot(int index);
	void SaveToSlot2(int index);
	void LoadFromSlot(int index);
	void DeleteSaveSlot(int index);

	std::string GetSlotFileName(int index) const;
	void InitializeGameObjects();

};

#define GAME_MGR (GameMgr::Instance())