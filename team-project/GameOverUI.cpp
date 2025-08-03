#include "stdafx.h"
#include "GameOverUI.h"

GameOverUI::GameOverUI(const std::string& name)
	: GameObject(name)
{
}

void GameOverUI::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	cursor.setPosition(pos);
}

void GameOverUI::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	cursor.setRotation(rot);
}

void GameOverUI::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	cursor.setScale(s);
}

void GameOverUI::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	cursor.setOrigin(o);
}

void GameOverUI::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(cursor, preset);
	}
}

void GameOverUI::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 20;
}

void GameOverUI::Release()
{
}

void GameOverUI::Reset()
{
	auto size = FRAMEWORK.GetWindowSizeF();
	center = { size.x * 0.5f, size.y * 0.5f };
	title.setFont(FONT_MGR.Get("fonts/DS-DIGIT.ttf"));
	title.setString("Game Over");
	title.setFillColor(sf::Color(150,150,225));
	title.setPosition(center.x, center.y - 100 );
	title.setCharacterSize(40);
	Utils::SetOrigin(title, Origins::MC);

	select.setFont(FONT_MGR.Get("fonts/DS-DIGIT.ttf"));
	select.setString("go to select?");
	select.setFillColor(sf::Color::White);
	select.setPosition(center.x, center.y + 140);
	select.setCharacterSize(24);
	Utils::SetOrigin(select, Origins::MC);

	retry.setFont(FONT_MGR.Get("fonts/DS-DIGIT.ttf"));
	retry.setString("Retry");
	retry.setFillColor(sf::Color::White);
	retry.setPosition(center.x, center.y + 200);
	retry.setCharacterSize(24);
	Utils::SetOrigin(retry, Origins::MC);

	cursor.setTexture(TEXTURE_MGR.Get("graphics/Items.png"));
	cursor.setTextureRect({ 153,272,16,16 });
	cursor.setPosition(center.x - 120.f, center.y - 40.f * selectedIndex * 60);
	cursor.setScale(-2.f, 2.f);
	Utils::SetOrigin(cursor, Origins::MC);
}

void GameOverUI::Update(float dt)
{
	timer += dt;
	if ((int)timer % 2 == 0)
	{
		cursor.setTextureRect({ 153,272,16,16 });
	}
	else
	{
		cursor.setTextureRect({ 177,272,16,16 });
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Up))
	{
		SOUND_MGR.PlaySfx(SOUNDBUFFER_MGR.Get("effects/cursor.wav"));
		selectedIndex = 0;
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Down))
	{
		SOUND_MGR.PlaySfx(SOUNDBUFFER_MGR.Get("effects/cursor.wav"));
		selectedIndex =  1;
	}
	cursor.setPosition(center.x - 150.f, center.y - 40.f + (selectedIndex + 3)  * 60);

	if (InputMgr::GetKeyDown(sf::Keyboard::Return))
	{
		if (selectedIndex == 0)
		{
			SCENE_MGR.ChangeScene(SceneIds::Select);
		}
		else
		{
			SCENE_MGR.ChangeScene((SceneIds)GAME_MGR.currentMapID);
		}
	}
}

void GameOverUI::Draw(sf::RenderWindow& window)
{
	window.draw(title);

	window.draw(cursor);
	window.draw(select);
	window.draw(retry);
	
}