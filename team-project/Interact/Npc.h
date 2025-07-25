#pragma once
#include "Interactable.h"
class Npc : public Interactable
{
protected:

public:

	// Interactable을(를) 통해 상속됨
	void OnInteract() override;
	void Init();
	void Reset();

};

