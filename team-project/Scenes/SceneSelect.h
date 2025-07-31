#pragma once
#include "Scene.h"
class SaveSlotUI;
class Player;
class SceneSelect : public Scene
{
	SaveSlotUI* saveSloatUI;

public:

	SceneSelect();

	void Init() override;
	void Enter() override;
	void Update(float dt) override;
};

