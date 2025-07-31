#include "stdafx.h"
#include "Defines.h"
#include "Npc.h"
#include "SpriteGo.h"
#include "Player.h"
#include <cmath>

Npc::Npc(const std::string& name)
{
    conversation = nullptr;
}

Npc::~Npc()
{
    if (conversation != nullptr)
    {
        delete conversation;
        conversation = nullptr;
    }
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
        SetScale({ 1, 1 });
        break;

    case Direction::Up: // Back
        body.setTextureRect({ 695, 1392, 26, 26 });
        SetOrigin(Origins::MC);
        SetScale({ 1, 1 });
        break;

    case Direction::Left:
        body.setTextureRect({ 724, 1395, 24, 29 });
        SetOrigin(Origins::MC);
        SetScale({ 1, 1 });
        break;

    case Direction::Right:
        body.setTextureRect({ 724, 1395, 24, 29 });
        SetOrigin(Origins::MC);
        SetScale({ -1, 1 });
        break;

    case Direction::None:
    default:
        body.setTextureRect({ 664, 1394, 25, 30 });
        SetOrigin(Origins::MC);
        SetScale({ 1, 1 });
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

void Npc::DaddySprite()
{
    body.setTexture(TEXTURE_MGR.Get("graphics/HiddenPathToGarden.png"));
    body.setTextureRect({ 217, 102, 30, 30 });
    SetOrigin(Origins::MC);
    SetScale({ 1, 1 });
}

void Npc::OnInteract()
{
    sf::FloatRect rect = player->GetGlobalBounds();
    rect.left -= 2.f;
    rect.top -= 2.f;
    rect.width += 4.f;
    rect.height += 4.f;

    //switch (type)
    //{
    //case Npc::Type::None:
    //    break;
    //case Npc::Type::Guard:
    //{
        if (rect.intersects(GetGlobalBounds()) && npcSay == 0 && sayCount < 2)
        {
            player->isNpcTalk = 1;

            if (conversation == nullptr)
            {
                conversation = new SpriteGo("graphics/conversation.png", "Conversation");
                conversation->Init();
                conversation->Reset();
                conversation->SetActive(1);  
                conversation->SetOrigin(Origins::MC);
                conversation->SetPosition({ 100.f, 100.f });
            }

            if (sayCount == 0)
            {
                sayCount++;

                //textGo ¶ç¿ì±â
                std::cout << "Hi, Don't Do That!" << sayCount << std::endl;
            }
            if (InputMgr::GetKeyDown(sf::Keyboard::N) && sayCount == 1)
            {
                sayCount++;
                //textGo ¶ç¿ì±â
                std::cout << "You Can Do It! Bye." << sayCount << std::endl;
            }
            if (sayCount == 2)
            {
                if (conversation != nullptr)
                {
                    delete conversation;
                    conversation = nullptr;
                }

                //conversation->SetActive(0);
                sayCount = 0;
                npcSay = !npcSay;
                player->isNpcTalk = 0;
            }
        }
    //}
    //    break;
    //case Npc::Type::Daddy:
    //    break;
    //default:
    //    break;
    //}


   
}

void Npc::Init()
{
    Interactable::Init();
}

void Npc::Reset()
{
    Interactable::Reset();
    currentDirection = Direction::Down;
    DirectionSprite(currentDirection);

    if (conversation != nullptr)
    {
        delete conversation;
        conversation = nullptr;
    }
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
    OnInteract();

    if (conversation != nullptr)
    {
        conversation->Update(dt);
    }
}

void Npc::Draw(sf::RenderWindow& window)
{
    Interactable::Draw(window);
    if (conversation != nullptr)
    {
        conversation->Draw(window);
    }
}