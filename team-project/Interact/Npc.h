#pragma once
#include "Interactable.h"
#include "SpriteGo.h"

class Npc : public Interactable
{
protected:
	Direction currentDirection;
	HitBox hitbox;

	bool npcSay=0;
	int sayCount = 0;

	SpriteGo* conversation;
public:

	Npc(const std::string& name = "");
	~Npc() = default;

	void SetPlayer(Player* p);
	void DirectionSprite(Direction dir);
	Direction GetDirectionToPlayer();

	// Interactable
	void OnInteract() override;
	void Init();
	void Reset();
	void Update(float dt);
	
	//void Draw(sf::RenderWindow& window) override;
};