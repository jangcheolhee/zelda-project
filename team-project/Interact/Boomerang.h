#pragma once
#include "Player.h"

class Boomerang : public GameObject
{
public:
    enum class State { Inactive, FlyingOut, Returning };

private:
    sf::Vector2f direction;
public:
    Boomerang(const std::string& name) : GameObject(name) {}
    float speed = 300.f;
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float maxDistance = 150.f*5.f;//부메랑 사정거리
    sf::Vector2f startPos;
    State state = State::Inactive;
    Player* player;
    void Init(Player* player);
    void Launch(sf::Vector2f direction);
    void Update(float dt);
    void Draw(sf::RenderWindow& window) override;
    bool IsActive() const { return state != State::Inactive; }
    void SetMaxDistance(float dist);
    void SetDirection(const sf::Vector2f& dir);
    void SetTexture(const sf::Texture& tex);
    void SetPosition(const sf::Vector2f& pos);
    // GameObject을(를) 통해 상속됨
    void Init() override;
    void Release() override;
    void Reset() override;
};

