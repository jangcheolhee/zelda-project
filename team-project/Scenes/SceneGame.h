#pragma once
#include "Scene.h"
class Player;
class TileMap;
class SceneGame : public Scene
{
protected:
	Player* player;
	TileMap* tileMap;

public:
	SceneGame();

	void Init() override;
	void Enter() override;
	
	void Update(float dt) override;
};

