#pragma once
#include "stdafx.h"
#include "AnimationClip.h"

class AnimationEditorUI
{
private:
    // --- UI 요소 ---
    sf::Font font;

    sf::RectangleShape addFrameButton, saveButton;
    sf::RectangleShape prevFrameButton, nextFrameButton;
    sf::RectangleShape loadButton;

    sf::Text addFrameText, saveText, prevText, nextText;
    sf::Text loadText;
    sf::Text frameLabel;

    // --- 미리보기 ---
    sf::Sprite sheetSprite;    // 전체 시트
    sf::Sprite previewSprite;  // 현재 프레임 확대 미리보기

    // --- 상태 ---
    int manualFrameIndex = 0;

    void CenterText(sf::Text& text, const sf::RectangleShape& box)
    {
        sf::FloatRect tb = text.getLocalBounds();
        sf::Vector2f pos = box.getPosition();
        sf::Vector2f size = box.getSize();

        text.setPosition(
            pos.x + (size.x - tb.width) * 0.5f - tb.left,
            pos.y + (size.y - tb.height) * 0.5f - tb.top
        );
    }

public:

    AnimationEditorUI()
    {
        font.loadFromFile("fonts/DS-DIGIT.TTF");

        // 버튼 공통
        const sf::Vector2f btnSize(120.f, 30.f);

        addFrameButton.setSize(btnSize);
        addFrameButton.setFillColor(sf::Color::Blue);

        saveButton.setSize(btnSize);
        saveButton.setFillColor(sf::Color::Green);

        loadButton.setSize(btnSize);
        loadButton.setFillColor(sf::Color::Yellow);

        prevFrameButton.setSize({ 30.f, 30.f });
        prevFrameButton.setFillColor(sf::Color(100, 100, 100));
        nextFrameButton.setSize({ 30.f, 30.f });
        nextFrameButton.setFillColor(sf::Color(100, 100, 100));

        // 텍스트
        auto setupText = [&](sf::Text& t, const std::string& str, unsigned size, sf::Color col)
            {
                t.setFont(font);
                t.setString(str);
                t.setCharacterSize(size);
                t.setFillColor(col);
            };

        setupText(addFrameText, "Add Frame", 16, sf::Color::White);
        setupText(saveText, "Save Animation", 16, sf::Color::White);
        setupText(loadText, "LOAD ANIMATION", 16, sf::Color::Black);
        setupText(prevText, "<", 18, sf::Color::White);
        setupText(nextText, ">", 18, sf::Color::White);
        setupText(frameLabel, "", 14, sf::Color::White);
    }

    void SetSpriteSheetTexture(sf::Texture* tex)
    {
        if (!tex) return;
        sheetSprite.setTexture(*tex);      // 전체 시트
        previewSprite.setTexture(*tex);    // 미리보기 (Rect만 바꿀 것)
    }

    // --------- RENDER ---------
    void Render(sf::RenderWindow& window, const AnimationClip& clip)
    {
        sf::Vector2f windowSize = window.getView().getSize();

        // 배치용 상수
        const float padding = 10.f;
        const float buttonWidth = 120.f;
        const float buttonHeight = 30.f;
        const float buttonSpacing = 8.f;

        // 기준점 (오른쪽 아래)
        float baseX = windowSize.x - buttonWidth - padding;
        float baseY = windowSize.y - (buttonHeight + buttonSpacing) * 4 - padding;

        // 버튼 위치
        loadButton.setPosition(baseX, baseY);
        addFrameButton.setPosition(baseX, baseY + (buttonHeight + buttonSpacing) * 1);
        saveButton.setPosition(baseX, baseY + (buttonHeight + buttonSpacing) * 2);
        prevFrameButton.setPosition(baseX, baseY + (buttonHeight + buttonSpacing) * 3);
        nextFrameButton.setPosition(baseX + buttonWidth - 30.f, baseY + (buttonHeight + buttonSpacing) * 3);

        // 텍스트 위치(대략), 이후 CenterText로 정중앙
        loadText.setPosition(loadButton.getPosition().x + 5.f, loadButton.getPosition().y + 5.f);
        addFrameText.setPosition(addFrameButton.getPosition().x + 5.f, addFrameButton.getPosition().y + 5.f);
        saveText.setPosition(saveButton.getPosition().x + 5.f, saveButton.getPosition().y + 5.f);
        prevText.setPosition(prevFrameButton.getPosition().x + 5.f, prevFrameButton.getPosition().y + 3.f);
        nextText.setPosition(nextFrameButton.getPosition().x + 8.f, nextFrameButton.getPosition().y + 3.f);

        // 인덱스 정리 & 라벨 문자열/위치 갱신
        ClampFrameIndex(clip);

        frameLabel.setString(
            std::to_string(manualFrameIndex + 1) + "/" + std::to_string(clip.frames.size())
        );
        const auto lb = frameLabel.getLocalBounds();
        frameLabel.setPosition(
            saveButton.getPosition().x + saveButton.getSize().x * 0.5f - frameLabel.getLocalBounds().width * 0.5f,
            saveButton.getPosition().y - 22.f
        );

        // 버튼 텍스트 중앙 정렬
        CenterText(loadText, loadButton);
        CenterText(addFrameText, addFrameButton);
        CenterText(saveText, saveButton);
        CenterText(prevText, prevFrameButton);
        CenterText(nextText, nextFrameButton);

       

        // 현재 프레임 미리보기
        if (!clip.frames.empty())
        {
            const FrameData& cur = clip.frames[manualFrameIndex];
            previewSprite.setTextureRect(cur.rect);
            previewSprite.setScale(3.f, 3.f);
            previewSprite.setPosition(20.f, window.getView().getSize().y - 150.f);
            window.draw(previewSprite);
        }

        // UI
        window.draw(addFrameButton);
        window.draw(saveButton);
        window.draw(prevFrameButton);
        window.draw(nextFrameButton);
        window.draw(loadButton);

        window.draw(addFrameText);
        window.draw(saveText);
        window.draw(prevText);
        window.draw(nextText);
        window.draw(loadText);
        window.draw(frameLabel);
    }

