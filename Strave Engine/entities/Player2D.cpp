#include "Player2D.h"

#include "../core/datatypes/UDT.h"
#include "../core/datatypes/Convert.h"
#include "GameObject2D.h"

namespace Strave {

	Player2D::Player2D(std::string name, std::weak_ptr<sf::Texture> texture, std::weak_ptr<sf::Sprite> sprite, std::weak_ptr<sf::RectangleShape> rectSprite, const Strave::Vector2f& position, const Strave::Vector2f& spriteScale)
		: RigidBody2D(OBJECT_TYPE::PLAYER, texture, sprite, rectSprite, position, spriteScale),
		m_Name(name)
	{}
	Player2D::Player2D()
		: RigidBody2D(),
		m_Name(UNDEF_STR)
	{}
	Player2D::~Player2D() {

	}

	Player2D* Player2D::Create(std::string name, sf::Texture& texture, const Strave::Vector2f& position, const Strave::Vector2f& spriteScale) {

		std::shared_ptr<sf::Sprite> t_Sprite2D = std::make_shared<sf::Sprite>();
		std::shared_ptr<sf::RectangleShape> t_RectSprite2D = std::make_shared<sf::RectangleShape>();

		t_Sprite2D->setTexture(texture);
		t_Sprite2D->setScale(Convert::ToVector2(spriteScale));
		t_Sprite2D->setPosition(Convert::ToVector2(position));

		t_RectSprite2D->setTexture(&texture);
		t_RectSprite2D->setSize(Convert::ToVector2(spriteScale));
		t_RectSprite2D->setPosition(Convert::ToVector2(position));

		return new Player2D(name, std::make_shared<sf::Texture>(texture), t_Sprite2D, t_RectSprite2D, position, spriteScale);

	}

}

