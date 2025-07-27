#include "stdafx.h"
#include "Bush.h"
#include "Player.h"

void Bush::OnInteract()
{
	////  ÷  ̾    ν      ٰ       ϱ  -> boolŸ            ?
	//if (player->WantsToInteract() && !player->IsInteract())
	//{
	//	player->SetIsInteract(true);
	//	life = true;
	//}
	switch (state)
	{
	case BushState::OnGround:
		// 집어들기
		state = BushState::Held;
		player->SetIsInteract(true);   // 필요 시 플레이어 상태 잠금
		// 충돌 비활성화(땅/플레이어와) 등이 필요하다면 여기서 처리
		break;

	case BushState::Held:
		// 두 번째 상호작용 -> 던지기
		state = BushState::Thrown;
		break;
	}
	//  ÷  ̾    ν        ¸   ذ 
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
	//SetOrigin(Origins::ML);
	type = Type::Throw;
	life = false;
	life = 0.f;
	state = BushState::OnGround;

}

void Bush::UpdateBeHavior(float dt)
{
	switch (state)
	{
	case BushState::OnGround:
		// 바닥에 있는 동안은 Scene 쪽에서 플레이어와 겹치고 X/Z 눌렀을 때 OnInteract가 불립니다.
		break;

	case BushState::Held:
	{
		// 플레이어 머리 위에 붙어서 따라다님
		sf::Vector2f pos = player->GetGlobalBounds().getPosition();
		//pos.x += player->GetLocalBounds().width * 0.5f;
		pos.y -= player->GetLocalBounds().height - GetLocalBounds().height + 3;
		SetPosition(pos);

		// X/Z를 다시 눌렀을 때 Scene/CollisionManager 쪽에서 OnInteract를 다시 호출해주거나,
		// 여기서 직접 입력을 체크해도 됩니다.
		if (InputMgr::GetKeyDown(sf::Keyboard::X) || InputMgr::GetKeyDown(sf::Keyboard::Z))
		{
			Shoot();
		}
		break;
	}
	case BushState::Thrown:

		life += dt;
		position += dir * dt *speed;
		SetPosition(position);

		// 벽/적과 충돌 시 바로 파괴하거나, lifeTime이 끝나면 파괴
		if (life > 1)
		{
			if (player) player->SetIsInteract(false);
			SetActive(false);        // 파괴 or 풀로 반환
			state = BushState::OnGround; // 재사용할 거면 상태 되돌리기
		}
		break;
	}
}
void Bush::Shoot()
{
	state = BushState::Thrown;
	life = 0.f;
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