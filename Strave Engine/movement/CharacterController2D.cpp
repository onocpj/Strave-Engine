#include "CharacterController2D.h"

#include "Player2D.h"
#include "Input.h"
#include "Log.h"
#include "UDT.h"
#include "Convert.h"
#include "Math.h"
#include "Collider.h"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>


namespace Strave {

	CharacterController2D::CharacterController2D() 
		: m_AssignedPlayer2D(UNDEF_PTR)
	{}
	CharacterController2D::~CharacterController2D() {}

	void CharacterController2D::AssignCharacter(Player2D& character) {

		m_AssignedPlayer2D = &character;

	}
	void CharacterController2D::Move(INPUT_AXIS axis, float inputVal, Keyboard::KEY keyBind) const {

		// If pointer to player exists
		if (m_AssignedPlayer2D != UNDEFINED_PLAYER) {
			// If specific key is pressed
			if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(keyBind))) {
				// If player is not colliding
				if (!m_AssignedPlayer2D->InCollision()) {

					// Move him in correct direction

					// If input axis is horizontal
					if (axis == INPUT_AXIS::HORIZONTAL) {
						m_AssignedPlayer2D->GetModel<sf::Sprite>()->move(sf::Vector2f(inputVal, 0.0f)); // Move player on horizontal axis
					}
					// If input axis is vertical
					if (axis == INPUT_AXIS::VERTICAL) {
						m_AssignedPlayer2D->GetModel<sf::Sprite>()->move(sf::Vector2f(0.0f, inputVal)); // Move player on vertical axis
					}

				} else { // If player is colliding
					float t_MovementValue = inputVal - (inputVal * Collider::CollidingResistance());

					// If input axis is horizontal 
					if (axis == INPUT_AXIS::HORIZONTAL) {
						m_AssignedPlayer2D->GetModel<sf::Sprite>()->move(sf::Vector2f(t_MovementValue, 0.0f)); // Move player on horizontal axis, in oposite direction
					}
					// If input axis is vertical
					if (axis == INPUT_AXIS::VERTICAL) {
						m_AssignedPlayer2D->GetModel<sf::Sprite>()->move(sf::Vector2f(0.0f, t_MovementValue)); // Move player on vertical axis, in oposite direction
					}
				}

				m_AssignedPlayer2D->SetPosition(
					Convert::ToVector2f(m_AssignedPlayer2D->GetModel<sf::Sprite>()->getPosition())
				); // Update object position to position of sprite

			}

		} else { // If pointer to player does not exist
			SV_CORE_WARN("Move: Character controller is not assigned to any character"); // Inform about it
		}

	}
	void CharacterController2D::Move(INPUT_AXIS axis, float inputVal) const {

		// If pointer to player exists
		if (m_AssignedPlayer2D != UNDEFINED_PLAYER) {
			// If player is not colliding
			if (!m_AssignedPlayer2D->InCollision()) {

				// If input axis is horizontal
				if (axis == INPUT_AXIS::HORIZONTAL) {
					m_AssignedPlayer2D->GetModel<sf::Sprite>()->move(sf::Vector2f(inputVal, 0.0f)); // Move player on horizontal axis
				}
				// If input axis is vertical
				if (axis == INPUT_AXIS::VERTICAL) {
					m_AssignedPlayer2D->GetModel<sf::Sprite>()->move(sf::Vector2f(0.0f, inputVal)); // Move player on vertical axis
				}

			} else { // If player is colliding
				float t_MovementValue = inputVal - (inputVal * Collider::CollidingResistance());

				// If input axis is horizontal 
				if (axis == INPUT_AXIS::HORIZONTAL) {
					m_AssignedPlayer2D->GetModel<sf::Sprite>()->move(sf::Vector2f(t_MovementValue, 0.0f)); // Move player on horizontal axis, in oposite direction
				}
				// If input axis is vertical
				if (axis == INPUT_AXIS::VERTICAL) {
					m_AssignedPlayer2D->GetModel<sf::Sprite>()->move(sf::Vector2f(0.0f, t_MovementValue)); // Move player on horizontal axis, in oposite direction
				}	
			}

			m_AssignedPlayer2D->SetPosition(
				Convert::ToVector2f(m_AssignedPlayer2D->GetModel<sf::Sprite>()->getPosition())
			); // Update object position to position of sprite

		} else { // If pointer to player does not exist
			SV_CORE_WARN("Move: Character controller is not assigned to any character"); // Inform about it
		}

	}

}

