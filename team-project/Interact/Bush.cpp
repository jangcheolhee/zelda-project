#include "stdafx.h"
#include "Bush.h"
#include "Player.h"

void Bush::OnInteract()
{
	// �÷��̾�� �ν� ����ٰ� �����ϱ� -> boolŸ������ ���� ?
	player->SetIsInteract(true);
	isShoot = true;
	// �÷��̾�� �ν� �� ���¸� �ذ�
}

void Bush::Init()
{
	Interactable::Init();
	
}

void Bush::Reset()
{
	Interactable::Reset();
	body.setTexture(TEXTURE_MGR.Get("graphics/Overworld.png"));
	body.setTextureRect({ 304,57,16,16 });
	SetOrigin(Origins::ML);
	type = Type::Throw;

}
