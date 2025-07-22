#pragma once
#include "stdafx.h"
#include "GameObject.h"
class TileMap : public GameObject
{
protected:
	std::string tileMap = "data/filed_20250722.tmj";

	json j;
	j["layers"]["chunks"]["data"] = ;
public:

	// GameObject을(를) 통해 상속됨
	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};