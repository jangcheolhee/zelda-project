#include "stdafx.h"
#include "HitboxGenerator.h"

void HitboxGenerator::SpawnSquareHitBox(
    TileMap* tileMapGame,
    std::vector<HitBox>& collisions,
    sf::RectangleShape& collisionBox)
{
    HitboxCorners corners;
    if (!tileMapGame->LoadHitboxLayer(corners)) return;

    // 검색 범위와 허용 오차 확대
    const float searchWidth = 500.f;  // 더 큰 히트박스 허용
    const float searchHeight = 500.f;
    const float epsilon = 2.f;        // 코너 매칭 허용 오차 증가
    const float minSize = 4.f;        // 최소 히트박스 크기 (너무 작은 히트박스 제외)

    auto nearlyEqual = [epsilon](float a, float b) {
        return std::abs(a - b) < epsilon;
        };

    // 생성된 hitbox들의 영역을 추적하여 겹침 방지
    std::vector<sf::FloatRect> createdRects;

    // 겹침 검사를 더 엄격하게 수정 - 완전히 포함되거나 상당 부분이 겹치는 경우만 겹침으로 판단
    auto isOverlapping = [](const sf::FloatRect& rect1, const sf::FloatRect& rect2) {
        if (!rect1.intersects(rect2)) return false;

        // 교집합 영역 계산
        sf::FloatRect intersection;
        if (rect1.intersects(rect2, intersection)) {
            float intersectionArea = intersection.width * intersection.height;
            float rect1Area = rect1.width * rect1.height;
            float rect2Area = rect2.width * rect2.height;
            float minArea = std::min(rect1Area, rect2Area);

            // 작은 사각형의 50% 이상이 겹치거나, 교집합이 8픽셀 이상인 경우만 겹침으로 판단
            return (intersectionArea / minArea > 0.5f) || (intersectionArea > 64.0f);
        }
        return false;
        };

    // 이미 처리된 좌상단 점들을 추적
    std::set<std::pair<int, int>> processedTopLefts;

    // 모든 가능한 hitbox 후보들을 수집
    struct HitboxCandidate {
        sf::Vector2f topLeft;
        sf::FloatRect rect;
        int priority; // 낮을수록 우선순위 높음
        std::string method;
    };

    std::vector<HitboxCandidate> allCandidates;

    // 모든 좌상단 점에 대해 가능한 hitbox들을 수집
    for (const auto& topLeft : corners.topLefts)
    {
        // 방법 1: 완전한 4코너 사각형 - 가장 가까운 점들부터 매칭
        // 가장 가까운 우상단 점 찾기
        sf::Vector2f closestTopRight;
        float minTopRightDistance = searchWidth;
        bool foundTopRight = false;

        for (const auto& topRight : corners.topRights)
        {
            if (nearlyEqual(topRight.y, topLeft.y) && topRight.x > topLeft.x)
            {
                float distance = topRight.x - topLeft.x;
                if (distance <= searchWidth && distance < minTopRightDistance)
                {
                    minTopRightDistance = distance;
                    closestTopRight = topRight;
                    foundTopRight = true;
                }
            }
        }

        if (foundTopRight)
        {
            // 가장 가까운 좌하단 점 찾기
            sf::Vector2f closestBottomLeft;
            float minBottomLeftDistance = searchHeight;
            bool foundBottomLeft = false;

            for (const auto& bottomLeft : corners.bottomLefts)
            {
                if (nearlyEqual(bottomLeft.x, topLeft.x) && bottomLeft.y > topLeft.y)
                {
                    float distance = bottomLeft.y - topLeft.y;
                    if (distance <= searchHeight && distance < minBottomLeftDistance)
                    {
                        minBottomLeftDistance = distance;
                        closestBottomLeft = bottomLeft;
                        foundBottomLeft = true;
                    }
                }
            }

            if (foundBottomLeft)
            {
                // 예상되는 우하단 위치
                sf::Vector2f expectedBottomRight(closestTopRight.x, closestBottomLeft.y);

                bool foundBottomRight = false;
                for (const auto& bottomRight : corners.bottomRights)
                {
                    if (nearlyEqual(bottomRight.x, expectedBottomRight.x) &&
                        nearlyEqual(bottomRight.y, expectedBottomRight.y))
                    {
                        sf::FloatRect rect(topLeft.x, topLeft.y,
                            closestTopRight.x - topLeft.x,
                            closestBottomLeft.y - topLeft.y);

                        // 최소 크기 체크
                        if (rect.width < minSize || rect.height < minSize) break;

                        // 우선순위 결정: 큰 히트박스와 완전한 4코너 사각형에 높은 우선순위 부여
                        float area = rect.width * rect.height;
                        float ratio = std::max(rect.width / rect.height, rect.height / rect.width);

                        int priority = 0;
                        if (area > 50000.0f) priority = 0; // 매우 큰 히트박스 (최우선)
                        else if (area > 20000.0f) priority = 1; // 큰 히트박스
                        else if (ratio > 8.0f) priority = 2; // 매우 얇고 긴 것
                        else if (ratio > 4.0f) priority = 3; // 얇고 긴 것
                        else priority = 4; // 일반적인 것

                        allCandidates.push_back({ topLeft, rect, priority, "4-corner" });
                        foundBottomRight = true;
                        break;
                    }
                }
            }
        }

        // 방법 2: 좌상단 + 우하단 대각선 매칭 - 가장 가까운 우하단 찾기
        sf::Vector2f closestBottomRight;
        float minBottomRightDistance = searchWidth + searchHeight;
        bool foundBottomRight = false;

        for (const auto& bottomRight : corners.bottomRights)
        {
            if (bottomRight.x > topLeft.x && bottomRight.y > topLeft.y &&
                bottomRight.x - topLeft.x <= searchWidth &&
                bottomRight.y - topLeft.y <= searchHeight)
            {
                float distance = std::sqrt(std::pow(bottomRight.x - topLeft.x, 2) +
                    std::pow(bottomRight.y - topLeft.y, 2));
                if (distance < minBottomRightDistance)
                {
                    minBottomRightDistance = distance;
                    closestBottomRight = bottomRight;
                    foundBottomRight = true;
                }
            }
        }

        if (foundBottomRight)
        {
            sf::FloatRect rect(topLeft.x, topLeft.y,
                closestBottomRight.x - topLeft.x,
                closestBottomRight.y - topLeft.y);

            if (rect.width >= minSize && rect.height >= minSize)
            {
                float area = rect.width * rect.height;
                float ratio = std::max(rect.width / rect.height, rect.height / rect.width);

                int priority = 10;
                if (area > 50000.0f) priority = 5; // 매우 큰 히트박스
                else if (area > 20000.0f) priority = 6; // 큰 히트박스
                else if (ratio > 8.0f) priority = 7;
                else if (ratio > 4.0f) priority = 8;

                allCandidates.push_back({ topLeft, rect, priority, "diagonal" });
            }
        }

        // 방법 3: 좌상단 + 우상단 (세로로 가장 가까운 하단 찾기)
        if (foundTopRight)
        {
            float minBottomY = topLeft.y + searchHeight;
            bool foundBottom = false;

            for (const auto& bottomLeft : corners.bottomLefts)
            {
                if (nearlyEqual(bottomLeft.x, topLeft.x) && bottomLeft.y > topLeft.y &&
                    bottomLeft.y < minBottomY)
                {
                    minBottomY = bottomLeft.y;
                    foundBottom = true;
                }
            }

            for (const auto& bottomRight : corners.bottomRights)
            {
                if (nearlyEqual(bottomRight.x, closestTopRight.x) && bottomRight.y > topLeft.y &&
                    bottomRight.y < minBottomY)
                {
                    minBottomY = bottomRight.y;
                    foundBottom = true;
                }
            }

            if (foundBottom)
            {
                sf::FloatRect rect(topLeft.x, topLeft.y,
                    closestTopRight.x - topLeft.x,
                    minBottomY - topLeft.y);

                if (rect.width >= minSize && rect.height >= minSize)
                {
                    float area = rect.width * rect.height;
                    float ratio = std::max(rect.width / rect.height, rect.height / rect.width);

                    int priority = 15;
                    if (area > 50000.0f) priority = 9; // 매우 큰 히트박스
                    else if (area > 20000.0f) priority = 10; // 큰 히트박스
                    else if (ratio > 8.0f) priority = 11;
                    else if (ratio > 4.0f) priority = 12;

                    allCandidates.push_back({ topLeft, rect, priority, "horizontal" });
                }
            }
        }

        // 방법 4: 좌상단 + 좌하단 (가로로 가장 가까운 우측 찾기)
        // 가장 가까운 좌하단 점 찾기 (이미 위에서 구했을 수도 있음)
        sf::Vector2f finalBottomLeft;
        float finalMinBottomLeftDistance = searchHeight;
        bool finalFoundBottomLeft = false;

        for (const auto& bottomLeft : corners.bottomLefts)
        {
            if (nearlyEqual(bottomLeft.x, topLeft.x) && bottomLeft.y > topLeft.y)
            {
                float distance = bottomLeft.y - topLeft.y;
                if (distance <= searchHeight && distance < finalMinBottomLeftDistance)
                {
                    finalMinBottomLeftDistance = distance;
                    finalBottomLeft = bottomLeft;
                    finalFoundBottomLeft = true;
                }
            }
        }

        if (finalFoundBottomLeft)
        {
            float minRightX = topLeft.x + searchWidth;
            bool foundRight = false;

            for (const auto& topRight : corners.topRights)
            {
                if (nearlyEqual(topRight.y, topLeft.y) && topRight.x > topLeft.x &&
                    topRight.x < minRightX)
                {
                    minRightX = topRight.x;
                    foundRight = true;
                }
            }

            for (const auto& bottomRight : corners.bottomRights)
            {
                if (nearlyEqual(bottomRight.y, finalBottomLeft.y) && bottomRight.x > topLeft.x &&
                    bottomRight.x < minRightX)
                {
                    minRightX = bottomRight.x;
                    foundRight = true;
                }
            }

            if (foundRight)
            {
                sf::FloatRect rect(topLeft.x, topLeft.y,
                    minRightX - topLeft.x,
                    finalBottomLeft.y - topLeft.y);

                if (rect.width >= minSize && rect.height >= minSize)
                {
                    float area = rect.width * rect.height;
                    float ratio = std::max(rect.width / rect.height, rect.height / rect.width);

                    int priority = 16;
                    if (area > 50000.0f) priority = 13; // 매우 큰 히트박스
                    else if (area > 20000.0f) priority = 14; // 큰 히트박스
                    else if (ratio > 8.0f) priority = 15;
                    else if (ratio > 4.0f) priority = 16;

                    allCandidates.push_back({ topLeft, rect, priority, "vertical" });
                }
            }
        }
    }

    // 우선순위별로 정렬 - 수동으로 구현
    for (size_t i = 0; i < allCandidates.size(); ++i)
    {
        for (size_t j = i + 1; j < allCandidates.size(); ++j)
        {
            bool shouldSwap = false;
            if (allCandidates[i].priority > allCandidates[j].priority)
            {
                shouldSwap = true;
            }
            else if (allCandidates[i].priority == allCandidates[j].priority)
            {
                float areaI = allCandidates[i].rect.width * allCandidates[i].rect.height;
                float areaJ = allCandidates[j].rect.width * allCandidates[j].rect.height;
                if (areaI < areaJ)
                {
                    shouldSwap = true;
                }
            }

            if (shouldSwap)
            {
                HitboxCandidate temp = allCandidates[i];
                allCandidates[i] = allCandidates[j];
                allCandidates[j] = temp;
            }
        }
    }
    // 우선순위 순서대로 겹치지 않는 hitbox들을 생성
    for (const auto& candidate : allCandidates)
    {
        std::pair<int, int> tlKey = std::make_pair(static_cast<int>(std::round(candidate.topLeft.x)),
            static_cast<int>(std::round(candidate.topLeft.y)));

        // 겹침 체크
        bool overlaps = false;
        for (const auto& existingRect : createdRects)
        {
            if (isOverlapping(candidate.rect, existingRect))
            {
                overlaps = true;
                break;
            }
        }

        // 이미 이 좌상단 점을 처리했는지 확인 (겹침이 없는 경우에만)
        if (!overlaps && processedTopLefts.find(tlKey) != processedTopLefts.end()) overlaps = true;

        if (!overlaps && candidate.rect.width > 0 && candidate.rect.height > 0)
        {
            HitBox hitbox;
            hitbox.UpdateTransformCollision(collisionBox, candidate.rect,
                sf::Vector2f(candidate.rect.left + 4, candidate.rect.top + 4));
            collisions.push_back(hitbox);
            createdRects.push_back(candidate.rect);
            processedTopLefts.insert(tlKey);
        }
    }
}