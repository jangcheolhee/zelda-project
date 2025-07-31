#include "stdafx.h"
#include "GameMgr.h"

void GameMgr::Release()
{

}

void GameMgr::SaveToSlot(int index)
{
	json j;
	j["map"] = currentMapID;
	j["position"] = { {"x", playerSpawnPosition.x}, {"y", playerSpawnPosition.y} };
	j["hp"] = playerHp;

	std::filesystem::create_directories("save");
	std::ofstream out(GetSlotFileName(index));
	if (out.is_open())
	{
		out << j.dump(4);
	}
}

void GameMgr::LoadFromSlot(int index)
{
	slotIdx = index;
	std::ifstream in(GetSlotFileName(index));
	if (!in.is_open()) 
	{ 
		SaveToSlot(index); 
		return; 
	}

	json j;
	in >> j;

	currentMapID = j["map"];
	playerSpawnPosition.x = j["position"]["x"];
	playerSpawnPosition.y = j["position"]["y"];
	playerHp = j["hp"];
}

void GameMgr::DeleteSaveSlot(int index)
{
	std::string path = GetSlotFileName(index);
	if (std::filesystem::exists(path))
	{
		std::filesystem::remove(path);
	}
}

std::string GameMgr::GetSlotFileName(int index) const
{
	return "save/slot" + std::to_string(index) + ".json";
}

void GameMgr::Init()
{
}
void GameMgr::Update(float dt)
{
  
}
void GameMgr::Draw(sf::RenderWindow& window)
{
   
}