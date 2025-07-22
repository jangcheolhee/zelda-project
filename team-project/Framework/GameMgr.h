#pragma once
#include "Singleton.h"
#include "Player.h";
#include "Inventory.h"
#include "QuestMgr.h"
class GameMgr 
{
private:
	static GameMgr* instance;
	GameMgr();
public:
	Player* player;
	Inventory* inventory;
	QuestMgr* questMgr;

	static GameMgr& GetInstance();
	~GameMgr();
	void Init();
	void Update(float dt);
	void Draw(sf::RenderWindow& window);
	void SaveGame(const std::string& filename);
	void LoadGame(const std::string& filename);

};

#define GAME_MGR (GameMgr::GetInstance())

