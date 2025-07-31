#pragma once
#include "GameObject.h"
#include "HitBox.h"
#include "Defines.h"
#include "Enemy.h"
#include "SceneGame.h"
class SceneGame;

enum class PlayerState
{
	Idle,
	Walk,
	Push,
	Attack,
};
class HUD;
class Player :  public GameObject
{
protected:
	std::vector<GameObject*>* pushTargets = nullptr;
	std::list<GameObject*> gameObjects;
	SceneGame* sceneGame = nullptr;
	PlayerState playerState = PlayerState::Idle;
	
	int rupee = 0;
	sf::Texture* swordTexture = nullptr;
	std::vector<sf::IntRect> attackFrames;
	float attackElapsed = 0.f;
	int attackFrameIndex = 0;
	bool isAttacking = false;
	sf::Vector2f moveDir = { 0.f, 0.f };
	float frameTime = 0.2f;   
	size_t currentFrame = 0;
	float elapsedTime = 0.f;
	float speed = 50.f;        
	float attackFrameTime = 1.f / 20.f;

	sf::Vector2f velocity = { 0.f, 0.f };
	std::map<Direction, std::vector<sf::IntRect>> animations;
	std::map<Direction, std::vector<sf::IntRect>> attackAnimations;
	std::map<Direction, std::vector<sf::IntRect>> pushingAnimations;

	sf::Sprite body;
	sf::Texture* texture = nullptr;
	
	HitBox hitBox;
	int hp = 6;
	int maxHp = 6;
	HUD* hud = nullptr;
	bool isInvincible = false;
	float invincibleTime = 1.0f;        // 무적 지속 시간 (초)
	float invincibleElapsed = 0.0f;     // 무적 상태 경과 시간

	HitBox swordHitBox;
	bool swordHitBoxActive = false;

	float timeSinceLastDamage = 0.f;
	float damageCooldown = 0.5f;

	bool movable = true;
	bool isInteract = false;
	bool wantsToInteract = false;
	
	Direction currentDirection = Direction::Down;
	Direction heldDirection = Direction::None;
	
	bool isMovingLeft = false;
	bool isRightPressed = false;
	bool isLeftPressed = false;
	sf::Vector2f previousPosition;

	bool wasPushing = false;   // 이전 프레임에서 밀고 있었는지
	bool isPushing = false;         // 밀고 있는 중인지 여부
	float pushTimer = 0.f;          // 밀고 있다고 판단되기까지 누적 시간
	const float pushThreshold = 1.f; // 1초 이상 밀면 isPushing 활성화

	GameObject* currentInteractable = nullptr;  // 현재 상호작용 가능한 오브젝트
	bool showInteractionUI = false;
	
public:
	
	sf::Vector2f GetPos() {
		return previousPosition;
	}
	Player(const std::string& name = "");
	virtual ~Player() = default;

	int GetMaxHp() { return maxHp; }
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

	sf::FloatRect GetGlobalBounds() const override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	sf::Sprite& GetBody() { return body; } // getter
	void TakeDamageIfPossible(int damage);
	void OnDamage(int damage);
	void UpdateFixedHitBox();
	void HandleMovement(float dt);
	void SetHUD(HUD* hudptr) { hud = hudptr; }
	bool IsAttacking() const;
	void Heal(int amount);
	void UpdateAnimation(float dt);
	void SetSceneGame(SceneGame* scene);
	HitBox GetHitBox() { return hitBox; }

	bool checkCollision(const HitBox& other);
	void CheckInteractions();
	void TriggerInteraction(GameObject* obj);
	GameObject* GetNearestInteractable() const { return currentInteractable; }
	bool CanInteract() const { return currentInteractable != nullptr; }
	bool isNpcTalk = 0;
};