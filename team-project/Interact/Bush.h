#pragma once
#include "Interactable.h"
class Bush : public Interactable
{
	// Interactable��(��) ���� ��ӵ�
	void OnInteract() override;
	void Init() override;
	void Reset() override;
};

