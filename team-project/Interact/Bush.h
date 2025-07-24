#pragma once
#include "Interactable.h"
class Bush : public Interactable
{
protected:
	float shootTimer = 0.f;
	bool isShoot = false;
public:
	// Interactable��(��) ���� ��ӵ�
	void OnInteract() override;
	void Init() override;
	void Reset() override;
	void UpdateBeHavior(float dt) override;
	void Shoot();
};