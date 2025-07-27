#pragma once
#include "Interactable.h"
class Player;

class Npc : public Interactable
{
protected:
	Player* player;
	Direction currentDirection;

public:
	Npc(const std::string& name = "");
	~Npc() = default;

	void SetPlayer(Player* p);
	void DirectionSprite(Direction dir);
	Direction GetDirectionToPlayer();

	// Interactable을(를) 통해 상속됨
	void OnInteract() override;
	void Init();
	void Reset();
	void Update(float dt);
};