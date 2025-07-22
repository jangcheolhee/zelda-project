#pragma once
#include "Singleton.h"
#include "Inventory.h"
#include "QuestMgr.h"

struct PlayerData
{
	std::string name;
	int hp;
	sf::Vector2f position;
};

class GameMgr : public Singleton<GameMgr>
{
	friend Singleton<GameMgr>;
private:
	PlayerData playerData;

	Inventory* inventory;
	QuestMgr* questMgr;

public:
	void Init();
	void Update(float dt);
	void Draw(sf::RenderWindow& window);
	void Release();
	
	void SaveGame(const std::string& filename);
	void LoadGame(const std::string& filename);
	PlayerData& GetPlayerData() { return playerData; }
	void SetPlayerData(int hp, sf::Vector2f& pos);

};

#define GAME_MGR (GameMgr::Instance())