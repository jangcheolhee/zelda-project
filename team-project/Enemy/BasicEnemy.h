#pragma once
#include "Enemy.h"
class BasicEnemy :  public Enemy
{
protected:
	float moveTimer = 0.f;
public:



	// Enemy��(��) ���� ��ӵ�
	void Init() override;
	void Reset() override;
	void UpdateBehavior(float dt) override;

	void ChangeSprite();
};

