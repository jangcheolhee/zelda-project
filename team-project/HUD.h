#pragma once
#include "GameObject.h"
#include "InventoryUI.h"
#include "HUD.h"

class HUD : public GameObject
{
protected:
	sf::Sprite hudSprite;
	sf::Sprite body;
	int rupeeCount = 0;
	sf::Text rupeeText;
	sf::Sprite statusUI;
	InventoryUI inventoryUI;
	bool showStatus = true;
	int hp = 6;
	int maxHp = 6;
	
	sf::Text heartText;
	sf::Texture fullHeartTex;
	std::vector<sf::Sprite> heartSprites;
	sf::RectangleShape background;
	

public:
	HUD(const std::string& name = "");
	virtual ~HUD() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float rot) override;
	void SetScale(const sf::Vector2f& s) override;
	void SetOrigin(const sf::Vector2f& o) override;
	void SetOrigin(Origins preset) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void AddRupee(int amount);
	void AddHeart(int amount);
	void SetHeartCount(int newHp)
	{
		hp = std::clamp(newHp, 0, maxHp);
		UpdateHeartSprites();
	}
	void UpdateHeartSprites();
	void SetSize(const sf::Vector2f& size);
	void ApplyBossStyle();
};

