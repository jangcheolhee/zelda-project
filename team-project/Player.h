#pragma once
#include "GameObject.h"
#include "HitBox.h"
#include "Enemy.h"

class Player :  public GameObject
{


protected:
	int rupee = 0;
	Direction currentDirection = Direction::Down;
	size_t currentFrame = 0;
	float frameTime = 0.2f;    // ������ ��ȯ �ð�
	float elapsedTime = 0.f;
	float speed = 200.f;        // �̵� �ӵ�
	sf::Vector2f velocity = { 0.f, 0.f };
	std::map<Direction, std::vector<sf::IntRect>> animations;

	sf::Sprite body;
	sf::Texture* texture = nullptr;
	
	HitBox hitBox;
	int hp = 0;
	int maxHp = 0;

	bool movable = true;
	bool isInteract = false;
	bool wantsToInteract = false;
	

public:
	Player(const std::string& name = "");
	virtual ~Player() = default;

	int GetHp() { return hp; }
	void SetHp(int hp) { this->hp = hp; }

	void SetMovable(bool b) { movable = b; }

	void SetIsInteract(bool b) { isInteract = b; }
	bool IsInteract() { return isInteract; }
	
	// 충돌 체크 함수-----------
	void OnCollide(Enemy* enemy);
	bool WantsToInteract(){ return wantsToInteract; }

	void AddRupee(int i) { rupee += i; }
	//--------------

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