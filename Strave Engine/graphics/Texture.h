#pragma once

#include <SFML\Graphics\Texture.hpp>

#include "../core/Core.h"


namespace Strave 
{
	class SV_SANDBOX_API Texture : public sf::Texture {
	public:
		Texture();
		~Texture();

		static Texture* LoadTexture(std::string filePath);

	private:

	};
}

