#pragma once
#include "Interactable.h"
class Bush : public Interactable
{
	// Interactable을(를) 통해 상속됨
	void OnInteract() override;
	void Init() override;
	void Reset() override;
};

