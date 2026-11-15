#include "PlatformManager.h"
#include <random>

PlatformManager::PlatformManager(sf::Texture& texture, float viewHeight) {
    m_texture = &texture;
    float currentY = viewHeight; // start from bottom

    m_platforms.reserve(PLATFORM_COUNT);
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        float x = static_cast<float>(rand() % static_cast<int>(480 - 102.6f));
        currentY -= static_cast<float>(rand() % MAX_GAP + MIN_GAP); // same spacing as recycling
        m_platforms.push_back(Platform(x, currentY, texture));
    }
}

void PlatformManager::Update(float viewTop, float viewWidth, float viewHeight) {
    for (auto& platform : m_platforms) {
        if (platform.GetBound().top > viewTop + viewHeight) {

            // find highest platform and its X
            float highestY = viewTop;
            float lastX = 0.f;
            for (auto& other : m_platforms) {
                if (other.GetBound().top < highestY) {
                    highestY = other.GetBound().top;
                    lastX = other.GetBound().left;
                }
            }

            // random position clamped to max horizontal distance from last platform
            float randomX = std::clamp(
                static_cast<float>(rand() % static_cast<int>(viewWidth - 102.6f)),
                lastX - 250.f,
                lastX + 250.f
            );
            float randomY = highestY - static_cast<float>(rand() % MAX_GAP + MIN_GAP);

            // overlap check
            sf::FloatRect newBounds(randomX, randomY, 102.6f, 27.f);
            bool overlaps = false;
            for (auto& other : m_platforms) {
                if (newBounds.intersects(other.GetBound())) {
                    overlaps = true;
                    break;
                }
            }

            if (!overlaps)
                platform.SetPosition(randomX, randomY);
        }
    }
}

void PlatformManager::Draw(sf::RenderWindow& window) const
{	
	for (auto& platform : m_platforms) {
		platform.Draw(window);
	}
}


void PlatformManager::DrawDebug(sf::RenderWindow& window) const
{	
	for (auto& platform : m_platforms) {
		platform.DrawDebug(window);
	}
}

void PlatformManager::Reset(float viewHeight) {
    m_platforms.clear();
    float currentY = viewHeight;
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        float x = static_cast<float>(rand() % static_cast<int>(480 - 102.6f));
        currentY -= static_cast<float>(rand() % MAX_GAP + MIN_GAP);
        m_platforms.push_back(Platform(x, currentY, *m_texture)); // dereference
    }
}