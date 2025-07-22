#include "stdafx.h"
#include "BasicEnemy.h""

void BasicEnemy::Init()
{
	Enemy::Init();
	type = Types::Basic;
	speed = 100.f;
	hp = 40;
	
	// 여기서 sprite texture 변경하기
	
}

void BasicEnemy::Reset()
{
	Enemy::Reset();
}

void BasicEnemy::UpdateBehavior(float dt)
{

}
