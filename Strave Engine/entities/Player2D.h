#pragma once

#include <string>

#include "RigidBody2D.h"

namespace Strave {

	class SV_SANDBOX_API Player2D : public RigidBody2D {

	public:
		~Player2D();

		/**
			Creates player

			@param: fPath - file path of texture for the object
			@param: position - object starting position
			@param: spriteScale - scale of the texture
		*/
		static Player2D* Create(std::string name, sf::Texture& texture, const Strave::Vector2f& position, const Strave::Vector2f& spriteScale = GAME_OBJECT_2D_SCALE);

	protected:
		Player2D(std::string name, std::weak_ptr<sf::Texture> texture, std::weak_ptr<sf::Sprite> sprite, std::weak_ptr<sf::RectangleShape> rectSprite, const Strave::Vector2f& position, const Strave::Vector2f& spriteScale);
		Player2D();

	private:
		std::string m_Name;

	};

}

