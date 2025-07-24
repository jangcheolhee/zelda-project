#include "stdafx.h"
#include "BasicEnemy.h""

void BasicEnemy::Init()
{
	Enemy::Init();
	type = Types::Basic;
	speed = 100.f;
	hp = 40;
	
	

	
}

void BasicEnemy::Reset()
{
	Enemy::Reset();
	// 여기서 sprite texture 변경하기
	body.setTexture(TEXTURE_MGR.Get("graphics/Enemy_sheet.png"));
	
	body.setTextureRect({11,682,24,36 });
}

void BasicEnemy::UpdateBehavior(float dt)
{

}
