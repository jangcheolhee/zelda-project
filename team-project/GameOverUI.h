#pragma once
#include "GameObject.h"
class GameOverUI :
    public GameObject
{
protected:
	sf::Sprite cursor;
	float timer = 0;
	int selectedIndex = 0;
	sf::Vector2f center;
	sf::Text title;
	sf::Text select;
	sf::Text retry;
public:
	GameOverUI(const std::string& name = "");
	virtual ~GameOverUI() = default;

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

