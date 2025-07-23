#pragma once
#include "stdafx.h"
#include "AnimationClip.h"

class AnimationEditorUI
{
private:
    sf::Font font;
    sf::RectangleShape addFrameButton, saveButton;
    sf::RectangleShape prevFrameButton, nextFrameButton;
    sf::Text addFrameText, saveText,prevText,nextText;
    int manualFrameIndex = 0;
    sf::Text frameLabel;
    sf::RectangleShape loadButton;
    sf::Text loadText;
    sf::Sprite framePreview;
    sf::RectangleShape backButton;
    sf::Text backText;

public:

    void CenterText(sf::Text& text, const sf::RectangleShape& box)
    {
        sf::FloatRect textBounds = text.getLocalBounds();
        sf::Vector2f boxPos = box.getPosition();
        sf::Vector2f boxSize = box.getSize();

        text.setPosition(
            boxPos.x + (boxSize.x - textBounds.width) / 2.f - textBounds.left,
            boxPos.y + (boxSize.y - textBounds.height) / 2.f - textBounds.top
        );
    }

    void SetSpriteSheetTexture(sf::Texture* texture) {
        framePreview.setTexture(*texture);
    }
    void SaveClipToCSV(const AnimationClip& clip, const std::string& filename)
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "CSV 저장 실패: " << filename << std::endl;
            return;
        }

        file << "Direction,Frame,X,Y,Width,Height\n";

        for (size_t i = 0; i < clip.frames.size(); ++i)
        {
            const FrameData& frame = clip.frames[i];
            file << "Link," << i << ","
                << frame.rect.left << ","
                << frame.rect.top << ","
                << frame.rect.width << ","
                << frame.rect.height << "\n";
        }

        file.close();
        std::cout << "CSV 저장 완료: " << filename << std::endl;
    }

    AnimationEditorUI() {
        font.loadFromFile("fonts/DS-DIGIT.TTF");
        addFrameButton.setFillColor(sf::Color::Blue);
        loadText.setFillColor(sf::Color::Black);
        addFrameText.setFillColor(sf::Color::White);
        saveText.setFillColor(sf::Color::White);
        prevText.setFillColor(sf::Color::White);
        nextText.setFillColor(sf::Color::White);
        frameLabel.setFillColor(sf::Color::White);
        addFrameButton.setSize({ 120, 30 });

        loadButton.setSize({ 120, 30 });
        loadButton.setFillColor(sf::Color::Yellow);
        loadText.setFont(font);
        loadText.setString("LOAD ANIMATION");
        loadText.setCharacterSize(16);
        loadText.setFillColor(sf::Color::Black);

        saveButton.setSize({ 120, 30 });
        saveButton.setPosition(10, 50);
        saveButton.setFillColor(sf::Color::Green);

        prevFrameButton.setSize({ 30, 30 });
        prevFrameButton.setPosition(10, 90);
        prevFrameButton.setFillColor(sf::Color(100, 100, 100));

        nextFrameButton.setSize({ 30, 30 });
        nextFrameButton.setPosition(100, 90);
        nextFrameButton.setFillColor(sf::Color(100, 100, 100));

        addFrameText.setFont(font);
        addFrameText.setString("Add Frame");
        addFrameText.setCharacterSize(16);
        addFrameText.setPosition(120, 12);

        saveText.setFont(font);
        saveText.setString("Save Animation");
        saveText.setCharacterSize(16);
        saveText.setPosition(15, 52);

        prevText.setFont(font);
        prevText.setString("<");
        prevText.setCharacterSize(18);
        prevText.setPosition(15, 90);

        nextText.setFont(font);
        nextText.setString(">");
        nextText.setCharacterSize(18);
        nextText.setPosition(105, 90);
   
    }
   

    void Render(sf::RenderWindow& window,const AnimationClip& clip) 
    {
        sf::Vector2f windowSize = window.getView().getSize();

        const float padding = 10.f;
        const float buttonWidth = 120.f;
        const float buttonHeight = 30.f;
        const float buttonSpacing = 8.f;

        // 기준점 (오른쪽 아래)
        float baseX = windowSize.x - buttonWidth - padding;
        float baseY = windowSize.y - (buttonHeight + buttonSpacing) * 4 - padding;

        // 버튼 위치 재설정
        loadButton.setPosition(baseX, baseY);
        addFrameButton.setSize({ 120, 30 });
        addFrameButton.setPosition(baseX, baseY + (buttonHeight + buttonSpacing) * 1);
        saveButton.setPosition(baseX, baseY + (buttonHeight + buttonSpacing) * 2);
        prevFrameButton.setPosition(baseX, baseY + (buttonHeight + buttonSpacing) * 3);
        nextFrameButton.setPosition(baseX + buttonWidth - 30.f, baseY + (buttonHeight + buttonSpacing) * 3);

        // 텍스트 위치 재설정
        loadText.setPosition(loadButton.getPosition().x + 5.f, loadButton.getPosition().y + 5.f);
        addFrameText.setPosition(addFrameButton.getPosition().x + 5.f, addFrameButton.getPosition().y + 5.f);
        saveText.setPosition(saveButton.getPosition().x + 5.f, saveButton.getPosition().y + 5.f);
        // 프레임 인덱스 텍스트
        std::stringstream ss;
        ss << manualFrameIndex + 1 << "/" << clip.frames.size();
        frameLabel.setString(ss.str());
        // 2. 폰트, 크기, 색상 설정 (안 보이면 반드시 필요)
        frameLabel.setFont(font);
        frameLabel.setCharacterSize(14);
        frameLabel.setFillColor(sf::Color::White);

        frameLabel.setPosition
        (
            saveButton.getPosition().x + (saveButton.getSize().x / 2.f) - 10.f,
            saveButton.getPosition().y - 20.f
        );
        if (!clip.frames.empty())
        {
            // clip.frames.size() > 0 일 때만 clamp() 호출 안전
            manualFrameIndex = std::clamp(manualFrameIndex, 0, static_cast<int>(clip.frames.size()) - 1);

            const FrameData& currentFrame = clip.frames[manualFrameIndex];
            framePreview.setTextureRect(currentFrame.rect);
            framePreview.setScale(3.f, 3.f);
            framePreview.setPosition(20.f, window.getView().getSize().y - 150.f);
            window.draw(framePreview);
        }
        else
        {
            manualFrameIndex = 0;
        }

        // Draw
        window.draw(addFrameButton);
        window.draw(saveButton);
        window.draw(prevFrameButton);
        window.draw(nextFrameButton);
        window.draw(addFrameText);
        window.draw(saveText);
        window.draw(prevText);
        window.draw(nextText);
        window.draw(loadButton);
        window.draw(loadText);
        window.draw(frameLabel);

        CenterText(loadText, loadButton);
        CenterText(addFrameText, addFrameButton);
        CenterText(saveText, saveButton);
        CenterText(prevText, prevFrameButton);
        CenterText(nextText, nextFrameButton);
    }

    bool isMouseOver(const sf::RectangleShape& rect, const sf::Vector2f& mousePos) {
        return rect.getGlobalBounds().contains(mousePos);
    }

    bool HandleClick(const sf::Vector2f& mousePos, AnimationClip& clip, const sf::IntRect& inputRect, float duration) {
       
        if (isMouseOver(addFrameButton, mousePos)) {
            if (inputRect.width > 0 && inputRect.height > 0) {
                FrameData frame{ inputRect, duration };
                clip.frames.push_back(frame);
                std::cout << "프레임 추가됨! 총 개수: " << clip.frames.size() << std::endl;
            }
            else {
                std::cout << "⚠ 유효하지 않은 프레임 영역입니다.\n";
            }
            return true;
        }

        if (isMouseOver(saveButton, mousePos)) {
            std::string jsonPath = "animations/" + clip.name + ".json";

            std::ifstream ifs(jsonPath);
            if (ifs.is_open()) {
                nlohmann::json j;
                ifs >> j;
                clip.frames.clear();
                clip.name = j["name"];
                clip.loop = j["loop"];
                for (const auto& f : j["frames"]) {
                    FrameData frame;
                    frame.rect.left = f["x"];
                    frame.rect.top = f["y"];
                    frame.rect.width = f["w"];
                    frame.rect.height = f["h"];
                    frame.duration = f["duration"];
                    clip.frames.push_back(frame);
                }
                std::cout << "애니메이션 로드 완료: " << jsonPath << std::endl;
            }
            else {
                std::cerr << "로드 실패: " << jsonPath << std::endl;
            }
            return true;
        }

        if (isMouseOver(prevFrameButton, mousePos)) {
            if (!clip.frames.empty()) {
                manualFrameIndex = std::max(0, manualFrameIndex - 1);
            return true;
            }
        }

        if (isMouseOver(nextFrameButton, mousePos)) {
            if (!clip.frames.empty()) {
                manualFrameIndex = std::min(manualFrameIndex + 1, static_cast<int>(clip.frames.size()) - 1);
            return true;
            }
        }

        return false;
    }
    int GetManualFrameIndex() const {
        return manualFrameIndex;
    }
    static void SaveAnimationClip(const AnimationClip& clip, const std::string& filename) {
        nlohmann::json j;
        j["name"] = clip.name;
        j["loop"] = clip.loop;
        for (const auto& frame : clip.frames) {
            j["frames"].push_back({
                {"x", frame.rect.left},
                {"y", frame.rect.top},
                {"w", frame.rect.width},
                {"h", frame.rect.height},
                {"duration", frame.duration}
                });
        }

        std::ofstream ofs(filename);
        ofs << j.dump(4);
    }

};

