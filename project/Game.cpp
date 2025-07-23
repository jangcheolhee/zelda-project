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
    sheetTex.loadFromFile("graphics/Link.png"); // 실제 경로
    editorUI.SetSpriteSheetTexture(&sheetTex);
    // Scene 초기화
    SCENE_MGR.ChangeScene(SceneIds::Title);  // 예시
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