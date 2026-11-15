#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class HUD
{
public:
	HUD();
	void Update(uint32_t score);
    void Draw(sf::RenderWindow& window) const;
    void DrawStartScreen(sf::RenderWindow& window) const;
    void DrawGameOver(sf::RenderWindow& window, uint32_t high_score);

private:
    uint16_t m_lastScore;
    sf::Font m_font;
    sf::Text m_scoreText;
    sf::Text m_startText;
    sf::Text m_fellText;
    sf::Text m_playAgainText;
    sf::Text m_exitGameText;
    sf::Text m_highScoreText;
};