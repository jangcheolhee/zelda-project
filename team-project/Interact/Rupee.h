#pragma once
#include "Interactable.h"
class Rupee : public Interactable
{
protected:
	int value;
public:

	
	void Init() override;
	void Reset() override;

	// Interactable��(��) ���� ��ӵ�
	void OnInteract() override;
};

