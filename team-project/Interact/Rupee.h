#pragma once
#include "Interactable.h"
class Rupee : public Interactable
{
protected:
	int value;
public:

	
	void Init() override;
	void Reset() override;

	// Interactable을(를) 통해 상속됨
	void OnInteract() override;
};

