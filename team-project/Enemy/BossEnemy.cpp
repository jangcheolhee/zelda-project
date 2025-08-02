#include "stdafx.h"
#include "BossEnemy.h"
#include "Player.h"
BossEnemy::BossEnemy(const std::string& name)
	: Enemy(name)
{
}

void BossEnemy::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	body.setPosition(pos);
	shadow.setPosition(pos + sf::Vector2f{ 0,24 });
}

void BossEnemy::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	body.setRotation(rot);
	shadow.setRotation(rot);
}

void BossEnemy::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	body.setScale(s);
	shadow.setScale(s);

}

void BossEnemy::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	body.setOrigin(o);
	shadow.setOrigin(o);
}

void BossEnemy::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(body, preset);
		Utils::SetOrigin(shadow, preset);
	}
}

void BossEnemy::Init()
{
	animator.SetTarget(&body);
	animator.AddEvent("BossDie", 5,
		[this]()
		{
			SetActive(false);
		}
	);
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;


	Utils::SetOrigin(body, Origins::MC);
	Utils::SetOrigin(shadow, Origins::MC);
}

void BossEnemy::Release()
{
}



void BossEnemy::Reset()
{

	Enemy::Reset();
	body.setTexture(TEXTURE_MGR.Get("graphics/Boss.png"));
	shadow.setTexture(TEXTURE_MGR.Get("graphics/Boss.png"));
	body.setTextureRect({ 207, 39, 34, 34 });
	shadow.setTextureRect({ 198,206,24,16 });
	state = BossState::Stop;
	SetPosition(startPos);
}

void BossEnemy::Update(float dt)
{
	pastPosition = GetPosition();
	animator.Update(dt);
	hitTimer += dt;
	switch (state)
	{
	case BossState::Stop:
		hitTimer = 0;
		pastPos = body.getPosition();
		timer += dt;
		if (Utils::RandomRange(0, 2) == 0)
		{
			SetPosition({ GetPosition().x - 0.05f ,GetPosition().y });

		}
		else
		{
			SetPosition({ GetPosition().x + 0.05f ,GetPosition().y });
		}
		if (timer > 3)
		{
			state = BossState::Idle;
			SetScale({ 1,1 });

			timer = 0;
		}
		break;
	case BossState::Idle:
		hitTimer = 0;
		if (timer == 0)
		{
			velocity.y = -151;
			if (Utils::Distance(destinyPos, position) < 15)
			{
				state = BossState::Jump;
			}
		}
		timer += dt;
		direction = Utils::GetNormal(destinyPos - GetPosition());
		velocity += gravity * dt;
		position += velocity * dt;
		SetPosition(position);
		SetPosition(GetPosition() + direction * dt * 50.f);

		if (velocity.y > 150)
		{
			timer = 0;
		}

		break;
	case BossState::Jump:
		if (onHit)
		{
			body.setColor(Utils::RandomColor());
			if (hitTimer > 1)
			{
				onHit = false;

				body.setColor(sf::Color(0xffffffff));

			}
		}
		if (timer == 0)
		{
			velocity.y = -151;
		}
		timer += dt;
		hitTimer += dt;
		direction = Utils::GetNormal(player->GetPosition() - GetPosition());
		velocity += gravity * dt;
		position += velocity * dt;
		SetPosition(position);
		SetPosition(GetPosition() + direction * dt * 20.f);

		if (velocity.y > 150) timer = 0;
		break;
	case BossState::Skill1:

		if (onHit)
		{
			body.setColor(Utils::RandomColor());
			if (hitTimer > 1)
			{
				onHit = false;

				body.setColor(sf::Color(0xffffffff));
			}
		}
		if (timer == 0)
		{
			velocity.y = -151;
		}
		timer += dt;
		hitTimer += dt;

		if (page1)
		{
			direction = Utils::GetNormal(point1 - GetPosition());

			if (Utils::Distance(point1, position) < 5)
			{
				page1 = false;
			}
		}
		else
		{
			direction = sf::Vector2f(0, 1);
		}

		SetPosition(GetPosition() + direction * dt * 80.f);
		if (Utils::Distance({ point1.x, -point1.y }, position) < 20)
		{
			page1 = true;
		}
		if (velocity.y > 150) timer = 0;
		break;
	case BossState::Berserk:
		body.setColor(sf::Color::Red);

		if (timer == 0)
		{
			velocity.y = -151;
		}
		timer += dt;
		hitTimer += dt;

		SetPosition(GetPosition() + direction * dt * 150.f);
	
		if (velocity.y > 150) timer = 0;

	}
	hitBox.UpdateTransform(body, GetLocalBounds());
	shadowBox.UpdateTransform(shadow, shadow.getLocalBounds());
	Enemy::Update(dt);
	for (auto& obj : interList)
	{
		if (Utils::CheckCollision(shadowBox.rect, obj->GetHitBox().rect))
		{
			CheckCollide(obj->GetHitBox());

		}
	}
	
}

void BossEnemy::Draw(sf::RenderWindow& window)
{

	window.draw(shadow);
	window.draw(body);
	hitBox.Draw(window);
}

void BossEnemy::OnDamage(int damage)
{
	if (hitTimer > 1)
	{

		SOUND_MGR.PlaySfx(SOUNDBUFFER_MGR.Get("effects/boss hit.wav"));
		hp -= damage;
		onHit = true;
		if (hp <= 0)
		{
			body.setColor(sf::Color(0xffffffff));
			SOUND_MGR.PlaySfx(SOUNDBUFFER_MGR.Get("effects/boss dies.wav"));
			Change();
		}
		hitTimer = 0;
	}

}

void BossEnemy::Change()
{
	hitBox.rect.setSize({ 0,0 });
	animator.Play("animations/bossDie.csv");
}

void BossEnemy::CheckCollide(HitBox box)
{
	sf::Vector2f position = GetPosition();
	SetPosition({ pastPosition.x, position.y });
	shadowBox.SetPosition({ pastPosition.x, position.y });
	if (Utils::CheckCollision(shadowBox.rect, box.rect)) {
		position.y = pastPosition.y;
	}
	SetPosition({ pastPosition.x, position.y });
	shadowBox.SetPosition({ position.x, pastPosition.y });
	if (Utils::CheckCollision(shadowBox.rect, box.rect)) {
		position.x = pastPosition.x;
	}
	SetPosition(position);

	shadowBox.UpdateTransform(shadow, shadow.getLocalBounds());
	if (state == BossState::Berserk)
	{
		direction = sf::Vector2f(Utils::RandomValue()*(Utils::RandomRange(0,2) % 2 == 0 ? -1:1), Utils::RandomValue() * (Utils::RandomRange(0, 2) % 2 == 0 ? -1 : 1));
	}
}

