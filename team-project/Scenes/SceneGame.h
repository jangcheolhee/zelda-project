#pragma once
#include "Scene.h"
class Player;
class SceneGame : public Scene
{
protected:
	Player* player;

public:
	SceneGame();

	void Init() override;
	void Enter() override;
	
	void Update(float dt) override;
};

