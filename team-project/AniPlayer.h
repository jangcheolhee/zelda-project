#pragma once
#include "Animator.h"

class AniPlayer : public GameObject
{
protected:
	sf::Sprite body;
	Animator animator;

	sf::Vector2f gravity = { 0.f, 500.f };
	sf::Vector2f velocity = { 0.f, 0.f };
	bool isGrounded = true;
	float speed = 500.f;
	std::map<Direction, std::vector<sf::IntRect>> animations;
	Direction currentDirection = Direction::Down;
	std::size_t currentFrameIndex = 0;

public:
	AniPlayer(const std::string& name = "");
	~AniPlayer() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
	sf::IntRect GetCurrentFrameRect() const;
	void SetDirection(Direction dir) { currentDirection = dir; }
	// 프레임 인덱스 업데이트 (Play() 내부에서 증가시킨다 가정)
	void SetFrameIndex(std::size_t idx) { currentFrameIndex = idx; }
};
