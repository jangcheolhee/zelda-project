#pragma once
#include "Enemy.h"

enum class BossState 
{
	Stop,
	Idle,
	Jump,
	Skill1,
	Berserk,
};
class BossEnemy : public Enemy
{
protected:
	
	bool page1 = false;
	bool page2 = false;
	
	int hp = 5;
	sf::Vector2f pastPos;
	sf::Sprite body;
	sf::Sprite shadow;
	sf::Vector2f velocity;
	sf::Vector2f gravity = { 0.f, 500.f };
	sf::Vector2f startPos = { 0.f, 0.f };
	sf::Vector2f destinyPos = { 0.f, 0.f };
	sf::Vector2f direction = { 0,0 };
	sf::Vector2f point1 = { 0,0 };
	float timer = 0;
	float hitTimer = 0;
	HitBox hitBox;
	HitBox shadowBox;
	BossState state;
	bool onHit = false;

public:
	BossState GetState() { return state; }
	void SetPage1(bool b) { page1 = b; state = BossState::Skill1; }
	void SetPage2(bool b) { page2 = b; state = BossState::Berserk; }
	BossEnemy(const std::string& name = "");
	virtual ~BossEnemy() = default;
	HitBox GetHitBox()override { return hitBox; }
	sf::Vector2f GetPos() { return pastPos; }
	void StartPos(sf::Vector2f pos) { startPos = pos; }
	void DesPos(sf::Vector2f pos) { destinyPos = pos; }
	void SetPoint1(sf::Vector2f pos) { point1 = pos; }
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

	void Change();
	void CheckCollide(HitBox box) ;

};
