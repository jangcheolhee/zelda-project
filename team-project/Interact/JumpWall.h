#pragma once
#include "Interactable.h"
class JumpWall : public Interactable
{
public:

    

    // Interactable을(를) 통해 상속됨
    void OnInteract() override;
    void Init() override;
    void Reset() override;
    void SetDirection(Direction d) { direction = d; }

};

