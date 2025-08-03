#pragma once
#include "Scene.h"
#include "GameOverUI.h"
class SceneGameOver :
    public Scene
{
protected:

public:

	SceneGameOver();

	void Init() override;
	void Enter() override;
	void Update(float dt) override;
};

