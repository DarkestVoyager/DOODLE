#include "HUD.h"

HUD::HUD()
{
	m_font;
	if (!m_font.loadFromFile("assets/DoodleJump.ttf")) {
		std::cerr << "Couldnt load font" << '\n';
	}

	m_lastScore = 0; 

	m_startText.setFont(m_font);
	m_startText.setString("Press SPACE to Start");
	m_startText.setCharacterSize(50);
	m_startText.setFillColor(sf::Color::Black);
	m_startText.setPosition(240.f - m_startText.getGlobalBounds().width / 2.f, 400.f);

	m_fellText.setFont(m_font);
	m_fellText.setString("You FELL!");
	m_fellText.setCharacterSize(100);
	m_fellText.setFillColor(sf::Color::Black);
	m_fellText.setPosition(235 - m_fellText.getGlobalBounds().width / 2.f, 220.f);

	m_playAgainText.setFont(m_font);
	m_playAgainText.setString("Press SPACE to Restart");
	m_playAgainText.setCharacterSize(50);
	m_playAgainText.setFillColor(sf::Color::Black);
	m_playAgainText.setPosition(245.f - m_playAgainText.getGlobalBounds().width / 2.f, 570.f);

	m_exitGameText.setFont(m_font);
	m_exitGameText.setString("Press ESC to exit");
	m_exitGameText.setCharacterSize(35);
	m_exitGameText.setFillColor(sf::Color::Black);
	m_exitGameText.setPosition(240.f - m_exitGameText.getGlobalBounds().width / 2.f, 615.f);

	m_scoreText.setFont(m_font);
	m_scoreText.setCharacterSize(50);
	m_scoreText.setFillColor(sf::Color::Black);
	m_scoreText.setPosition(0.f, 0.f);

	m_highScoreText.setFont(m_font);
	m_highScoreText.setCharacterSize(57);
	m_highScoreText.setFillColor(sf::Color::Black);
	m_highScoreText.setPosition(0.f, 36.f);
}

void HUD::Update(uint32_t score) {
	if (score != m_lastScore) { // only update string if score changed
		m_scoreText.setString("Score: " + std::to_string(score));
		m_lastScore = score;
	}
}

void HUD::Draw(sf::RenderWindow& window) const {
	window.draw(m_scoreText);
}

void HUD::DrawStartScreen(sf::RenderWindow& window) const {
	window.draw(m_startText);
}

void HUD::DrawGameOver(sf::RenderWindow& window, uint32_t high_score) {
	window.draw(m_fellText);
	window.draw(m_playAgainText);
	window.draw(m_exitGameText);
	m_highScoreText.setString("Best: " + std::to_string(high_score));
	window.draw(m_highScoreText);
}