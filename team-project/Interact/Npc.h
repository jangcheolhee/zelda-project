#pragma once
#include "Interactable.h"
class Npc : public Interactable
{
protected:

public:

	// Interactable��(��) ���� ��ӵ�
	void OnInteract() override;
	void Init();
	void Reset();

};

