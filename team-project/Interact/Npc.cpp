#include "stdafx.h"
#include "Defines.h"
#include "Npc.h"
#include "SpriteGo.h"
#include "TextGo.h"
#include "Player.h"
#include <cmath>
#include "Scene.h"

int Npc::nextNpcId = 0;
std::unordered_map<int, int> Npc::npcDialogueIndices;

Npc::Npc(const std::string& name)
{
    conversation = nullptr;
    dialogText = nullptr;
    npcId = nextNpcId++;
    dialogueIndex = 0; 
    InitDialogues();
}

Npc::~Npc()
{
    if (conversation != nullptr)
    {
        delete conversation;
        conversation = nullptr;
    }
    if (dialogText != nullptr)
    {
        delete dialogText;
        dialogText = nullptr;
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
    body.setTexture(TEXTURE_MGR.Get("data/HiddenPathToGarden.png"));
    body.setTextureRect({ 218, 102, 28, 29 }); 
    SetOrigin(Origins::MC); 
    SetScale({ 1, 1 });
}

void Npc::HandleBasicNpcInteraction()
{
    sf::FloatRect rect = player->GetGlobalBounds();
    rect.left -= 2.f;
    rect.top -= 2.f;
    rect.width += 4.f;
    rect.height += 4.f;

    sf::FloatRect npcRect = GetGlobalBounds();

    if (rect.intersects(npcRect) && npcSay == 0 && sayCount < 2)
    {
        player->isNpcTalk = 1;

        if (player->isNpcTalk == 1 && conversation == nullptr)
        {
            conversation = new SpriteGo("graphics/conversation.png", "Conversation");
            conversation->Init();
            conversation->Reset();
            conversation->SetActive(1);
            conversation->SetOrigin(Origins::MC);
            conversation->SetScale({ 2.5f, 2.5f });
            auto size = FRAMEWORK.GetWindowSizeF();
            conversation->SetPosition({ size.x * 0.5f, size.y * 0.5f });

            dialogText = new TextGo("fonts/DungGeunMo.ttf", "DialogText");
            dialogText->Init();
            dialogText->Reset();
            dialogText->SetActive(1);
            dialogText->SetOrigin(Origins::MC);
            dialogText->SetCharacterSize(18);
            dialogText->SetFillColor(sf::Color::White);
            dialogText->SetPosition({ size.x * 0.5f, size.y * 0.5f });
            dialogText->SetOutlineThickness(1.5f);
            dialogText->SetOutlineColor(sf::Color(0, 0, 150));
        }

        if (sayCount == 0)
        {
            sayCount++;
            if (dialogText != nullptr && !dialogues.empty())
            {
                int currentDialogueIndex = npcDialogueIndices[npcId];
                dialogText->SetString(dialogues[currentDialogueIndex]);
            }
        }
        if (InputMgr::GetKeyDown(sf::Keyboard::N) && sayCount == 1)
        {
            sayCount++;
            if (dialogText != nullptr)
            {
                dialogText->SetString(L"잘 가");
            }
        }
        if (sayCount == 2)
        {
            sayCount = 0;
            npcSay = !npcSay;
            player->isNpcTalk = 0;
            npcDialogueIndices[npcId] = (npcDialogueIndices[npcId] + 1) % 6;
            if (player->isNpcTalk == 0)
            {
                delete conversation;
                conversation = nullptr;
                delete dialogText;
                dialogText = nullptr;
            }
        }
    }
}

void Npc::HandleDadInteraction()
{
    if (!player) return;

    sf::Vector2f playerPos = player->GetPosition();
    sf::Vector2f dadPos = GetPosition();

    float distance = sqrt(pow(playerPos.x - dadPos.x, 2) + pow(playerPos.y - dadPos.y, 2));

    if (distance < 30.0f)
    {
        player->isNpcTalk = 1;

        if (conversation == nullptr)
        {
            conversation = new SpriteGo("graphics/conversation.png", "Conversation");
            conversation->Init();
            conversation->Reset();
            conversation->SetActive(1);
            conversation->SetOrigin(Origins::MC);
            conversation->SetScale({ 2.5f, 2.5f });
            auto size = FRAMEWORK.GetWindowSizeF();
            conversation->SetPosition({ size.x * 0.5f, size.y * 0.5f });

            dialogText = new TextGo("fonts/DungGeunMo.ttf", "DialogText");
            dialogText->Init();
            dialogText->Reset();
            dialogText->SetActive(1);
            dialogText->SetOrigin(Origins::MC);
            dialogText->SetCharacterSize(18);
            dialogText->SetFillColor(sf::Color::White);
            dialogText->SetPosition({ size.x * 0.5f, size.y * 0.5f });
            dialogText->SetOutlineThickness(1.5f);
            dialogText->SetOutlineColor(sf::Color(0, 0, 150));
        }

        if (sayCount == 0)
        {
            sayCount++;
            if (dialogText != nullptr)
            {
                dialogText->SetString(L"Hi, 검을 내게 맡기마...링크");
            }
        }

        if (InputMgr::GetKeyDown(sf::Keyboard::N) && sayCount == 1)
        {
            sayCount++;
            if (dialogText != nullptr)
            {
                dialogText->SetString(L"링크...젤다공주를 부탁한다");
            }
        }

        if (InputMgr::GetKeyDown(sf::Keyboard::N) && sayCount == 2)
        {
            sayCount = 0;
            player->isNpcTalk = 0;

            // UI 정리
            if (conversation != nullptr)
            {
                delete conversation;
                conversation = nullptr;
            }
            if (dialogText != nullptr)
            {
                delete dialogText;
                dialogText = nullptr;
            }
        }
    }
}
void Npc::InitDialogues()
{
    dialogues = {
        L"물건에 가까이 가서 X 버튼을\n누르면 물건을 들어 올릴 수\n있어...",
        L"밥은 먹고 다니니?",
        L"풀 들어봤어?",
        L"어린아이는 이 시간에 돌아다니면\n안돼",
        L"위로 가면 성이 있단다.",
        L"모험을 떠날 때는 항상\n준비물을 챙기는 걸 잊지마"
    };
}

void Npc::OnInteract()
{
    switch (npcType)
    {
    case Type::Basic:
        HandleBasicNpcInteraction();
        break;
    case Type::Dad:
        HandleDadInteraction();
        break;
    default:
        HandleBasicNpcInteraction();
        break;
    }
}

void Npc::Init()
{
    Interactable::Init();

    auto size = FRAMEWORK.GetWindowSizeF();
    talkUi.setSize(size);
    talkUi.setCenter({ size.x * 0.5f, size.y * 0.5f });
}

void Npc::Reset()
{
    Interactable::Reset();
    npcId = static_cast<int>(GetPosition().x) * 1000 + static_cast<int>(GetPosition().y);
    currentDirection = Direction::Down;

    if (dialogues.empty()) InitDialogues();
    if (npcType == Type::Dad) DaddySprite();
    if (npcType != Type::Dad) DirectionSprite(currentDirection);
    if (conversation != nullptr)
    {
        delete conversation;
        conversation = nullptr;
    }

    auto size = FRAMEWORK.GetWindowSizeF();
    talkUi.setSize(size);
    talkUi.setCenter({ size.x * 0.5f, size.y * 0.5f });

    sayCount = 0;
    npcSay = 0;
}

void Npc::Update(float dt)
{
    Interactable::Update(dt);

    if (player != nullptr && npcType != Type::Dad)
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
    if (player->isNpcTalk == 1 && conversation != nullptr)
    {
        sf::View originalView = window.getView();
        window.setView(talkUi);
        conversation->Draw(window);
        if (dialogText != nullptr) dialogText->Draw(window);
        window.setView(originalView);
    }
}