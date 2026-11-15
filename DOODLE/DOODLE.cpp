#include "HUD.h"
#include "PlatformManager.h"
#include "Player.h"
#include <iostream>
#include <ctime>
#include <string>
#include <Windows.h>


enum class GameState{ WaitingToStart, Playing, GameOver};


int main() {
	srand(static_cast<unsigned int>(time(nullptr)));
	// Window & view
	constexpr float VIEW_HEIGHT = 800.f;
	constexpr float VIEW_WIDTH = 480.f;
	sf::RenderWindow window(sf::VideoMode(VIEW_WIDTH, VIEW_HEIGHT), "Doodle Jump", sf::Style::Close); // might use resize in future
	window.setVerticalSyncEnabled(true); // VSync
	SetPriorityClass(GetCurrentProcess, HIGH_PRIORITY_CLASS); // tells Windows to prioritize game over background tasks

	sf::View view(sf::FloatRect(0, 0, 480, 800));
	float currentViewY = VIEW_HEIGHT / 2.f;
	
	// Textures 
	sf::Texture doodle_texture;
	if (!doodle_texture.loadFromFile("assets/doodle_body.png")) {
		std::cerr << "Couldnt load Doodle texture" << '\n';
	}
	
	sf::Texture doodle_jump_texture;
	if (!doodle_jump_texture.loadFromFile("assets/doodle_body_jump.png")) {
		std::cerr << "Couldnt load Doodle texture" << '\n';
	}
	sf::Texture platform_texture;
	if (!platform_texture.loadFromFile("assets/tile.png")) {
		std::cerr << "Couldnt load tile texture" << '\n';
	}

	doodle_texture.setSmooth(true);
	doodle_jump_texture.setSmooth(true);
	platform_texture.setSmooth(true);


	Player doodle(doodle_texture, doodle_jump_texture,  200.0f);
	PlatformManager platformManager(platform_texture, VIEW_HEIGHT);
	HUD hud;

	sf::Clock clock;

	bool debug = false;
	GameState game_state = GameState::WaitingToStart;
	//-----------GAME LOOP------------------------------------------------
	while (window.isOpen())
	{
		float deltaTime = std::min(clock.restart().asSeconds(), 0.05f);
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type) {
			case  sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::F1) {
					debug = !debug;
					std::cout << "Debug Mode ";
					switch (debug) {
					case 1: std::cout << "\033[32mON\033[0m\n"; break;
					case 0: std::cout << "\033[31mOFF\033[0m\n"; break;
					default: std::cout << "UNAVAILABLE" << '\n'; break;
					}
				}
				if (event.key.code == sf::Keyboard::Space &&
					game_state == GameState::WaitingToStart || game_state == GameState::GameOver) {

					if (game_state == GameState::GameOver) {
						doodle.Reset();
						platformManager.Reset(VIEW_HEIGHT);
						currentViewY = VIEW_HEIGHT / 2.f; // reset camera too
					}
					game_state = GameState::Playing;
				}
				break;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			window.close();
		}

		// UPDATE - only when playing
		if (game_state == GameState::Playing) {
			float targetViewY = doodle.GetViewY(VIEW_HEIGHT);
			currentViewY += (targetViewY - currentViewY) * 5.f * deltaTime;
			view.setCenter(VIEW_WIDTH / 2.f, currentViewY);

			float viewTop = view.getCenter().y - VIEW_HEIGHT / 2.f;
			platformManager.Update(viewTop, 480.f, VIEW_HEIGHT);
			doodle.Update(deltaTime, platformManager.GetPlatforms());

			if (doodle.GetPosition().y > view.getCenter().y + (VIEW_HEIGHT / 2.f) + 200.f) {
				game_state = GameState::GameOver;
				doodle.SaveHighScore();
			}
		}

		// DRAW - always
		window.clear(sf::Color::White);
		window.setView(view);

		platformManager.Draw(window);
		if (debug) platformManager.DrawDebug(window);
		doodle.Draw(window);
		if (debug) doodle.DrawDebug(window);

		hud.Update(doodle.GetScore());

		// UI - drawn in default view so it stays fixed on screen
		window.setView(window.getDefaultView());
		hud.Draw(window);
		if (game_state == GameState::WaitingToStart) hud.DrawStartScreen(window);
		if (game_state == GameState::GameOver)     hud.DrawGameOver(window, doodle.GetHighScore());
		window.display();
	}
}