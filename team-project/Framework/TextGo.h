#pragma once
#include "GameObject.h"

class TextGo : public GameObject
{
protected:
	sf::Text text;
	std::string fontId;

public:
	TextGo(const std::string& fontId = "", const std::string& name = "");
	~TextGo() override = default;

	const std::string& GetFontId() const { return fontId; }
	void SetFontId(const std::string& fId) { fontId = fId; }

	void SetString(const std::string& str);
	void SetString(const std::wstring& str);
	void SetString(const wchar_t* str);
	void SetString(const sf::String& str);
	void SetCharacterSize(unsigned int size);
	void SetFillColor(const sf::Color& color);
	std::string GetString() const;
	void SetOutlineThickness(float thickness);
	void SetOutlineColor(const sf::Color& color);

	sf::Text& GetText() { return text; }
	const sf::Text& GetText() const { return text; }

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float rot) override;
	void SetScale(const sf::Vector2f& s) override;
	void SetOrigin(const sf::Vector2f& o) override;
	void SetOrigin(Origins preset) override;


	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};