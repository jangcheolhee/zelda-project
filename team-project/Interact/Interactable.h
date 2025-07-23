#pragma once
#include "GameObject.h"
#include "HitBox.h"
class SceneGame;
class Player;
class Interactable :  public GameObject
{
protected:
	sf::Sprite body;
	HitBox hitBox;
	Player* player;
	SceneGame* sceneGame = nullptr;
	
	float shootTimer = 0.f;
	bool isShoot = false;

	sf::Vector2f direction = { 0.f,0.f };
	float speed = 150.f;

public:
	Interactable(const std::string& name = "");
	virtual ~Interactable() = default;

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

	sf::FloatRect GetLocalBounds() const override
	{
		return body.getLocalBounds();
	}

	sf::FloatRect GetGlobalBounds() const override
	{

		return body.getGlobalBounds();
	}

	virtual void OnInteract() = 0;
	virtual bool IsDestructible() const { return false; }

	void Shoot();
};



