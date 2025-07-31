#pragma once
#include "Enemy.h"
enum class moveType
{
	Random,
	Patrol,
};
enum class EnemyState
{
	None = -1,
	Patrol,
	Chase,
	ReturnToPatrol
};
class BasicEnemy :  public Enemy
{
protected:
	float moveTimer = 0.f;
	EnemyState state = EnemyState::Patrol;

public:
	// Enemy을(를) 통해 상속됨
	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void ChangeSprite();
	
};

