#include "Platform.h"

Platform::Platform(float x, float y, sf::Texture& texture)
{	
	m_platform_body.setScale(SCALE, SCALE);
	m_platform_body.setTexture(texture);
	m_platform_body.setPosition(x, y);
}

void Platform::SetPosition(float x, float y)
{
	m_platform_body.setPosition(x, y);
}


void Platform::Draw(sf::RenderWindow& window) const {
	window.draw(m_platform_body);
}

sf::FloatRect Platform::GetHitbox() const
{	
	sf::Vector2f pos = m_platform_body.getPosition();
	return sf::FloatRect(
	pos.x + 7.f,
	pos.y + 2.f,
	50.f * SCALE, // multiply by scale
	8.f * SCALE
	);
}

void Platform::DrawDebug(sf::RenderWindow& window) const
{
	sf::RectangleShape platformDebugBox;
	sf::FloatRect hb = GetHitbox();
	platformDebugBox.setPosition(hb.left, hb.top);
	platformDebugBox.setSize(sf::Vector2f(hb.width, hb.height));
	platformDebugBox.setFillColor(sf::Color::Transparent);
	platformDebugBox.setOutlineColor(sf::Color::Blue);
	platformDebugBox.setOutlineThickness(3.f);
	window.draw(platformDebugBox);
}