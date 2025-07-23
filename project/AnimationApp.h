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

    AnimationClip currentClip;
    AnimationPlayer* player = nullptr;
    AnimationEditorUI ui;

    sf::IntRect inputRect;
    float frameDuration = 0.1f;

    sf::Vector2f dragStartPos;
    sf::Vector2f currentDragPos;
    bool isDragging = false;

    sf::IntRect previewRect;
    bool hasPreview = false;

public:
    AnimationApp();
    ~AnimationApp();

    void Run();
    void LoadClipFromCSV(const std::string& csvPath);
    void SaveClipToCSV(const AnimationClip& clip, const std::string& filename);
};

