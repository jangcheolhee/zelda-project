#pragma once
#include "Interactable.h"
class Enemy;
enum class BushState { OnGround, Held, Thrown, Crush };
class Bush : public Interactable
{
protected:
	float lifeTime = 0.f;
	sf::Vector2f gravity = { 0.f, 150.f };
	BushState state = BushState::OnGround;
	sf::Vector2f velocity;
	std::list<Enemy*> enemyList;
	bool isHit = false;

public:
	void ChangeAnimation();
	// Interactable을(를) 통해 상속됨
	void OnInteract() override;
	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void Shoot();
};