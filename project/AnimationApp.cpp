#include "stdafx.h"
#include "AnimationApp.h"

AnimationApp::AnimationApp()
    : window(sf::VideoMode(800, 600), "SFML Animation Tool")
{

    texture.loadFromFile("graphics/Link.png");
    sprite.setTexture(texture);
    sprite.setPosition(0.f, 0.f);
    animSprite.setTexture(texture);
    sprite.setScale(2.f, 2.f);
    ui.SetSpriteSheetTexture(&texture);
    
    currentClip.name = "Link_idle";
    currentClip.loop = true;

    player = new AnimationPlayer(&currentClip);
    player->Play();
    player->SetClip(&currentClip);
    //sprite.setTextureRect(player->GetCurrentFrameRect());
    //sf::RectangleShape backButton;
    //sf::Text backText;
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
void AnimationApp::Run()
{
    sprite.setTexture(texture);
    sprite.setPosition(0.f, 0.f);
    sprite.setScale(1.f, 1.f);
    sprite.setTextureRect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));


    if (!font.loadFromFile("fonts/DS-DIGIT.TTF"))
    {
        std::cerr << "⚠ 폰트 로딩 실패\n";
    }

    if (player)
    {
        // sprite.setTextureRect(player->GetCurrentFrameRect());
    }

    sf::Text frameInfoText;
    frameInfoText.setFont(font);
    frameInfoText.setCharacterSize(16);
    frameInfoText.setFillColor(sf::Color::White);
    frameInfoText.setPosition(300.f, 550.f);

    while (window.isOpen())
    {
        //AnimationClip clip;
        float duration = 0.1f;
        //sf::IntRect inputRect(0, 0, 32, 32);





        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords
                ({ event.mouseButton.x, event.mouseButton.y });
                // ✅ 버튼 클릭 처리 먼저

                // 1) UI 먼저
                if (ui.HandleClick(mousePos,
                    currentClip,
                    hasPreview ? previewRect : sf::IntRect(),  // 선택 없으면 빈 rect 전달
                    frameDuration))
                {
                    // UI 버튼이 눌린 경우 후처리
                    player->SetClip(&currentClip);
                    player->SetCurrentFrameIndex(ui.GetManualFrameIndex());
                    if (!currentClip.frames.empty())
                        sprite.setTextureRect(player->GetCurrentFrameRect());
                    hasPreview = false;  // 프레임 추가했다면 선택 해제
                    continue;            // 드래그 시작 X
                }
                // 2) 영역 드래그 시작
                isDragging = true;
                dragStartPos = currentDragPos = mousePos;

                // 클릭으로 프레임 추가 시도
                if (hasPreview)
                {
                    if (ui.HandleClick(mousePos, currentClip, previewRect, frameDuration))
                    {
                        player->SetCurrentFrameIndex(ui.GetManualFrameIndex());
                        sprite.setTextureRect(player->GetCurrentFrameRect());
                    }
                }
                else
                {
                    std::cout << "⚠ 선택된 영역이 없습니다.\n";
                    continue;
                }

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
        //window.draw(sprite);

        //2. 선택된 프레임 미리보기 보여주기
        if (hasPreview)
        {
            sf::Sprite previewSprite;
            previewSprite.setTexture(texture);
            previewSprite.setTextureRect(previewRect);
            previewSprite.setScale(5.f, 5.f); // 크기 확대
            previewSprite.setPosition(550.f, 450.f); // 오른쪽 위에 배치
            window.draw(previewSprite);
        }

        if (!hasPreview)
        {
            player->SetCurrentFrameIndex(ui.GetManualFrameIndex());

        }

        if (!currentClip.frames.empty())
        {
            const FrameData& currentFrame = player->GetCurrentFrame();
            sf::IntRect rect = currentFrame.rect;
            std::stringstream ss;
            ss << "Frame: " << player->GetCurrentFrameIndex()
                << "\nX: " << rect.left
                << " Y: " << rect.top
                << " W: " << rect.width
                << " H: " << rect.height;

            frameInfoText.setString(ss.str());
        }
        else
        {
            frameInfoText.setString("No frames loaded.");
        }


        if (isDragging || hasPreview)
        {
            sf::RectangleShape rectShape;
            sf::Vector2f pos(std::min(dragStartPos.x, currentDragPos.x),
                std::min(dragStartPos.y, currentDragPos.y));
            sf::Vector2f size(std::abs(currentDragPos.x - dragStartPos.x),
                std::abs(currentDragPos.y - dragStartPos.y));
            rectShape.setPosition(pos);
            rectShape.setSize(size);
            rectShape.setFillColor(hasPreview ? sf::Color(255, 255, 0, 60) : sf::Color(0, 0, 255, 60));
            rectShape.setOutlineColor(hasPreview ? sf::Color::Yellow : sf::Color::Blue);
            rectShape.setOutlineThickness(1.f);
            window.draw(rectShape);
        }

        window.draw(sheetSprite);   // 전체 시트 먼저
        // ... 드래그 박스 등 ...
        window.draw(animSprite);    // 현재 프레임
        ui.Render(window, currentClip);
        window.draw(frameInfoText);
        window.display();
    }

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
