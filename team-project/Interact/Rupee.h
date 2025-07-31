#pragma once
#include "Interactable.h"
class Rupee : public Interactable
{
protected:
	int value = 0;
	float life = 0;
public:

	
	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	// Interactable을(를) 통해 상속됨
	void OnInteract() override; 
};

