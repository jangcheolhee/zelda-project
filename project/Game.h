#pragma once
#include "stdafx.h"
#include "Scene.h"
class Game :
    public Scene
{
protected:
    sf::Texture sheetTex;
    AnimationClip clip;              // ���� ���
    AnimationEditorUI editorUI;
public:
    Game();
    virtual ~Game()=default;
    void Init();
    void Release();
    void Update(float dt);
    void Draw(sf::RenderWindow& window);
};

