#pragma once
#include "Scene.h"
class Player;
class Enemy;
class SceneBoss : public Scene
{
protected:
	Player* player;
	std::list<Enemy*> bosses;
	std::vector<sf::Vector2f> points;
	std::vector<sf::Vector2f> starts;



public:
	std::list<Enemy*> GetEnemy() { return bosses; }
	SceneBoss();



	void Init() override;
	void Enter() override;
	void Update(float dt) override;
};

