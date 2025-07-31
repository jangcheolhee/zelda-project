#include "stdafx.h"
#include "InventoryUI.h"
#include "ResourceMgr.h"
#include "Player.h"

InventoryUI::InventoryUI()
{
    sortingLayer = SortingLayers::UI;
}   

void InventoryUI::Init()
{
    texturePath = "graphics/inventory.png";
   
    if (!inventoryTexture.loadFromFile(texturePath))
    {
        std::cerr << "Failed to load inventory texture\n";
    }
    else
    {
        inventorySprite.setTexture(inventoryTexture); // ← 이건 load 성공 후에 넣어야 함
        inventorySprite.setPosition(100.f, 400.f);
        inventorySprite.setScale(2.f, 2.f);
    }

    sf::Vector2f center = FRAMEWORK.GetWindow().getView().getCenter();

  
    shownPosition =
    {
        center.x - inventorySprite.getGlobalBounds().width * 0.5f,
        center.y - inventorySprite.getGlobalBounds().height * 0.5f
    };

    hiddenPosition = shownPosition;
    hiddenPosition.y -= 500.f; // 위로 숨기기

    currentPosition = hiddenPosition;
    inventorySprite.setPosition(currentPosition);

  
    isShowing = false;
    isAnimating = false;
    GameObject::SetActive(true); // 시작 시 감춤
}


void InventoryUI::Update(float dt)
{
  /*  if (!IsActive())
        return;*/
    //if (!GetActive()) return;
    if (!isAnimating && !isShowing)
        return;
    // 현재 목표 위치 설정
    sf::Vector2f targetPos = isShowing ? shownPosition : hiddenPosition;

    currentPosition += (targetPos - currentPosition) * animationSpeed * dt;

    // 실제 스프라이트 위치 설정
    inventorySprite.setPosition(currentPosition);

    // 애니메이션 끝났는지 확인
    if (std::abs(currentPosition.y - targetPos.y) < 1.f)
    {
        currentPosition = targetPos;
        isAnimating = false;

        // 숨겨야 할 경우 완전히 숨김 처리
        if (!isShowing)
            GameObject::SetActive(false);
    }
}

void InventoryUI::Draw(sf::RenderWindow& window)
{
    if (!isShowing && !isAnimating) return;

    window.draw(inventorySprite);

    //for (auto& slot : itemSlots)
      //  window.draw(slot);
}

void InventoryUI::Release()
{
}

void InventoryUI::Reset()
{
}
void InventoryUI::SetActive(bool active)
{
    GameObject::SetActive(true);
    isAnimating = true;
    isShowing = active;
    Update(0);
}

bool InventoryUI::IsVisible() const
{
    return isShowing;
}
