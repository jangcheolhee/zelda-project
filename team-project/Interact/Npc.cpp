#include "stdafx.h"
#include "Defines.h"
#include "Npc.h"
#include "Player.h"
#include <cmath>

Npc::Npc(const std::string& name)
{
}

void Npc::SetPlayer(Player* p)
{
    player = p;
}

void Npc::DirectionSprite(Direction dir)
{
    body.setTexture(TEXTURE_MGR.Get("graphics/Enemy_sheet.png"));

    switch (dir)
    {
    case Direction::Down: // Front
        body.setTextureRect({ 664, 1394, 25, 30 });
        SetOrigin(Origins::MC);
        break;

    case Direction::Up: // Back
        body.setTextureRect({ 695, 1392, 26, 26 });
        SetOrigin(Origins::MC);
        break;

    case Direction::Left:
        body.setTextureRect({ 724, 1395, 24, 29 });
        SetOrigin(Origins::MC);
        break;

    case Direction::Right:
        body.setTextureRect({ 724, 1395, 24, 29 });
        SetOrigin(Origins::MC);
        SetScale({-1, 1});
        break;

    case Direction::None:
    default:
        body.setTextureRect({ 664, 1394, 25, 30 });
        SetOrigin(Origins::MC);
        break;
    }
}

Direction Npc::GetDirectionToPlayer()
{
    if (player == nullptr) return Direction::Down;

    sf::Vector2f npcPos = GetPosition();
    sf::Vector2f playerPos = player->GetPosition();

    sf::Vector2f direction = playerPos - npcPos;

    if (std::abs(direction.x) > std::abs(direction.y))
    {
        return (direction.x > 0) ? Direction::Right : Direction::Left;
    }
    else
    {
        return (direction.y > 0) ? Direction::Down : Direction::Up;
    }
}

void Npc::OnInteract()
{
}

void Npc::Init()
{
	Interactable::Init();
}

void Npc::Reset()
{
	currentDirection = Direction::Down;
	DirectionSprite(currentDirection);
	Interactable::Reset();
}

void Npc::Update(float dt)
{
    Interactable::Update(dt);

    if (player != nullptr)
    {
        Direction newDirection = GetDirectionToPlayer();
        if (newDirection != currentDirection)
        {
            currentDirection = newDirection;
            DirectionSprite(currentDirection);
        }
    }
}