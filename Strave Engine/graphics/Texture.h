#pragma once

#include <SFML\Graphics\Texture.hpp>

namespace Strave {

	class Texture : public sf::Texture {
	public:
		Texture();
		~Texture();

		static Texture* LoadTexture(std::string filePath);

	private:

	};

}

