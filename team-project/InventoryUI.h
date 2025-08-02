#pragma once
#include "GameObject.h"

class Player;

class InventoryUI :
    public GameObject
{
private:
    sf::RectangleShape background;
    Player* player = nullptr;
    bool isVisible = false;
    //sf::Texture* texture = nullptr;
    //sf::Sprite sprite;
    sf::Texture inventoryTexture;
    sf::Sprite inventorySprite;
    std::string texturePath;
    bool isShowing = false; // 현재 보여지는 상태
    bool isAnimating = false; // 애니메이션 중인지
    sf::Vector2f shownPosition;    // 화면 중앙 (보이는 위치)
    sf::Vector2f hiddenPosition;   // 화면 밖 위쪽
    sf::Vector2f currentPosition;  // 현재 위치 (보간용)
    
    float slideSpeed = 300.f;
    float animationSpeed = 5.f;
    std::vector<sf::RectangleShape> itemSlots;
public:

    InventoryUI(const std::string& name="");

    void Init() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;

    // GameObject을(를) 통해 상속됨
    void Release() override;
    void Reset() override;
    void SetPlayer(Player* p) { player = p; }
    void SetTexturePath(const std::string& path) { texturePath = path; }
    void Toggle() { SetActive(!IsActive()); }
    void SetActive(bool active);
    bool IsVisible() const;
    void SetSize(const sf::Vector2f& size);
    void SetPosition(const sf::Vector2f& pos);

    void SetScale(float sx, float sy);
    void SetInventoryPosition(const sf::Vector2f& pos);

};

