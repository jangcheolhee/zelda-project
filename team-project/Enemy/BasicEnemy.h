#pragma once
#include "Enemy.h"
class BasicEnemy :  public Enemy
{
protected:
	float moveTimer = 0.f;
public:



	// Enemy을(를) 통해 상속됨
	void Init() override;
	void Reset() override;
	void UpdateBehavior(float dt) override;

	void ChangeSprite();
};

