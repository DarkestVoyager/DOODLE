#pragma once
#include <SFML/Graphics.hpp>


class Platform
{
public:

	Platform(float x, float y, sf::Texture& texture);
	void SetPosition(float x, float y);

	void Draw(sf::RenderWindow& window) const; 

	sf::FloatRect GetBound() const { return m_platform_body.getGlobalBounds(); };
	sf::FloatRect GetHitbox() const;
	void DrawDebug(sf::RenderWindow& window) const;

private:
	static constexpr float SCALE = 1.8f;
	sf::Sprite m_platform_body;
};