#include "stdafx.h"
#include "HUD.h"
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
	
	sortingLayer = SortingLayers::UI;
	sortingOrder = 5;
	body.setTexture(TEXTURE_MGR.Get("graphics/HUD.png"));

	rupeeText.setFont(FONT_MGR.Get("fonts/Neo.ttf"));
	rupeeText.setCharacterSize(18);
	rupeeText.setFillColor(sf::Color::White);
	rupeeText.setPosition(134.f, 43.f);
	rupeeText.setString("0 0 0");

	heartSprites.clear();


	for (int i = 0; i < maxHp / 2; ++i)
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