    // --------- INPUT ---------
    bool HandleClick(const sf::Vector2f& mousePos, AnimationClip& clip,
        const sf::IntRect& inputRect, float duration)
    {
        if (isMouseOver(addFrameButton, mousePos))
        {
            if (inputRect.width > 0 && inputRect.height > 0)
            {
                clip.frames.push_back({ inputRect, duration });
                manualFrameIndex = static_cast<int>(clip.frames.size()) - 1;
                ClampFrameIndex(clip);
                return true;
            }
            else
            {
                std::cout << "⚠ 유효하지 않은 프레임 영역입니다.\n";
            }
            return true;
        }

        if (isMouseOver(saveButton, mousePos))
        {
            std::string jsonPath = "animations/" + clip.name + ".json";
            std::string csvPath = "animations/" + clip.name + ".csv";

            SaveAnimationClip(clip, jsonPath);
            SaveClipToCSV(clip, csvPath);

            std::cout << "애니메이션 저장 완료: " << jsonPath << " 와 " << csvPath << std::endl;
            ClampFrameIndex(clip);
            return true;
        }

        if (isMouseOver(loadButton, mousePos))
        {
            std::string jsonPath = "animations/" + clip.name + ".json";
            std::ifstream ifs(jsonPath);
            if (ifs.is_open())
            {
                nlohmann::json j;
                ifs >> j;
                clip.frames.clear();
                clip.name = j["name"];
                clip.loop = j["loop"];
                for (const auto& f : j["frames"])
                {
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
            else
            {
                std::cerr << "로드 실패: " << jsonPath << std::endl;
            }
            ClampFrameIndex(clip);
            return true;
        }

        if (isMouseOver(prevFrameButton, mousePos))
        {
            if (!clip.frames.empty())
            {
                manualFrameIndex--;
                ClampFrameIndex(clip);
                return true;
            }
        }

        if (isMouseOver(nextFrameButton, mousePos))
        {
            if (!clip.frames.empty())
            {
                manualFrameIndex++;
                ClampFrameIndex(clip);
                return true;
            }
        }

        return false;
    }

    // --------- UTIL ---------
    bool isMouseOver(const sf::RectangleShape& rect, const sf::Vector2f& mousePos) {
        return rect.getGlobalBounds().contains(mousePos);
    }

    int GetManualFrameIndex() const { return manualFrameIndex; }

    static void SaveAnimationClip(const AnimationClip& clip, const std::string& filename)
    {
        nlohmann::json j;
        j["name"] = clip.name;
        j["loop"] = clip.loop;
        for (const auto& frame : clip.frames)
        {
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

    static void SaveClipToCSV(const AnimationClip& clip, const std::string& filename)
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
            const FrameData& frame = clip.frames[i]; //csv파일속에 들어가는 이름 정의
            file << "Link_down," << i << ","
                << frame.rect.left << ","
                << frame.rect.top << ","
                << frame.rect.width << ","
                << frame.rect.height << "\n";
        }
        file.close();
        std::cout << "CSV 저장 완료: " << filename << std::endl;
    }

    inline void ClampFrameIndex(const AnimationClip& clip)
    {
        if (clip.frames.empty()) { manualFrameIndex = 0; return; }
        if (manualFrameIndex < 0) manualFrameIndex = 0;
        else if (manualFrameIndex >= static_cast<int>(clip.frames.size()))
            manualFrameIndex = static_cast<int>(clip.frames.size()) - 1;
    }
};
