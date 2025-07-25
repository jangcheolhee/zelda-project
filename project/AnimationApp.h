#pragma once
#include "stdafx.h"
#include "AnimationApp.h" 
#include "AnimationPlayer.h"
#include "AnimationClip.h"
#include "AnimationEditorUI.h"
#include <algorithm>

class AnimationApp
{
private:
    sf::RenderWindow window;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Sprite sheetSprite;   // 전체 시트 전용
    sf::Sprite animSprite;    // 플레이어/현재 프레임 표시용
    // ▼ UI 요소
    sf::Font font;
   

    // ▼ 애니메이션 관련
    AnimationClip currentClip;
    AnimationPlayer* player;

    // ▼ 에디터 UI
    AnimationEditorUI ui;

    // ▼ 마우스 드래그 상태
    bool isDragging = false;
    sf::Vector2f dragStartPos;
    sf::Vector2f currentDragPos;

    // ▼ 선택된 프리뷰 영역
    sf::IntRect previewRect;
    bool hasPreview = false;

    // ▼ 프레임 지속시간 기본값
    float frameDuration = 0.1f;

public:
    AnimationApp();
    ~AnimationApp();

    void Run();
    void LoadClipFromCSV(const std::string& csvPath);
    void SaveClipToCSV(const AnimationClip& clip, const std::string& path);

};

