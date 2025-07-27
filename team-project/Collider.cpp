#include "stdafx.h"
#include "Collider.h"

void Collider::InitScene(Scene* scene)
{
    this->scene = scene;
    this->player = (Player*)SCENE_MGR.GetCurrentScene()->FindGameObject("Player");

}
