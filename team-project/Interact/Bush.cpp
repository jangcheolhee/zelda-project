#include "stdafx.h"
#include "Bush.h"

void Bush::OnInteract()
{
	// �÷��̾�� �ν� ����ٰ� �����ϱ� -> boolŸ������ ���� ?
	// �÷��̾�� �ν� �� ���¸� �ذ�
	SetActive(false);
}

void Bush::Init()
{
	Interactable::Init();
}

void Bush::Reset()
{
	Interactable::Reset();
	body.setTexture(TEXTURE_MGR.Get("graphics/sprite_sheet.png"));
	body.setTextureRect({ 70,30,18,26 });

}
