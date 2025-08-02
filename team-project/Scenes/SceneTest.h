#pragma once
#include "Scene.h"
class Player;
class SceneTest : public Scene
{
	Player* player;

public:

	SceneTest();
	void Release() override;
	void Init() override;
	void Enter() override;
	void Update(float dt) override;
};

