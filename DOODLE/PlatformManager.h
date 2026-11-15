#pragma once
#include "Platform.h"
#include <vector>

class PlatformManager
{
public:
	PlatformManager(sf::Texture& texture, float viewHeight);
	void Draw(sf::RenderWindow& window) const;
	void Update(float viewTop, float VIEW_WIDTH, float VIEW_HEIGHT);
	void DrawDebug(sf::RenderWindow& window) const;
	std::vector<Platform>& GetPlatforms() { return m_platforms; };

	void Reset(float viewHeight);

private:
	// PLATFROM_COUNT <= VIEW_HEIGHT / MIN_GAP - 800 / 55 = 14.54
	static const int PLATFORM_COUNT = 17; // 14
	static const int MAX_GAP = 100; // 88
	static const int MIN_GAP = 45; // 55
	sf::Texture* m_texture;
	std::vector<Platform> m_platforms;
};