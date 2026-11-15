#pragma once
#include <vector>
#include "Platform.h"
#include <SFML/Audio.hpp>

class Player
{
public:	
	Player(sf::Texture& idle_texture, sf::Texture& jump_texture, float speed);

	void Draw(sf::RenderWindow& window);
	void Update(float deltaTime, std::vector<Platform>& platforms);

	sf::FloatRect GetHitbox() const;

	void DrawDebug(sf::RenderWindow& window) const;

	float GetViewY(unsigned int VIEW_HEIGHT);
	uint32_t GetScore() const;
	uint32_t GetHighScore() const;
	void SaveHighScore();

	sf::Vector2f GetPosition() { return m_body.getPosition(); }
	sf::Vector2f GetMovement() const { return m_movement; }

	void Reset();

private:
	sf::Sprite m_body;
	sf::Texture* m_idleTexture;
	sf::Texture* m_jumpTexture;
	sf::SoundBuffer m_jumpSoundBuffer;
	sf::SoundBuffer m_startSoundBuffer;
	sf::Sound m_startSound;
	sf::Clock m_idleClock;

	float m_speed;
	float m_highestY;
	std::uint32_t m_highScore;
	bool m_faceRight;
	bool m_spacePressed;
	bool m_isGrounded;
	bool m_showingIdle;

	static constexpr std::uint16_t MAX_SIMULTANEOUS_SOUNDS = 3;

	// helper functions
	void InputHandling();
	void ApplyGravity(float deltaTime);
	void UpdateSprite();
	void ChangeSpriteTexture();
	void JumpSound();

	std::vector<sf::Sound> m_jumpSoundPool;
	sf::Vector2f m_movement;
	sf::Vector2f m_velocity;
};