#pragma once
#include "Interactable.h"
class JumpWall : public Interactable
{
public:

    

    // Interactable��(��) ���� ��ӵ�
    void OnInteract() override;
    void Init() override;
    void Reset() override;
    void SetDirection(Direction d) { direction = d; }

};

