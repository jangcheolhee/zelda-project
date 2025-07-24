#pragma once
#include "GameObject.h"
#include "HitBox.h"
#include "Defines.h"
class Player :  public GameObject
{


protected:
	Direction currentDirection = Direction::Down;
	std::size_t currentFrame = 0;
  // ������ ��ȯ �ð�
	
	     // �̵� �ӵ�
	std::map<Direction, std::vector<sf::IntRect>> animations;

	sf::Sprite body;
	sf::Texture* texture = nullptr;
	
	HitBox hitBox;
	int hp = 0;
	int maxHp = 0;
	bool isInteract = false;
	float elapsedTime = 0.f;
	float frameTime = 0.2f; // 예: 10fps
	float speed = 200.f;

public:
	Player(const std::string& name = "");
	virtual ~Player() = default;

	int GetHp() { return hp; }
	void SetHp(int hp) { this->hp = hp; }

	void SetIsInteract(bool b) { isInteract = b; }
	bool IsInteract() { return isInteract; }

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float rot) override;
	void SetScale(const sf::Vector2f& s) override;
	void SetOrigin(const sf::Vector2f& o) override;
	void SetOrigin(Origins preset) override;
	Direction GetDirection() { return currentDirection; }

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
	bool checkCollision(const HitBox& other);
	sf::Sprite& GetBody() { return body; } // getter
};

