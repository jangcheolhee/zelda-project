#include "stdafx.h"
#include "BossEnemy.h"
BossEnemy::BossEnemy(const std::string& name)
	: GameObject(name)
{
}

void BossEnemy::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	body.setPosition(pos);
	shadow.setPosition(pos + sf::Vector2f{0,24});
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
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;

	Utils::SetOrigin(body, Origins::BC);
	Utils::SetOrigin(shadow, Origins::BC);


	

}

void BossEnemy::Release()
{
}
void BossEnemy::OnDamage(int dmg)
{

	hp=hp - dmg;
	if (hp < 0)
	{
		hp = 0;
		SetActive(false);
	}
}


void BossEnemy::Reset()
{
	
	body.setTexture(TEXTURE_MGR.Get("graphics/Boss.png"));
	shadow.setTexture(TEXTURE_MGR.Get("graphics/Boss.png"));
	body.setTextureRect({ 207, 39, 34, 34 });
	shadow.setTextureRect({ 198,206,24,16 });
	state = BossState::Stop;
	SetPosition(startPos);
}

void BossEnemy::Update(float dt)
{
	
	switch (state)
	{
	case BossState::Stop:
		pastPos = body.getPosition();
		timer += dt;
		if (Utils::RandomRange(0, 2) == 0)
		{
			SetPosition({ GetPosition().x - 0.05f ,GetPosition().y});
			
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
		
		if (velocity.y >150)
		{
			timer = 0;
			
		}
		
		break;
	case BossState::Jump:


		break;
	}
	
	

	
	hitBox.UpdateTransform(body, GetLocalBounds());
}

void BossEnemy::Draw(sf::RenderWindow& window)
{
	
	window.draw(shadow);
	window.draw(body);
	hitBox.Draw(window);
}
