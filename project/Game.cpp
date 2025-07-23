#include "stdafx.h"
#include "game.h"
#include "AnimationClipManager.h"
#include "SceneMgr.h"
#include "Player.h"
Player player;
Enemy enemy;

Game::Game():Scene(SceneIds::Title)
{
}

void Game::Init()
{
    ANI_CLIP_MGR.Load("animations/LinkMovementFrames.csv");
    sheetTex.loadFromFile("graphics/Link.png"); // ���� ���
    editorUI.SetSpriteSheetTexture(&sheetTex);
    // Scene �ʱ�ȭ
    SCENE_MGR.ChangeScene(SceneIds::Title);  // ����
}
void Game::Release()
{

}
void Game::Update(float dt)
{
    player.Update(dt);
    enemy.Update(dt);
}
void Game::Draw(sf::RenderWindow& window)
{
    player.Draw(window);
    enemy.Draw(window);
    editorUI.Render(window, clip);
}