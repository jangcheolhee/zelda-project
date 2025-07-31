#pragma once
#include "Singleton.h"


class GameMgr : public Singleton<GameMgr>
{
private:
	
	int slotIdx = -1;

public:
	void Init();
	void Update(float dt);
	void Draw(sf::RenderWindow& window);
	void Release();
	

	int currentMapID = 1;
	sf::Vector2f playerSpawnPosition = {184,536};
	int playerHp = 3;
	
	void Save() { SaveToSlot(slotIdx); }
	void SaveToSlot(int index);
	void LoadFromSlot(int index);
	void DeleteSaveSlot(int index);

	std::string GetSlotFileName(int index) const;
};

#define GAME_MGR (GameMgr::Instance())