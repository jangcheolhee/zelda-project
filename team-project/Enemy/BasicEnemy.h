#pragma once
#include "Enemy.h"
class BasicEnemy :  public Enemy
{
public:



	// Enemy��(��) ���� ��ӵ�
	void Init() override;
	void Reset() override;
	void UpdateBehavior(float dt) override;
};

