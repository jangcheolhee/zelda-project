#include "stdafx.h"
#include "BasicEnemy.h""

void BasicEnemy::Init()
{
	Enemy::Init();
	type = Types::Basic;
	speed = 100.f;
	hp = 40;
	
	// ���⼭ sprite texture �����ϱ�
	
}

void BasicEnemy::Reset()
{
	Enemy::Reset();
}

void BasicEnemy::UpdateBehavior(float dt)
{

}
