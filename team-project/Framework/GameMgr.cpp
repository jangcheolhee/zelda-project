#include "stdafx.h"
#include "GameMgr.h"

GameMgr* GameMgr::instance = nullptr;

GameMgr::GameMgr()
{
    
}

GameMgr& GameMgr::GetInstance()
{
    if (!instance)
    {
        instance = new GameMgr();
    }
    return *instance;

}

GameMgr::~GameMgr()
{
    delete player;
    delete inventory;
    delete questMgr;
}


void GameMgr::SaveGame(const std::string& filename)
{
}

void GameMgr::LoadGame(const std::string& filename)
{
}


void GameMgr::Init()
{
    player = new Player();
    inventory = new Inventory();
    questMgr = new QuestMgr();
}
void GameMgr::Update(float dt)
{
    player->Update(dt);
}
void GameMgr::Draw(sf::RenderWindow& window)
{
    player->Draw(window);
}
