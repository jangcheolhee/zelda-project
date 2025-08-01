#pragma once
#include "Enemy.h"

enum class BossState 
{
	Stop,
	Idle,
	Jump,
	Berserk,
};
class BossEnemy : public Enemy
{
protected:
	int hp = 5;
	sf::Vector2f pastPos;
	sf::Sprite body;
	sf::Sprite shadow;
	sf::Vector2f velocity;
	sf::Vector2f gravity = { 0.f, 500.f };
	sf::Vector2f startPos = { 0.f, 0.f };
	sf::Vector2f destinyPos = { 0.f, 0.f };
	sf::Vector2f direction = { 0,0 };
	float timer = 0;
	HitBox hitBox;
	BossState state;

public:
	BossEnemy(const std::string& name = "");
	virtual ~BossEnemy() = default;
	HitBox GetHitBox()override { return hitBox; }
	sf::Vector2f GetPos() { return pastPos; }
	void StartPos(sf::Vector2f pos) { startPos = pos; }
	void DesPos(sf::Vector2f pos) { destinyPos = pos; }
	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float rot) override;
	void SetScale(const sf::Vector2f& s) override;
	void SetOrigin(const sf::Vector2f& o) override;
	void SetOrigin(Origins preset) override;
	sf::FloatRect GetLocalBounds() const override
	{
		return body.getLocalBounds();
	}

	sf::FloatRect GetGlobalBounds() const override
	{
		return body.getGlobalBounds();
	}
	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void OnDamage(int damage) override;


};
