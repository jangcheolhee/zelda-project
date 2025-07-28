#pragma once
#include "Interactable.h"
class JumpWall : public Interactable
{
    sf::RectangleShape body;
public:
    // Interactable을(를) 통해 상속됨
    void OnInteract() override;
    void Init() override;
    void Reset() override;
    void SetDirection(Direction d) { direction = d; }
    void SetBounds(float x, float y, float width, float height);
	sf::FloatRect GetGlobalBounds() const override
	{

		sf::FloatRect rect = body.getGlobalBounds();
		rect.left -= 2.f;
		rect.top -= 2.f;
		rect.width += 4.f;
		rect.height += 4.f;
		return rect;
	}
	void Draw(sf::RenderWindow& window) override;
};