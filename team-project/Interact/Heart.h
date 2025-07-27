#pragma once
#include "Interactable.h"
class Heart :  public Interactable
{
public:
	void Init() override;
	void Reset() override;

	// Interactable을(를) 통해 상속됨
	void OnInteract() override;
};

