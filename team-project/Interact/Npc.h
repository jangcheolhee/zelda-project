#pragma once
#include "Interactable.h"
class SpriteGo;
class TextGo;
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
	TextGo* dialogText;
	sf::View talkUi;

	Type npcType = Type::Basic;

	int dialogueIndex = 0; 
	std::vector<std::wstring> dialogues;
	int npcId = 0;  
	static int nextNpcId;  
	static std::unordered_map<int, int> npcDialogueIndices;
	//대화 쿨타임 추가
	bool recentlyTalked = false;
	float talkCooldown = 0.f;

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

	void InitDialogues();

	// Interactable
	void OnInteract() override;
	void Init();
	void Reset();
	void Update(float dt);
	void Draw(sf::RenderWindow& window) override;
};