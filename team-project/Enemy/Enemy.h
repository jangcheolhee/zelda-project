#pragma once
#include "GameObject.h"
#include "Animator.h"
#include "HitBox.h"

class SceneGame;
class Player;
class Enemy : public GameObject
{
public:
	enum class Types
	{
		Basic,
		Count,
	};
protected:
	float hitCooldown = 0.5f;     // 피격 간 최소 시간 (초)
	float LastHit = 0.f; // 마지막 피격 이후 시간

	sf::Sprite body;
	Animator animator;
	SceneGame* sceneGame = nullptr;
	Player* player;
	
	sf::Vector2f velocity = { 0.f, 0.f };
	
	sf::Vector2f initPosition;

	HitBox hitBox;
	HitBox boundBox;

	int maxHp = 10;
	int hp = 3;
	float speed = 0.f;
	bool isHitThisFrame = false;
	Types type = Types::Basic;
	
	float skillInetrval = 0.f;
	float skillTimer = 0.f;
	int damage = 0;
	int attackInterval = 0.f;
	sf::Vector2f dir = { 0.f, 0.f };
	Direction direction = Direction::Down;
	sf::Vector2f pastPosition;

public:
	Enemy(const std::string& name = "");
	~Enemy() = default;

	sf::Vector2f GetPos() { return pastPosition; }
	Types GetType() { return type; }
	void SetInitPosition(sf::Vector2f pos) { initPosition = pos; }
	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	sf::FloatRect GetLocalBounds() const override
	{
		return body.getLocalBounds();
	}

	sf::FloatRect GetGlobalBounds() const override
	{
		return body.getGlobalBounds();
	}

	void OnCollide(Player* player);
	int GetDamage() { return damage; }

	const HitBox& GetHitBox() const { return hitBox; }
	void OnDamage(int damage);
	
	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	
	virtual void OnCollideBySword();
	virtual void OnHit(int damage);

	void DeathAnimation();
	HitBox GetHitBox() { return hitBox; };
	
};

