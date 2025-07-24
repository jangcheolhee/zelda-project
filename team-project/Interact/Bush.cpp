#include "stdafx.h"
#include "Bush.h"
#include "Player.h"

void Bush::OnInteract()
{
	// �÷��̾�� �ν� ����ٰ� �����ϱ� -> boolŸ������ ���� ?
	player->SetIsInteract(true);
	isShoot = true;
	// �÷��̾�� �ν� �� ���¸� �ذ�
}

void Bush::Init()
{
	Interactable::Init();
	
}

void Bush::Reset()
{
	Interactable::Reset();
	body.setTexture(TEXTURE_MGR.Get("graphics/Overworld.png"));
	body.setTextureRect({ 304,57,16,16 });
	SetOrigin(Origins::ML);
	type = Type::Throw;
	isShoot = false;
	shootTimer = 0.f;

}

void Bush::UpdateBeHavior(float dt)
{
	if (player->IsInteract())
	{
		if (isShoot)
		{
			sf::Vector2f pos = player->GetGlobalBounds().getPosition();
			pos.x += player->GetLocalBounds().width * 0.5f;
			pos.y -= player->GetLocalBounds().height;
			SetPosition(pos);
			
			if (InputMgr::GetKeyDown(sf::Keyboard::X) || InputMgr::GetKeyDown(sf::Keyboard::Z))
			{
				Shoot();
			}
		}
		else
		{
			shootTimer += dt;
			position += speed * dt * dir;
			SetPosition(position);
			if (shootTimer > 1)
			{
				//player->SetIsInteract(false);
				SetActive(false);

			}
		}
	}
}

void Bush::Shoot()
{
	isShoot = false;
	shootTimer = 0.f;
	switch (player->GetDirection())
	{
	case Direction::Down:
		dir = { 0.f, 1.f };
		break;
	case Direction::Up:
		dir = { 0.f, -1.f };
		break;
	case Direction::Left:
		dir = { -1.f,0.f };
		break;
	case Direction::Right:
		dir = { 1.f, 0.f };
		break;
	}

}