#pragma once
#include "GameObject.h"
#include "HitBox.h"

class Player :  public GameObject
{
enum class Direction { Down, Left, Right, Up };

protected:
	Direction currentDirection = Direction::Down;
	size_t currentFrame = 0;
	float frameTime = 0.2f;    // ������ ��ȯ �ð�
	float elapsedTime = 0.f;
	float speed = 200.f;        // �̵� �ӵ�
	std::map<Direction, std::vector<sf::IntRect>> animations;

	sf::Sprite body;
	sf::Texture* texture = nullptr;
	
	HitBox hitBox;
	int hp = 0;
	int maxHp = 0;
	HitBox hitBox;

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

