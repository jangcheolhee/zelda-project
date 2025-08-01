#include "stdafx.h"
#include "SaveSlotUI.h"

SaveSlotUI::SaveSlotUI(const std::string& name)
	: GameObject(name)
{
}

void SaveSlotUI::SetSlotData(int index, int heartCount)
{
	if (index < 0 || index >= slots.size()) return;

	Slot& s = slots[index];

	for (int i = 0; i < s.hearts.size(); ++i)
	{
		s.hearts[i].setColor(i < heartCount ? sf::Color::White : sf::Color(100, 100, 100, 100));
	}
}

void SaveSlotUI::SetState(MenuState newState)
{
	state = newState;
	UpdateUIByState();
}

void SaveSlotUI::UpdateUIByState()
{
	switch (state)
	{
	case MenuState::SelectSlot:
		title.setString("Select Player");
		Utils::SetOrigin(title, Origins::MC);
		break;
	case MenuState::ConfirmDelete:
		title.setString("Check delete slot?");
		Utils::SetOrigin(title, Origins::MC);
		break;
	}
}

void SaveSlotUI::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	body.setPosition(pos);
}

void SaveSlotUI::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	body.setRotation(rot);
}

void SaveSlotUI::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	body.setScale(s);
}

void SaveSlotUI::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	body.setOrigin(o);
}

void SaveSlotUI::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(body, preset);
	}
}

void SaveSlotUI::Init()
{

	sortingLayer = SortingLayers::UI;
	sortingOrder = 0;

}

void SaveSlotUI::Release()
{
}

void SaveSlotUI::Reset()
{
	auto size = FRAMEWORK.GetWindowSizeF();
	center = { size.x * 0.5f, size.y * 0.5f };


	title.setFont(FONT_MGR.Get("fonts/DS-DIGIT.ttf"));

	title.setString("Select Player");
	title.setFillColor(sf::Color::White);
	title.setPosition(center.x, center.y - 160);
	title.setCharacterSize(24);
	Utils::SetOrigin(title, Origins::MC);

	slots.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		if (GAME_MGR.CheckSlot(i))
		{
			Slot& s = slots[i];
			s.icon.setTexture(TEXTURE_MGR.Get("graphics/Link.png"));
			if (GAME_MGR.currentMapID == 1)
			{
				s.icon.setTextureRect({ 0, 0, 16, 24 });
			}
			else if (GAME_MGR.currentMapID == 2)
			{
				s.icon.setTextureRect({ 0, 112, 16, 24 });
			}
			s.icon.setScale(2.f, 2.f);
			Utils::SetOrigin(s.icon, Origins::MC);

			s.hearts.resize(GAME_MGR.playerHp);
			for (int j = 0; j < s.hearts.size(); ++j)
			{
				s.hearts[j].setTexture(TEXTURE_MGR.Get("graphics/Items.png"));
				s.hearts[j].setTextureRect({ 104,280,8,8 });
				s.hearts[j].setScale(1.5f, 1.5f);
			}
		}
		copyText.setFont(FONT_MGR.Get("fonts/DS-DIGIT.ttf"));
		copyText.setString("Copy");
		copyText.setFillColor(sf::Color::White);
		copyText.setPosition(center.x, center.y + 140);
		copyText.setCharacterSize(24);
		Utils::SetOrigin(copyText, Origins::MC);

		deleteText.setFont(FONT_MGR.Get("fonts/DS-DIGIT.ttf"));
		deleteText.setString("Delete");
		deleteText.setFillColor(sf::Color::White);
		deleteText.setPosition(center.x, center.y + 200);
		deleteText.setCharacterSize(24);
		Utils::SetOrigin(deleteText, Origins::MC);

		body.setTexture(TEXTURE_MGR.Get("graphics/Items.png"));
		body.setTextureRect({ 153,272,16,16 });
		body.setPosition(center.x - 120.f, center.y - 40.f * selectedIndex * slotSpacing);
		body.setScale(-2.f, 2.f);
		Utils::SetOrigin(body, Origins::MC);

		}
		
}

void SaveSlotUI::Update(float dt)
{
	timer += dt;
	if ((int)timer % 2 == 0)
	{
		body.setTextureRect({ 153,272,16,16 });
	}
	else
	{
		body.setTextureRect({ 177,272,16,16 });
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Up))
	{
		selectedIndex = selectedIndex - 1 < 0 ? 0 : selectedIndex - 1;
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Down))
	{
		selectedIndex = selectedIndex + 1 > 4 ? 4 : selectedIndex + 1;
	}
	body.setPosition(center.x - 150.f, center.y - 40.f + selectedIndex * slotSpacing);
	if (InputMgr::GetKeyDown(sf::Keyboard::Return))
	{
		if (state == MenuState::SelectSlot)
		{
			if (selectedIndex < 3 && onSlotSelected)
				onSlotSelected(selectedIndex);
			else if (selectedIndex == 4)
				SetState(MenuState::ConfirmDelete);
		}
		else if (state == MenuState::ConfirmDelete)
		{
			if (selectedIndex < 3 && onSlotDelete)
				onSlotDelete(selectedIndex);

			SetState(MenuState::SelectSlot);
		}
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		if (state == MenuState::ConfirmDelete)
			SetState(MenuState::SelectSlot);

		
	}

}

void SaveSlotUI::Draw(sf::RenderWindow& window)
{
	window.draw(title);
	for (int i = 0; i < slots.size(); ++i)
	{
		if (GAME_MGR.CheckSlot(i))
		{
			float y = center.y - 40.f + i * slotSpacing;
			Slot& s = slots[i];

			s.icon.setPosition(sf::Vector2f{ center.x - 110.f, y });
			window.draw(s.icon);

			for (int j = 0; j < s.hearts.size(); ++j)
			{
				s.hearts[j].setPosition(center.x + 90.f + j * 18.f, y + 5);
				window.draw(s.hearts[j]);
			}
		}
		
	}
	window.draw(body);
	window.draw(copyText);
	window.draw(deleteText);

}