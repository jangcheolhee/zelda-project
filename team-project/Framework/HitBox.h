#pragma once
class HitBox
{
public:
	HitBox();

	sf::RectangleShape rect;
	bool visible = true;
	sf::Vector2f position;
	sf::Vector2f origin;
	Origins originPreset = Origins::TL;

	sf::Vector2f GetPosition() const { return position; }
	void SetPosition(const sf::Vector2f& pos) { position = pos; }
	sf::Vector2f GetOrigin() const { return origin; }
	void SetOrigin(const sf::Vector2f& o)
	{
		originPreset = Origins::Custom;
		origin = o;
	}
	void SetOrigin(Origins preset) { originPreset = preset; }

	void UpdateTransform(const sf::Transformable& tr, const sf::FloatRect& localBounds);
	void UpdateTransformCollision(const sf::Transformable& tr, const sf::FloatRect& localBounds, const sf::Vector2f& pos);
	void Draw(sf::RenderWindow& window);
};