#pragma once
#include "GameObject.h"
enum class MenuState
{
	SelectSlot,
	ConfirmDelete
};
class SaveSlotUI : public GameObject
{
protected:
	struct Slot {
		sf::Sprite icon;
		
		std::vector<sf::Sprite> hearts;
	};

	sf::Text title;
	sf::Text copyText;
	sf::Text deleteText;
	float timer = 0;
	sf::Sprite body;
	std::vector<Slot> slots;
	int selectedIndex = 0;
	sf::Vector2f center;
	float slotSpacing = 60.f;

	MenuState state = MenuState::SelectSlot;

	
public:
	SaveSlotUI(const std::string& name = "");
	virtual ~SaveSlotUI() = default;

	void SetSlotData(int index, int heartCount);
	void SetState(MenuState newState);
	void UpdateUIByState();

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

	std::function<void(int)> onSlotSelected;
	std::function<void(int)> onSlotDelete;
};

