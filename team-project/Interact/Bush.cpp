#include "stdafx.h"
#include "Bush.h"
#include "Player.h"
#include "SceneGame.h"

void Bush::ChangeAnimation()
{
	animator.Play("animations/bush2.csv");
}

void Bush::OnInteract()
{
	switch (state)
	{
	case BushState::OnGround:

		state = BushState::Held;
		player->SetIsInteract(true);
		break;

	case BushState::Held:

		state = BushState::Thrown;
		break;
	}
	//  ÷  ̾    ν        ¸   ذ 
}

void Bush::Init()
{
	Interactable::Init();
	animator.AddEvent("Crush", 7,
		[this]()
		{
			std::cout << "!!" << std::endl;
			state = BushState::OnGround;
			animator.Stop();
			SetActive(false);

		}
	);
	hitBox.rect.setSize({ 10,10 });
	hitBox.SetOrigin(Origins::MR);

}

void Bush::Reset()
{
	Interactable::Reset();
	body.setTexture(TEXTURE_MGR.Get("graphics/Overworld.png"));
	body.setTextureRect({ 304,57,16,16 });
	SetOrigin(Origins::ML);

	type = Type::Throw;

	lifeTime = 0.f;
	state = BushState::OnGround;
	isHit = false;

}

void Bush::Update(float dt)
{
	Interactable::Update(dt);
	switch (state)
	{
	case BushState::OnGround:

		break;

	case BushState::Held:
	{

		sf::Vector2f pos = player->GetGlobalBounds().getPosition();

		pos.y -= player->GetGlobalBounds().height + 1;
		SetPosition(pos);

		if (InputMgr::GetKeyDown(sf::Keyboard::X) || InputMgr::GetKeyDown(sf::Keyboard::Z))
		{
			Shoot();
		}
		break;
	}
	case BushState::Thrown:

		velocity += gravity * dt;
		lifeTime += dt;
		position += velocity * dt;
		SetPosition(position);

		for (auto& enemy : enemyList)
		{

			if (Utils::CheckCollision(enemy->GetBoundBox().rect, boundBox.rect))
			{
				isHit = true;
				enemy->OnDamage(1);
				break;
			}
		}
		if (lifeTime > 0.5 || isHit)
		{
			player->SetIsInteract(false);
			state = BushState::Crush;
			ChangeAnimation();
		}
		break;
	}
	hitBox.rect.setPosition(GetPosition() + sf::Vector2f{ 3, - 6 });
	boundBox.rect.setPosition(GetPosition() + sf::Vector2f{0,-8});


}
void Bush::Shoot()
{
	enemyList = sceneGame->GetEnemy();
	state = BushState::Thrown;
	lifeTime = 0.f;
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
	velocity = dir * speed;
	SOUND_MGR.PlaySfx(SOUNDBUFFER_MGR.Get("effects/throw.wav"));
}