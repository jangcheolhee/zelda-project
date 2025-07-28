#include "stdafx.h"
#include "GameMgr.h"

void GameMgr::Release()
{
    delete inventory;
    delete questMgr;
}

void GameMgr::SaveGame(const std::string& filename)
{
	
	playerData.name = "Link"; 

	json j;

	// PlayerData ����
	j["player"]["name"] = playerData.name;
	j["player"]["hp"] = playerData.hp;
	j["player"]["position"]["x"] = playerData.position.x;
	j["player"]["position"]["y"] = playerData.position.y;

	std::ofstream file(filename);
	if (file.is_open())
	{
		file << j.dump(4); 
		file.close();
	}
}

void GameMgr::LoadGame(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Failed to open save file.\n";
		return;
	}

	json j;
	file >> j;
	file.close();

	// PlayerData �ҷ�����
	playerData.name = j["player"]["name"];
	playerData.hp = j["player"]["hp"];
	playerData.position.x = j["player"]["position"]["x"];
	playerData.position.y = j["player"]["position"]["y"];
}

void GameMgr::SetPlayerData(int hp, const sf::Vector2f& pos)
{
	playerData.hp = hp;
	playerData.position = pos;
}


void GameMgr::Init()
{
    inventory = new Inventory();
    questMgr = new QuestMgr();
	LoadGame("data/data.json");
}
void GameMgr::Update(float dt)
{
  
}
void GameMgr::Draw(sf::RenderWindow& window)
{
   
}