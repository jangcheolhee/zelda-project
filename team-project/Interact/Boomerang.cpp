#include "stdafx.h"
#include "Boomerang.h"
#include "GameObject.h"

void Boomerang::Init(Player* p)
{
    player = p;
    std::string texPath = "graphics/Boomerang.png";

    if (!TEXTURE_MGR.Exists(texPath))
        TEXTURE_MGR.Load(texPath);

    sprite.setTexture(TEXTURE_MGR.Get("graphics/Boomerang.png"));
    sprite.setTextureRect({ 0, 0, 16, 16 });
    SetOrigin(Origins::MC);  // 중심
}

void Boomerang::Launch(sf::Vector2f direction)
{
    if (Utils::SqrMagnitude(direction) == 0.f) return; // 무효한 발사 방지
    this->direction = Utils::Normalize(direction);
    velocity = this->direction * speed;

    startPos = player->GetPosition();
    position = startPos;
    sprite.setPosition(startPos);
    state = State::FlyingOut;
    SetActive(true);
}
void Boomerang::Update(float dt)
{
    if (state == State::Inactive) return;

    position += velocity * dt;
    sprite.setPosition(position);
    sprite.rotate(720.f * dt);
    float dist = Utils::Distance(startPos, position);

    if (state == State::FlyingOut && dist >= maxDistance)
    {
        state = State::Returning;
    }

    if (state == State::Returning)
    {
        sf::Vector2f toPlayer = player->GetPosition() - position;
        velocity = Utils::Normalize(toPlayer) * speed;

        if (Utils::Distance(position, player->GetPosition()) < 10.f)
        {
            state = State::Inactive;
            SetActive(false); // 🔥 부메랑 다시 꺼지게
        }
    }
}

void Boomerang::SetMaxDistance(float dist)
{
    maxDistance = dist;
}

void Boomerang::SetDirection(const sf::Vector2f& dir)
{
    direction = Utils::Normalize(dir);
}

void Boomerang::Init()
{
    std::string texPath = "graphics/Boomerang.png";

    // 텍스처가 로드되지 않았다면 로드
    if (!TEXTURE_MGR.Exists(texPath))
    {
        TEXTURE_MGR.Load(texPath);
    }

    SetTexture(TEXTURE_MGR.Get(texPath));

    // 부메랑 사이즈 조절 등 추가 설정
    sprite.setTextureRect({ 0, 0, 16, 16 });
    SetOrigin(Origins::MC); // 중심
}

void Boomerang::Release()
{
}

void Boomerang::Reset()
{
    sprite.setOrigin(8.f, 8.f); // 중심 정렬 (16x16 기준)
    sprite.setTextureRect({ 0, 0, 16, 16 });
}
void Boomerang::Draw(sf::RenderWindow& window)
{
    if (!active) return;
    window.draw(sprite);
  
}
void Boomerang::SetTexture(const sf::Texture& tex)
{
    sprite.setTexture(tex); 
}
void Boomerang::SetPosition(const sf::Vector2f& pos)
{
    position = pos;
    sprite.setPosition(pos); // ✅ 반드시 필요!
}