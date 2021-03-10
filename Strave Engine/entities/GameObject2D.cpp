#include "GameObject2D.h"

#include "../core/logger/Log.h"
#include "../renderer/Renderer.h"
#include "../core/Application.h"
#include "../core/datatypes/Convert.h"
#include "container/ObjectContainer.h"

namespace Strave {

	//========================================================================================
	//									class: GameObject2D
	//========================================================================================

	sf::Texture GameObject2D::s_EmptyTexture = sf::Texture();

	GameObject2D::GameObject2D()
		: m_ObjectType(OBJECT_TYPE::BASIC),
		m_Texture2D(UNDEF_SMARTPTR), m_Sprite2D(UNDEF_SMARTPTR), m_RectSprite2D(UNDEF_SMARTPTR),
		m_Position(UNDEF_VECTOR2F), m_SpriteScale(UNDEF_VECTOR2F), m_Key(UNDEF_OBJECT_KEY)
	{}
	GameObject2D::GameObject2D(OBJECT_TYPE objType, std::weak_ptr<sf::Texture> texture, std::weak_ptr<sf::Sprite> sprite, std::weak_ptr<sf::RectangleShape> rectSprite, const Strave::Vector2f& position, const Strave::Vector2f& spriteScale)
		: m_ObjectType(objType),
		m_Texture2D(texture), m_Sprite2D(sprite), m_RectSprite2D(rectSprite),
		m_Position(position), m_SpriteScale(spriteScale), m_SpriteSize(spriteScale),
		m_Key(UNDEF_OBJECT_KEY)
	{
		Init();
	}
	GameObject2D::~GameObject2D() {}

	void GameObject2D::Init(void) 
	{
		ObjectContainer::Get()->Insert(m_ObjectType, *this);
	}
	void GameObject2D::UpdateTexture(const sf::Texture& texture) {

		m_Sprite2D->setTexture(texture, true);
		m_RectSprite2D->setTexture(&texture, true);

	}
	void GameObject2D::SetPosition(const Strave::Vector2f& position) {

		m_Position = position;
		m_Sprite2D->setPosition(Convert::ToVector2(position));
		m_RectSprite2D->setPosition(Convert::ToVector2(position));

	}
	void GameObject2D::SetSpriteScale(const Strave::Vector2f& scale) {

		m_SpriteScale = scale;
		m_Sprite2D->setScale(Convert::ToVector2(scale));

	}
	void GameObject2D::SetSpriteSize(const Strave::Vector2f& size) {

		m_SpriteSize = size;
		m_RectSprite2D->setSize(Convert::ToVector2(size));

	}

	GameObject2D* GameObject2D::Create(sf::Texture& texture, const Strave::Vector2f& position, const Strave::Vector2f& spriteScale) {

		std::shared_ptr<sf::Sprite> t_Sprite2D = std::make_shared<sf::Sprite>();
		std::shared_ptr<sf::RectangleShape> t_RectSprite2D = std::make_shared<sf::RectangleShape>();

		t_Sprite2D->setTexture(texture);
		t_Sprite2D->setScale(Convert::ToVector2(spriteScale));
		t_Sprite2D->setPosition(Convert::ToVector2(position));

		t_RectSprite2D->setTexture(&texture);
		t_RectSprite2D->setSize(Convert::ToVector2(spriteScale));
		t_RectSprite2D->setPosition(Convert::ToVector2(position));

		return new GameObject2D(OBJECT_TYPE::BASIC, std::make_shared<sf::Texture>(texture), t_Sprite2D, t_RectSprite2D, position, spriteScale);

	}

}
