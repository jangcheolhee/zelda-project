#include "stdafx.h"
#include "HUD.h"
#include "GameObject.h"

HUD::HUD(const std::string& name)
	: GameObject(name)
{
}

void HUD::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	body.setPosition(pos);
}

void HUD::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	body.setRotation(rot);
}

void HUD::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	body.setScale(s);
}

void HUD::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	body.setOrigin(o);
}

void HUD::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(body, preset);
	}
}

void HUD::Init()
{
	GameObject::Init();
	hp = 6;                 // 기본 하트 3개
	maxHp = hp;
	heartCount = hp;
	rupeeCount = 0;

	sortingLayer = SortingLayers::UI;
	sortingOrder = 5;
	body.setTexture(TEXTURE_MGR.Get("graphics/HUD.png"));

	rupeeText.setFont(FONT_MGR.Get("fonts/Neo.ttf"));
	rupeeText.setCharacterSize(18);
	rupeeText.setFillColor(sf::Color::White);
	rupeeText.setPosition(134.f, 43.f);
	rupeeText.setString("0 0 0");

	heartSprites.clear();


	for (int i = 0; i < hp / 2; ++i)
	{
		sf::Sprite sprite;
		sprite.setScale(0.9f, 0.9f);
		sprite.setPosition(323.f + i * 17.f, 45.f);
		heartSprites.push_back(sprite);
	}
	UpdateHeartSprites();
}

void HUD::Release()
{
}

void HUD::Reset()
{

	body.setTexture(TEXTURE_MGR.Get("graphics/HUD.png"));
	body.setTextureRect({ 0,0,256,224 });
	SetScale({ 2.f,2.f });

	sf::Font& font = FONT_MGR.Get("fonts/Neo.ttf");
	rupeeText.setFont(font);


}

void HUD::Update(float dt)
{
}

void HUD::Draw(sf::RenderWindow& window)
{
	GameObject::Draw(window);
	
	
	window.draw(hudSprite);
	window.draw(body);
	window.draw(rupeeText);  // 루피 텍스트 같이 그리기

	int heartsToDraw = (hp + 1) / 2;

	if (showStatus)
	{
		window.draw(statusUI);
	}

	for (const auto& heart : heartSprites)
	{
		window.draw(heart);
	}
}

void HUD::SetRupee(int count)
{
	rupeeCount = count;
	rupeeText.setString("0 0 " + std::to_string(rupeeCount));
}

void HUD::AddRupee(int amount)
{
	rupeeCount += amount;
	rupeeText.setString("0 0 " + std::to_string(rupeeCount));
	SOUND_MGR.PlaySfx(SOUNDBUFFER_MGR.Get("effects/rupee.wav"));

}

void HUD::AddHeart(int amount)
{
	SetHeartCount(hp + amount);
}

void HUD::SetHeartCount(int currentHp)
{
	hp = currentHp;
	int fullHearts = currentHp / 2;
	bool hasHalfHeart = currentHp % 2 == 1;

	for (int i = 0; i < heartSprites.size(); ++i)
	{
		if (i < fullHearts)
		{
			heartSprites[i].setTexture(TEXTURE_MGR.Get("graphics/Heart.png"));
		}
		else if (i == fullHearts && hasHalfHeart)
		{
			//heartSprites[i].setTexture(TEXTURE_MGR.Get("graphics/Heart_half.png")); // 반 하트 이미지가 있다면
		}
		else
		{
			heartSprites[i].setTexture(TEXTURE_MGR.Get("graphics/Heart_empty.png"));
		}
	}
}

void HUD::UpdateHeartSprites()
{
	int heartsToDraw = (hp + 1) / 2;

	for (int i = 0; i < heartSprites.size(); ++i)
	{
		if (i < heartsToDraw)
		{
			heartSprites[i].setTexture(TEXTURE_MGR.Get("graphics/Heart.png"));
		}
		else
		{
			heartSprites[i].setTexture(TEXTURE_MGR.Get("graphics/Heart_empty.png"));
		}
	}
}

void HUD::SetSize(const sf::Vector2f& size)
{
	if (!hudSprite.getTexture()) return;
	auto texSize = hudSprite.getTexture()->getSize();
	float scaleX = size.x / texSize.x;
	float scaleY = size.y / texSize.y;
	hudSprite.setScale(scaleX, scaleY);
}

void HUD::ApplyBossStyle()
{
	// 텍스트 크기, 위치(보스씬에 맞게 커짐)
	rupeeText.setCharacterSize(37);    // 글자 크기 키움
	rupeeText.setPosition(255.f, 87.f); // 창 위치 조정

	// 하트도 크기/위치 커지게 조정
	for (int i = 0; i < heartSprites.size(); ++i)
	{
		heartSprites[i].setScale(1.9f, 1.9f); // 하트 크기 두배
		heartSprites[i].setPosition(632.f + i * 30.f, 89.f); // 가로로 넓게!
	}
}
