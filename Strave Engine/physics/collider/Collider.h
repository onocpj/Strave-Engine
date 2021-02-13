#pragma once

#include <SFML/Graphics.hpp>

#define DEFAULT_COLLISION_ANGLE     90.0f

namespace Strave {
	namespace Collider {

		bool PixelPerfectTest(const sf::Sprite& Object1, const sf::Sprite& Object2, sf::Uint8 AlphaLimit = 0);
		bool CreateTextureAndBitmask(sf::Texture& LoadInto, const std::string& Filename);
		bool CircleTest(const sf::Sprite& Object1, const sf::Sprite& Object2);
		bool BoundingBoxTest(const sf::Sprite& Object1, const sf::Sprite& Object2);

		sf::Vector2f GetSpriteCenter(const sf::Sprite& Object);
		sf::Vector2f GetSpriteSize(const sf::Sprite& Object);

		float CollidingResistance(float angle = DEFAULT_COLLISION_ANGLE);

	}
}