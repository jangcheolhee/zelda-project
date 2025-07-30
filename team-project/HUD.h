#pragma once
#include "GameObject.h"
#include "InventoryUI.h"
class HUD :
    public GameObject
{
protected:
	sf::Sprite body;
	int rupeeCount = 0;
	sf::Text rupeeText;
	sf::Sprite statusUI;
	InventoryUI inventoryUI;
	bool showStatus = true;

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
};

