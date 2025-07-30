#pragma once
#include "Interactable.h"

class Npc : public Interactable
{
public:
	enum class Type
	{
		None = -1,
		Guard,
		Daddy,
	};
protected:
	Direction currentDirection;
	HitBox hitbox;

	bool npcSay=0;
	int sayCount = 0;
	
	sf::Sprite talkUI;

	Type type = Type::None;

public:

	Npc(const std::string& name = "");
	~Npc() = default;

	void SetPlayer(Player* p);
	void DirectionSprite(Direction dir);
	Direction GetDirectionToPlayer();

	void DaddySprite();

	Type GetType() const { return type; }

	// Interactable
	void OnInteract() override;
	void Init();
	void Reset();
	void Update(float dt);
	
	//void Draw(sf::RenderWindow& window) override;
};