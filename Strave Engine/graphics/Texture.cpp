#include "Texture.h"

namespace Strave {

	Texture::Texture() :
		sf::Texture() 
	{

	}
	Texture::~Texture() {}

	Texture* Texture::LoadTexture(std::string filePath) {
		Texture* t_Texture = new Texture();
		t_Texture->loadFromFile(filePath);

		return t_Texture;
	}

}