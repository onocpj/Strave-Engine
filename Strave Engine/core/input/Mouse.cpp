#include "Mouse.h"

#if defined(USE_SFML_INPUT)
	#include <SFML/Window/Window.hpp>
#endif

#include "WindowsWindow.h"
#include "GameObject2D.h"
#include "UserInterface.h"
#include "Convert.h"
#include "Vector2.h"
#include "Log.h"

#if defined(SV_PLATFORM_WINDOWS)
	#include <Windows.h>
	#include <conio.h>
	#include <iostream>
#endif


namespace Strave {

	Mouse::Mouse(WindowsWindow& window)
		: m_WindowReference(UNDEF_PTR)
	{
		Init(window);
	}
	Mouse::~Mouse() {}

	void Mouse::Init(WindowsWindow& window) {

		m_WindowReference = &window.GetWindow();
		m_Cursor.Reference = std::make_shared<sf::Mouse>();

		// Double click init
		m_DoubleClick.LapsedTime = 0.0f;
		m_DoubleClick.Active = false;
		// Click init
		m_Click.ClickedObjectHolder = UNDEF_PTR;
		m_Click.ClickFlag = false;

	}

#if defined(USE_SFML_INPUT)
	bool Mouse::IsButtonPressed(Mouse::Button button) {

		return m_Cursor.Reference->isButtonPressed(static_cast<sf::Mouse::Button>(button));

	}
	bool Mouse::IsButtonPressed(Mouse::Button button, const GUI::UserInterface& obj) {

		// if cursor is hovering over button
		if (Mouse::Hover(obj)) {
			// if mouse button is pressed
			if (m_Cursor.Reference->isButtonPressed(static_cast<sf::Mouse::Button>(button))) {
				return true; // click state return true
			}
		}

		return false; // click state return false

	}
	bool Mouse::IsButtonPressed(Mouse::Button button, const GameObject2D& obj) {

		// if cursor is hovering over button
		if (Mouse::Hover(obj)) {
			// if mouse button is pressed
			if (m_Cursor.Reference->isButtonPressed(static_cast<sf::Mouse::Button>(button))) {
				return true; // click state return true
			}
		}

		return false; // click state return false

	}
#endif
#if defined(USE_STRAVE_INPUT)
	bool Mouse::IsButtonPressed(Mouse::BUTTON button) {...}
	bool Mouse::IsButtonPressed(Mouse::BUTTON button, const GUI::UserInterface& obj) {...}
	bool Mouse::IsButtonPressed(Mouse::BUTTON button, const GameObject2D& obj) {...}
#endif
	bool Mouse::Click(Mouse::Button button) {

		return IsButtonPressed(button);

	}
	bool Mouse::Click(Mouse::Button button, const GUI::UserInterface& obj) {

		// if cursor is hovering over object
		if (Mouse::Hover(obj)) {
			// if mouse button is pressed
			if (m_Cursor.Reference->isButtonPressed(static_cast<sf::Mouse::Button>(button))) {
				return true; // click state return true
			}
		}

		return false; // click state return false

	}
	bool Mouse::Click(Mouse::Button button, const GameObject2D& obj) {

		// if cursor is hovering over object
		if (Mouse::Hover(obj)) {
			// if mouse button is pressed
			if (m_Cursor.Reference->isButtonPressed(static_cast<sf::Mouse::Button>(button))) {
				return true; // click state return true
			}
		}

		return false; // click state return false

	}
	bool Mouse::DoubleClick(Mouse::Button button) {

		if (Mouse::Released(button)) {
			m_DoubleClick.Active = true;
		}

		if (m_DoubleClick.Active == true) {
			m_DoubleClick.LapsedTime += EngineClocks::deltaTime;

			if (m_DoubleClick.LapsedTime <= MOUSE_DOUBLE_CLICK_DELAY) {
				if (Mouse::Click(button)) {
					m_DoubleClick.LapsedTime = 0.0f;
					m_DoubleClick.Active = false;

					return true;
				}

			} else {
				m_DoubleClick.LapsedTime = 0.0f;
				m_DoubleClick.Active = false;
			}
		}

		return false;

	}
	bool Mouse::DoubleClick(Mouse::Button button, const GUI::UserInterface& obj) {

		if (!Hover(obj)) {
			m_DoubleClick.Active = false;
			m_DoubleClick.LapsedTime = 0.0f;
		}

		if (Mouse::Released(button, obj)) {
			m_DoubleClick.Active = true;
			SV_CORE_INFO("Released");
		}

		if (m_DoubleClick.Active == true) {
			m_DoubleClick.LapsedTime += EngineClocks::deltaTime;

			if (m_DoubleClick.LapsedTime <= MOUSE_DOUBLE_CLICK_DELAY) {
				if (Mouse::Click(button, obj)) {
					m_DoubleClick.LapsedTime = 0.0f;
					m_DoubleClick.Active = false;

					SV_CORE_INFO("Double clicked");

					return true;
				}

			} else {
				m_DoubleClick.LapsedTime = 0.0f;
				m_DoubleClick.Active = false;
			}
		}

		return false;

	}
	bool Mouse::DoubleClick(Mouse::Button button, const GameObject2D& obj) {

		if (!Hover(obj)) {
			m_DoubleClick.Active = false;
			m_DoubleClick.LapsedTime = 0.0f;
		}

		if (Mouse::Released(button, obj)) {
			m_DoubleClick.Active = true;
		}

		if (m_DoubleClick.Active == true) {
			m_DoubleClick.LapsedTime += EngineClocks::deltaTime;

			if (m_DoubleClick.LapsedTime <= MOUSE_DOUBLE_CLICK_DELAY) {
				if (Mouse::Click(button, obj)) {
					m_DoubleClick.LapsedTime = 0.0f;
					m_DoubleClick.Active = false;

					return true;
				}

			} else {
				m_DoubleClick.LapsedTime = 0.0f;
				m_DoubleClick.Active = false;
			}
		}

		return false;

	}
	bool Mouse::DoubleClick(Mouse::Button button, float clickDelay) {

		if (Mouse::Released(button)) {
			m_DoubleClick.Active = true;
		}

		if (m_DoubleClick.Active == true) {
			m_DoubleClick.LapsedTime += EngineClocks::deltaTime;

			if (m_DoubleClick.LapsedTime <= clickDelay) {
				if (Mouse::Click(button)) {
					m_DoubleClick.LapsedTime = 0.0f;
					m_DoubleClick.Active = false;

					return true;
				}

			} else {
				m_DoubleClick.LapsedTime = 0.0f;
				m_DoubleClick.Active = false;
			}
		}

		return false;

	}
	bool Mouse::DoubleClick(Mouse::Button button, float clickDelay, const GUI::UserInterface& obj) {

		if (!Hover(obj)) {
			m_DoubleClick.Active = false;
			m_DoubleClick.LapsedTime = 0.0f;
		}

		if (Mouse::Released(button, obj)) {
			m_DoubleClick.Active = true;
		}

		if (m_DoubleClick.Active == true) {
			m_DoubleClick.LapsedTime += EngineClocks::deltaTime;

			if (m_DoubleClick.LapsedTime <= clickDelay) {
				if (Mouse::Click(button, obj)) {
					m_DoubleClick.LapsedTime = 0.0f;
					m_DoubleClick.Active = false;

					return true;
				}

			} else {
				m_DoubleClick.LapsedTime = 0.0f;
				m_DoubleClick.Active = false;
			}
		}

		return false;

	}
	bool Mouse::DoubleClick(Mouse::Button button, float clickDelay, const GameObject2D& obj) {

		if (!Hover(obj)) {
			m_DoubleClick.Active = false;
			m_DoubleClick.LapsedTime = 0.0f;
		}

		if (Mouse::Released(button, obj)) {
			m_DoubleClick.Active = true;
		}

		if (m_DoubleClick.Active == true) {
			m_DoubleClick.LapsedTime += EngineClocks::deltaTime;

			if (m_DoubleClick.LapsedTime <= clickDelay) {
				if (Mouse::Click(button, obj)) {
					m_DoubleClick.LapsedTime = 0.0f;
					m_DoubleClick.Active = false;

					return true;
				}

			}
			else {
				m_DoubleClick.LapsedTime = 0.0f;
				m_DoubleClick.Active = false;
			}
		}

		return false;

	}
	bool Mouse::Released(Mouse::Button button) {

		if (Mouse::Click(button)) {
			if (m_Click.ClickFlag == false) {
				m_Click.ClickFlag = true;
			}
		}

		if (m_Click.ClickFlag == true) {
			if (!Mouse::Click(button)) {
				m_Click.ClickFlag = false;

				return true;
			}
		}

		return false;

	}
	bool Mouse::Released(Mouse::Button button, const GUI::UserInterface& obj) {

		if (Mouse::Click(button, obj)) {
			if (m_Click.ClickFlag == false) {
				m_Click.ClickFlag = true;
			}
		}

		if (m_Click.ClickFlag == true) {
			if (!Mouse::Click(button, obj)) {
				m_Click.ClickFlag = false;

				return true;
			}
		}

		return false;

	}
	bool Mouse::Released(Mouse::Button button, const GameObject2D& obj) {

		if (Mouse::Click(button, obj)) {
			if (m_Click.ClickFlag == false) {
				m_Click.ClickFlag = true;
			}
		}

		if (m_Click.ClickFlag == true) {
			if (!Mouse::Click(button, obj)) {
				m_Click.ClickFlag = false;

				return true;
			}
		}

		return false;

	}
	bool Mouse::Hold(Mouse::Button button) {

		if (Mouse::Click(button)) {
			return true;
		}

		return false;

	}
	bool Mouse::Hold(Mouse::Button button, const GUI::UserInterface& obj) {

		if (Mouse::Click(button, obj)) {
			return true;
		}

		return false;

	}
	bool Mouse::Hold(Mouse::Button button, const GameObject2D& obj) {

		if (Mouse::Click(button, obj)) {
			return true;
		}

		return false;

	}
	bool Mouse::Hover(const GUI::UserInterface& obj) {

		Vector2i t_MousePosition = Convert::ToVector2i(m_Cursor.Reference->getPosition(*m_WindowReference));

		if (t_MousePosition.x >= obj.GetPosition().x &&
			t_MousePosition.y >= obj.GetPosition().y) {

			if (t_MousePosition.x <= obj.GetPosition().x + obj.GetRectSpriteSize().x &&
				t_MousePosition.y <= obj.GetPosition().y + obj.GetRectSpriteSize().y) {

				return true; // if mouse cursor is hovering interface instance return true
			}
		}

		return false; // if mouse cursor is not hovering interface instance return false

	}
	bool Mouse::Hover(const GameObject2D& obj) {

		Vector2i t_MousePosition = UNDEF_VECTOR2I;
		Vector2f t_TranslatedPosition = UNDEF_VECTOR2F;

		t_MousePosition = Convert::ToVector2i(m_Cursor.Reference->getPosition(*m_WindowReference));	// Mouse position relative to the window
		t_TranslatedPosition = Convert::ToVector2f(m_WindowReference->mapPixelToCoords(sf::Vector2i(
			t_MousePosition.x, t_MousePosition.y
		)));																						// Mouse position translated into world coordinates
		
		if (t_TranslatedPosition.x >= obj.GetPosition().x &&
			t_TranslatedPosition.y >= obj.GetPosition().y) {

			if (t_TranslatedPosition.x <= obj.GetPosition().x + obj.GetSpriteSize().x &&
				t_TranslatedPosition.y <= obj.GetPosition().y + obj.GetSpriteSize().y) {

				return true; 
			}
		}

		return false; 

	}
	bool Mouse::Drag(GUI::UserInterface& obj) {
		// Optimize this function !!!

		m_Cursor.Position = Convert::ToVector2f(m_Cursor.Reference->getPosition(*m_WindowReference));
		obj.SetPosition(Vector2f(
			m_Cursor.Position.x - obj.GetRectSpriteSize().x / 2,
			m_Cursor.Position.y - obj.GetRectSpriteSize().y / 2
		));

		return true;

	}
	bool Mouse::Drag(GameObject2D& obj) {

		Vector2f t_OldObjectPosition = UNDEF_VECTOR2F;
		Vector2f t_NewObjectPosition = UNDEF_VECTOR2F;
		Vector2f t_TranslatedPosition = UNDEF_VECTOR2F;

		t_OldObjectPosition = obj.GetPosition();
		t_TranslatedPosition = Convert::ToVector2f(m_WindowReference->mapCoordsToPixel(
			sf::Vector2f(t_OldObjectPosition.x, t_OldObjectPosition.y)
		));

		m_Cursor.Position = Convert::ToVector2f(m_Cursor.Reference->getPosition(*m_WindowReference));

		t_NewObjectPosition = Convert::ToVector2(m_WindowReference->mapPixelToCoords(sf::Vector2i(
			m_Cursor.Position.x - obj.GetSpriteSize().x / 2, 
			m_Cursor.Position.y - obj.GetSpriteSize().y / 2
		)));

		obj.SetPosition(t_NewObjectPosition);

		return true;

	}

}
