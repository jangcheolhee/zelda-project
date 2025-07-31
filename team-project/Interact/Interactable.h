#pragma once
#include "GameObject.h"
#include "HitBox.h"
#include "Animator.h"
class SceneGame;
class Player;
class Interactable :  public GameObject
{
public:
	enum class Type
	{
		None = -1,
		Throw,
		Chest,
		JumpWall,
		Heart,
		Rupee,
		Npc
	};
protected:
	Animator animator;
	sf::Sprite body;
	HitBox hitBox;
	HitBox boundBox;
	Player* player;
	SceneGame* sceneGame = nullptr;

	sf::Vector2f dir = { 0.f,0.f };
	float speed = 150.f;
	Type type = Type::None;
	Direction direction = Direction::Down;

public:
	Interactable(const std::string& name = "");
	virtual ~Interactable() = default;

	Type GetType() const { return type; }
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

	HitBox GetHitBox() { return hitBox; };
	HitBox GetBoundBox() { return boundBox; };
	virtual void OnInteract() = 0;
	virtual bool IsDestructible() const { return false; }
	void Shoot();
};