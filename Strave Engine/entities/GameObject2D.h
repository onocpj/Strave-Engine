#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

#include "../core/datatypes/Vector2.h"
#include "../animation/Animation.h"
#include "../core/datatypes/UDT.h"

#define I32_ERROR_TYPE (Strave::Int32)ERROR_TYPE
#define UNDEF_OBJECT_KEY { I32_ERROR_TYPE, I32_ERROR_TYPE }

enum class GAME_OBJECT_2D_TYPE;
#define GAME_OBJECT_2D_TYPE GAME_OBJECT_2D_TYPE

#define GAME_OBJECT_2D_SCALE				{ 1.0f, 1.0f }
#define GAME_OBJECT_2D_CONTAINER_EMPTY		1
#define GAME_OBJECT_2D_CONTAINER_NOT_EMPTY	0
#define ELEMENT_FOUND_IN_CONTAINER			1
#define ELEMENT_NOT_FOUND_IN_CONTAINER		0


namespace Strave {
	enum class SV_SANDBOX_API OBJECT_TYPE {
		UNDEFINED		= 0,
		BASIC,			
		RIGID_BODY,		
		PLAYER,				

		// User Interface
		UI,
		UI_UNDEFINED,
		UI_CELL,
		UI_ELEMENT,
		UI_TABLE,
		UI_TEXTBOX,
		UI_LABEL,
		UI_BUTTON

	};

	class SV_SANDBOX_API GameObject2D {
	private:
		struct ObjectKey 
		{
			Int32 objectKey;
			Int32 animationKey;
		};

	public:
		static sf::Texture s_EmptyTexture;

	public:
		virtual ~GameObject2D();

		inline const Strave::Vector2f& GetSpritePosition(void) const
		{
			return m_Position;
		}
		inline const Strave::Vector2f GetPosition(void) const
		{
			return Strave::Vector2f(
				m_Position.x + (m_SpriteSize.x / 2),
				m_Position.y + (m_SpriteSize.y / 2)
			);
		}
		inline const Strave::Vector2f& GetSpriteScale(void) const
		{
			return m_SpriteScale;
		}
		inline const Strave::Vector2f GetSpriteSize(void) const
		{
			return Vector2f(
				m_Texture2D->getSize().x * m_SpriteScale.x,
				m_Texture2D->getSize().y * m_SpriteScale.y
			);
		}
		inline const Strave::Vector2f& GetRectSpriteSize(void) const
		{
			return m_SpriteSize;
		}
		inline const sf::Texture& GetTexture(void) const
		{
			return *m_Texture2D;
		}
		inline OBJECT_TYPE GetObjectType(void) const
		{
			return m_ObjectType;

		}
		template<typename Type> Type* GetModel(void) const;
		template<> inline sf::Sprite* GetModel<sf::Sprite>(void) const
		{
			return &(*m_Sprite2D);
		}
		template<> inline sf::RectangleShape* GetModel<sf::RectangleShape>(void) const
		{
			return &(*m_RectSprite2D);
		}
		void UpdateTexture(const sf::Texture& texture);
		void SetPosition(const Strave::Vector2f& position);
		void SetSpriteScale(const Strave::Vector2f& scale);
		void SetSpriteSize(const Strave::Vector2f& size);

		static GameObject2D* Create(sf::Texture& texture, const Strave::Vector2f& position, const Strave::Vector2f& spriteScale = GAME_OBJECT_2D_SCALE);

	protected:
		GameObject2D(OBJECT_TYPE objType, std::weak_ptr<sf::Texture> texture, std::weak_ptr<sf::Sprite> sprite, std::weak_ptr<sf::RectangleShape> rectSprite, const Strave::Vector2f& position, const Strave::Vector2f& spriteScale);
		GameObject2D();

		void Init(void);

	private:
		friend static Int32 Animation::GetObjectAnimationKey(const GameObject2D& go2d);
		friend void Animation::AssignTo(GameObject2D& go2d);

	private:
		OBJECT_TYPE							m_ObjectType;
		std::shared_ptr<sf::Texture>		m_Texture2D;
		std::shared_ptr<sf::Sprite>			m_Sprite2D;
		std::shared_ptr<sf::RectangleShape> m_RectSprite2D;
		Strave::Vector2f					m_Position;
		Strave::Vector2f					m_SpriteScale;
		Strave::Vector2f					m_SpriteSize;

	protected:
		ObjectKey							m_Key;

	};

}

