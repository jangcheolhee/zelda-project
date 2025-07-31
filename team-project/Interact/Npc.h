#pragma once
#include "Interactable.h"
class SpriteGo;
class Npc : public Interactable
{
public:
	enum class Type
	{
		None = -1,
		Basic,
		Dad,
	};
protected:
	Direction currentDirection;
	HitBox hitbox;

	bool npcSay=0;
	int sayCount = 0;
	
	SpriteGo* conversation;
	sf::View talkUi;

	Type npcType = Type::Basic;

public:

	Npc(const std::string& name = "");
	~Npc();

	void SetPlayer(Player* p);
	void SetNpcType(Type type) { npcType = type; }
	Type GetNpcType() const { return npcType; }

	void DirectionSprite(Direction dir);
	Direction GetDirectionToPlayer();
	void DaddySprite();

	void HandleBasicNpcInteraction();
	void HandleDadInteraction();

	// Interactable
	void OnInteract() override;
	void Init();
	void Reset();
	void Update(float dt);
	void Draw(sf::RenderWindow& window) override;
};