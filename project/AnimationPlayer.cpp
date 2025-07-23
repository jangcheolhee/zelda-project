#include "stdafx.h"
#include "AnimationClip.h"
#include "AnimationPlayer.h"
#include <algorithm>

AnimationApp::AnimationApp()
    : window(sf::VideoMode(800, 600), "SFML Animation Tool")
{

    texture.loadFromFile("graphics/Link.png");
    sprite.setTexture(texture);
    sprite.setPosition(200.f, 150.f);
    sprite.setScale(2.f, 2.f);

    currentClip.name = "default_clip";
    currentClip.loop = true;

    player = new AnimationPlayer(&currentClip);
    player->Play();
    player->SetClip(&currentClip);
    sprite.setTextureRect(player->GetCurrentFrameRect());
    sf::RectangleShape backButton;
    sf::Text backText;
}

AnimationApp::~AnimationApp()
{
    delete player;
}

void AnimationApp::LoadClipFromCSV(const std::string& csvPath)
{
    std::ifstream file(csvPath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open CSV file: " << csvPath << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // skip header

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string value;
        int x, y, w, h;

        std::getline(ss, value, ','); // Direction
        std::getline(ss, value, ','); // Frame
        std::getline(ss, value, ','); x = std::stoi(value);
        std::getline(ss, value, ','); y = std::stoi(value);
        std::getline(ss, value, ','); w = std::stoi(value);
        std::getline(ss, value, ','); h = std::stoi(value);

        FrameData frame;
        frame.rect = sf::IntRect(x, y, w, h);
        frame.duration = frameDuration;

        currentClip.frames.push_back(frame);
    }

    file.close();
}

void AnimationApp::SaveClipToCSV(const AnimationClip& clip, const std::string& filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "파일 열기 실패: " << filename << std::endl;
        return;
    }

    file << "Direction,Frame,X,Y,Width,Height\n";

    for (size_t i = 0; i < clip.frames.size(); ++i)
    {
        const FrameData& frame = clip.frames[i];
        file << "None," << i << ","
            << frame.rect.left << ","
            << frame.rect.top << ","
            << frame.rect.width << ","
            << frame.rect.height << "\n";
    }

    file.close();
    std::cout << "CSV 저장 완료: " << filename << std::endl;
}

void AnimationApp::Run()
{
    sprite.setTexture(texture);
    sprite.setPosition(0.f, 0.f);
    sprite.setScale(1.f, 1.f);
    sprite.setTextureRect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));

    sf::Font font;
    font.loadFromFile("fonts/DS-DIGIT.TTF");

    sf::Text frameInfoText;
    frameInfoText.setFont(font);
    frameInfoText.setCharacterSize(16);
    frameInfoText.setFillColor(sf::Color::White);
    frameInfoText.setPosition(150.f, 10.f);

    while (window.isOpen())
    {
        //AnimationClip clip;
        float duration = 0.1f;
        //sf::IntRect inputRect(0, 0, 32, 32);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
                if (hasPreview)
                {
                    ui.HandleClick(mousePos, currentClip, previewRect, frameDuration);

                }
                else {

                    std::cout << "⚠ 선택된 영역이 없습니다.\n";
                }
                isDragging = true;
                dragStartPos = mousePos;
                currentDragPos = mousePos;
            }

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Right)
                {
                    player->NextFrame();
                    sprite.setTextureRect(player->GetCurrentFrameRect());
                }
                else if (event.key.code == sf::Keyboard::Left)
                {
                    player->PrevFrame();
                    sprite.setTextureRect(player->GetCurrentFrameRect());
                }
                else if (event.key.code == sf::Keyboard::E && hasPreview)
                {
                    FrameData frame;
                    frame.rect = previewRect;
                    frame.duration = frameDuration;
                    currentClip.frames.push_back(frame);
                    player->SetClip(&currentClip);
                    player->SetCurrentFrameIndex((int)currentClip.frames.size() - 1);
                    sprite.setTextureRect(player->GetCurrentFrameRect());
                    hasPreview = false;
                }
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                isDragging = true;
                dragStartPos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
                currentDragPos = dragStartPos;
                hasPreview = false;
                sprite.setTextureRect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));
            }

            if (event.type == sf::Event::MouseButtonReleased)
            {
                isDragging = false;
                sf::Vector2f endPos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });

                int left = std::min(dragStartPos.x, endPos.x);
                int top = std::min(dragStartPos.y, endPos.y);
                int width = std::abs(endPos.x - dragStartPos.x);
                int height = std::abs(endPos.y - dragStartPos.y);

                if (width > 1 && height > 1)
                {
                    previewRect = sf::IntRect(left, top, width, height);
                    hasPreview = true;
                }
            }

            if (event.type == sf::Event::MouseMoved && isDragging)
            {
                currentDragPos = window.mapPixelToCoords({ event.mouseMove.x, event.mouseMove.y });
            }
        }

        window.clear();
        window.draw(sprite);

        if (isDragging || hasPreview)
        {
            sf::RectangleShape rect;
            sf::Vector2f pos(std::min(dragStartPos.x, currentDragPos.x), std::min(dragStartPos.y, currentDragPos.y));
            sf::Vector2f size(std::abs(currentDragPos.x - dragStartPos.x), std::abs(currentDragPos.y - dragStartPos.y));
            rect.setPosition(pos);
            rect.setSize(size);
            rect.setFillColor(hasPreview ? sf::Color(255, 255, 0, 60) : sf::Color(0, 0, 255, 60));
            rect.setOutlineColor(hasPreview ? sf::Color::Yellow : sf::Color::Blue);
            rect.setOutlineThickness(1.f);
            window.draw(rect);
        }

        ui.Render(window, currentClip);
        window.display();
    }
}

void AnimationPlayer::SetCurrentFrameIndex(int index)
{
    if (!clip || clip->frames.empty())
        return;

    currentFrameIndex = std::clamp(index, 0, static_cast<int>(clip->frames.size()) - 1);
    elapsedTime = 0.f;
}

void AnimationPlayer::SetClip(AnimationClip* newClip)
{
    this->clip = clip;
    currentFrameIndex = 0;
    elapsedTime = 0.f;
}
