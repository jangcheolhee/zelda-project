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
	body.setTexture(TEXTURE_MGR.Get("graphics/sprite_sheet.png"));
	body.setTextureRect({ 50,25,18,26 });
}

void BasicEnemy::UpdateBehavior(float dt)
{

}
