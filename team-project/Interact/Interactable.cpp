#include "stdafx.h"
#include "Interactable.h"
#include "Player.h"

Interactable::Interactable(const std::string& name)
	: GameObject(name)
{
}

void Interactable::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	body.setPosition(pos);
}

void Interactable::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	body.setRotation(rot);
}

void Interactable::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	body.setScale(s);
}

void Interactable::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	body.setOrigin(o);
}

void Interactable::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(body, preset);
	}
}

void Interactable::Init()
{
	
}

void Interactable::Release()
{
}

void Interactable::Reset()
{
	if (SCENE_MGR.GetCurrentSceneId() == SceneIds::Game)
	{
		sceneGame = (SceneGame*)SCENE_MGR.GetCurrentScene(); //�ٿ� ĳ���� 
	}
	else
	{
		sceneGame = nullptr;
	}


	player = (Player*)SCENE_MGR.GetCurrentScene()->FindGameObject("Player");
}

void Interactable::Update(float dt)
{
	
	if (!GetActive())
		return;

	// 플레이어에서 더 크게 비교하는 바운드 리턴 함수 있으면 좋겠음
	sf::FloatRect playerBounds = player->GetGlobalBounds();
	playerBounds.left -= 2.f;
	playerBounds.top -= 2.f;
	playerBounds.width += 4.f;
	playerBounds.height += 4.f;


	if (playerBounds.intersects(GetGlobalBounds()))
	{
		if (InputMgr::GetKeyDown(sf::Keyboard::X))
		{

			OnInteract(); // 상태 변경, 파괴, 대화 등
		}
	}
	hitBox.UpdateTransform(body, GetLocalBounds());
}

void Interactable::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitBox.Draw(window);
}