#pragma once
class Scene;
class Player;
class Enemy;
class Interactable;

class Collider
{
protected:

	Scene* scene = nullptr;
	Player* player = nullptr;

	std::list<Enemy*>* enemies = nullptr;
	std::vector<Interactable*>* interactables = nullptr;
public:
	void InitScene(Scene* scene);
	void Update(float dt);
	void CheckPlayerEnemy();
	void CheckPlayerInteractable();

};

