#pragma once
#include "Interactable.h"
class Chest : public Interactable
{
protected:
    bool available = false;
public:

    // Interactable을(를) 통해 상속됨
    void OnInteract() override;
    void Init() override;
    void Reset() override;
};

