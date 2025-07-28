#pragma once
#include "Interactable.h"
enum class BushState { OnGround, Held, Thrown };
class Bush : public Interactable
{
protected:
	float life = 0.f;
	sf::Vector2f gravity = { 0.f, 50.f };
	BushState state = BushState::OnGround;
public:
	// Interactable을(를) 통해 상속됨
	void OnInteract() override;
	void Init() override;
	void Reset() override;
	void UpdateBeHavior(float dt) override;
	void Shoot();
};