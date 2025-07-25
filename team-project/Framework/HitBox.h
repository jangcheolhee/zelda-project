#pragma once
class HitBox
{
public:
	HitBox();

	sf::RectangleShape rect;
	bool visible = true;

	void UpdateTransform(const sf::Transformable& tr, const sf::FloatRect& localBounds);
	void Draw(sf::RenderWindow& window);
};

