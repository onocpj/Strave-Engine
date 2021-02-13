#pragma once

#include "Core.h"

#if defined(USE_SFML) 
	// SFML libs
	#include <SFML/Graphics/RenderWindow.hpp>
#endif

// Additive libs
#include <memory>

#include "Vector2.h"


#if defined(USE_SFML_CAMERA) 
	#define CAMERA_VIEW	sf::View
#endif
#if defined(USE_STRAVE_CAMERA) 
	#define CAMERA_VIEW	Strave::Graphics::View2D
#endif


namespace Strave {

	class GameObject2D;
	class WindowsWindow;

	class GameCamera2D {

	public:
		~GameCamera2D();

		/**
			Creates new GameCamera2D

			@param: initialView - initial position of camera
			@param: viewportSize - camera viewport size
		*/
		static GameCamera2D* Create(const Strave::Vector2f& initialView, const Strave::Vector2f& viewportSize);

		/**
			Assigns character to game camera

			@param: object - object that will be assigned to the game camera
		*/
		void AssignTo(GameObject2D& object);
		/**
			Updates viewport

			@param: window - for this window will be camera viewport updated
		*/
		void UpdateViewport(const WindowsWindow& window);
		inline const Vector2f& GetPosiiton(void) const { return m_CameraPosition; }

	protected:
		GameCamera2D(std::weak_ptr<CAMERA_VIEW> view, const Strave::Vector2f& initialView, const Strave::Vector2f& viewportSize);
		GameCamera2D();

	private:
		std::shared_ptr<CAMERA_VIEW>	m_Camera2D;
		Vector2f			m_CameraPosition;
		Vector2f			m_ViewportSize;
		GameObject2D*			m_AssignedGameObject2D;

	};

}

