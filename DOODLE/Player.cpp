#include "Player.h"
#include <fstream> 
#include <iostream>

Player::Player(sf::Texture& idle_texture, sf::Texture& jump_texture, float speed)
{
    m_idleTexture = &idle_texture;
    m_jumpTexture = &jump_texture;
	m_speed = speed;

	m_faceRight = true;
    m_spacePressed = false;
    m_isGrounded = false;
    m_showingIdle = false;
    m_highestY = 720.0f;
    m_highScore = 0;

    std::ifstream file("HIGH_SCORE.txt");
    if (file.is_open()) 
        file >> m_highScore;

    if (!m_startSoundBuffer.loadFromFile("assets/start.wav")) {
        std::cerr << "Couldnt load start sound\n";
    }
    m_startSound.setBuffer(m_startSoundBuffer);

    if (!m_jumpSoundBuffer.loadFromFile("assets/jump.wav")) {
        std::cerr << "Couldnt load jump sound\n";
    }
    m_jumpSoundPool.reserve(MAX_SIMULTANEOUS_SOUNDS);
    for (uint16_t i = 0; i < MAX_SIMULTANEOUS_SOUNDS; ++i) {
        m_jumpSoundPool.emplace_back(m_jumpSoundBuffer);
    }

    m_body.setScale(1.0f, 1.0f);
	m_body.setOrigin(62.0f, 60.0f);
	m_body.setTexture(*m_idleTexture);
	m_body.setPosition(240.0f, 720.0f); 
}

void Player::Draw(sf::RenderWindow& window) {
	window.draw(m_body);
}

void Player::Update(float deltaTime, std::vector<Platform>& platforms) {
    
    InputHandling();
    ApplyGravity(deltaTime);
    UpdateSprite();

    for (auto& platform : platforms) {
        if (m_velocity.y > 0 && // cade 
            GetHitbox().intersects(platform.GetHitbox()) ) {
            JumpSound();
            m_isGrounded = true;
            m_velocity.y = -850.0f; // sare automat | was -600.f
        }
    }

    if (m_showingIdle && m_idleClock.getElapsedTime().asSeconds() > 0.25f) {
        m_showingIdle = false;
        m_body.setTexture(*m_jumpTexture);
        m_body.setPosition(m_body.getPosition().x, m_body.getPosition().y + 10.f);
    }

    m_body.move(m_movement * deltaTime);
    m_body.move(m_velocity * deltaTime);
}

void Player::InputHandling()
{
    m_movement = sf::Vector2f(0.0f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        m_movement.x -= m_speed;
        m_faceRight = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        m_movement.x += m_speed;
        m_faceRight = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !m_spacePressed) {
        m_spacePressed = true;
        m_velocity.y = -850.0f; // jump | was -600.0f
        m_startSound.play();
    } 
}

void Player::ApplyGravity(float deltaTime) {
    m_velocity.y += 1400.0f * deltaTime; // gravity | was 980.0f
}

void Player::UpdateSprite() {
    // flip based on direction
    if (m_faceRight) {
        m_body.setTextureRect(sf::IntRect(0, 0, 124, 120));       // normal
    }
    else {
        m_body.setTextureRect(sf::IntRect(124, 0, -124, 120));    // flipped
    }
    ChangeSpriteTexture();
}

void Player::ChangeSpriteTexture() {
    if (m_showingIdle) return;

    if (!m_isGrounded) 
        m_body.setTexture(*m_jumpTexture);
    else {
        m_body.setTexture(*m_idleTexture);
        m_body.setPosition(m_body.getPosition().x, m_body.getPosition().y - 10.f);
        m_idleClock.restart(); // start counting
        m_showingIdle = true; 
    }
    m_isGrounded = false;
}

void Player::JumpSound() {
    for (sf::Sound& jumpSound : m_jumpSoundPool) {
        if (jumpSound.getStatus() == sf::Sound::Stopped) {
            jumpSound.play();
            break;
        }
    }
}

sf::FloatRect Player::GetHitbox() const
{
    sf::Vector2f pos = m_body.getPosition();
    if (!m_faceRight) {
        return sf::FloatRect(
            pos.x + 22.f,   // right
            pos.y + 55.f,   // top (near feet, not top of head)
            -50.f,          // width
            5.f            // height (thin strip at the bottom)
        );
    } else {
        return sf::FloatRect(
            pos.x - 22.f,   // left
            pos.y + 55.f,   // top (near feet, not top of head)
            50.f,           // width
            5.f            // height (thin strip at the bottom)
        );
    }
}
void Player::DrawDebug(sf::RenderWindow& window) const
{
    sf::RectangleShape debugBox;
    sf::FloatRect hb = GetHitbox();
    debugBox.setPosition(hb.left, hb.top);
    debugBox.setSize(sf::Vector2f(hb.width, hb.height));
    debugBox.setFillColor(sf::Color::Transparent);
    debugBox.setOutlineColor(sf::Color::Red);
    debugBox.setOutlineThickness(3.f);
    window.draw(debugBox);
}

float Player::GetViewY(unsigned int VIEW_HEIGHT) 
{
    float currentY = m_body.getPosition().y;

    if (currentY < m_highestY) m_highestY = currentY;

    if (m_highestY < static_cast<float>(VIEW_HEIGHT) / 3) {
        return m_highestY;
    }
    return static_cast<float>(VIEW_HEIGHT) / 2;
}

uint32_t Player::GetScore() const {
    // score = how far above starting Y (720) the player has climbed
    // higher up = smaller Y value in SFML
    if (m_highestY < 720.f)
        return static_cast<uint32_t>(((720.f - m_highestY) * 0.2f));
    return 0;
}

void Player::SaveHighScore() {
    uint32_t currentScore = GetScore();
    if (currentScore > m_highScore)
        m_highScore = currentScore;

    std::ofstream file("HIGH_SCORE.txt");
    if (file.is_open())
        file << m_highScore;
}

uint32_t Player::GetHighScore() const {
    return m_highScore;
}

void Player::Reset()
{
    m_body.setPosition(240.f, 720.f);
    m_velocity = sf::Vector2f(0.f, 0.f);
    m_movement = sf::Vector2f(0.f, 0.f);
    m_highestY = 720.f;
    m_spacePressed = false;
}