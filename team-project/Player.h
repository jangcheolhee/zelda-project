#pragma once
#include "GameObject.h"
class Player :  public GameObject
{
protected:
	//sf::Sprite body;
	sf::RectangleShape body;
	int hp = 0;
	int maxHp = 0;

public:
	Player(const std::string& name = "");
	virtual ~Player() = default;

	int GetHp() { return hp; }
	void SetHp(int hp) { this->hp = hp; }

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
};

