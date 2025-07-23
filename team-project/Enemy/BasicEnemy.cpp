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
	body.setTexture(TEXTURE_MGR.Get("graphics/sprite_sheet.png"));
	body.setTextureRect({ 26,0,61,125 });
}

void BasicEnemy::UpdateBehavior(float dt)
{

}
