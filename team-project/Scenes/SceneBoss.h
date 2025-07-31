#pragma once
#include "Scene.h"
class Player;
class BossEnemy;
class SceneBoss : public Scene
{
protected:
	Player* player;
	std::vector<BossEnemy*> bosses;
	std::vector<sf::Vector2f> points;
	std::vector<sf::Vector2f> starts;
public:

	SceneBoss();

	void Init() override;
	void Enter() override;
	void Update(float dt) override;
};

