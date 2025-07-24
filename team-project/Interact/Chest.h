#pragma once
#include "Interactable.h"
class Chest : public Interactable
{
protected:
    bool available = false;
public:

    // Interactable��(��) ���� ��ӵ�
    void OnInteract() override;
    void Init() override;
    void Reset() override;
};

