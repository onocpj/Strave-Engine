#include "GameCamera2D.h"

#include "../window/WindowsWindow.h"
#include "../entities/GameObject2D.h"
#include "../core/Application.h"
#include "../renderer/Renderer.h"
#include "../core/logger/Log.h"
#include "../core/datatypes/UDT.h"
#include "../core/datatypes/Convert.h"

namespace Strave {

	extern Strave::Application* StraveApplication;

	GameCamera2D::GameCamera2D(std::weak_ptr<CAMERA_VIEW> view, const Strave::Vector2f& initialView, const Strave::Vector2f& viewportSize)
		: m_Camera2D(view), m_CameraPosition(initialView), m_ViewportSize(viewportSize), m_AssignedGameObject2D(UNDEF_PTR)
	{}
	GameCamera2D::GameCamera2D() 
		: m_Camera2D(UNDEF_SMARTPTR), m_CameraPosition(UNDEF_VECTOR2F), m_ViewportSize(UNDEF_VECTOR2F), m_AssignedGameObject2D(UNDEF_PTR)
	{}

	GameCamera2D::~GameCamera2D() {}

	GameCamera2D* GameCamera2D::Create(const Strave::Vector2f& initialView, const Strave::Vector2f& viewportSize) {

		GameCamera2D* t_GameCamera2D;

		// create new camera controller
		std::shared_ptr<CAMERA_VIEW> t_Camera2D = std::make_shared<CAMERA_VIEW>(Convert::ToVector2(initialView), Convert::ToVector2(viewportSize));
		t_GameCamera2D = new GameCamera2D(t_Camera2D, initialView, viewportSize);

		Strave::StraveApplication->GetRenderer()->SetGameCamera2DReference(*t_GameCamera2D); // tell to renderer, that new camera controller has been created

		return t_GameCamera2D; // return camera controller

	}
	void GameCamera2D::AssignTo(GameObject2D& object) {

		m_AssignedGameObject2D = &object;

	}
	void GameCamera2D::UpdateViewport(const WindowsWindow& window) {
#if defined(USE_SFML_CAMERA)
		if (m_AssignedGameObject2D != UNDEF_PTR) {
			window.SetView(*m_Camera2D);
			m_Camera2D->setCenter(Convert::ToVector2(m_AssignedGameObject2D->GetSpritePosition()));

			m_CameraPosition = Convert::ToVector2f(m_Camera2D->getCenter());

		} else {
			SV_CORE_WARN("UpdateView: No character assigned ot game camera");
		}
#endif
#if defined(USE_STRAVE_CAMERA)
		if (m_AssignedGameObject2D != UNDEF_PTR) {
			window.SetView(*m_Camera2D);
			m_Camera2D->SetCenterPoint(m_AssignedGameObject2D->GetPosition());

			m_CameraPosition = m_Camera2D->GetCenterPoint();

		}
		else {
			SV_CORE_WARN("UpdateView: No character assigned ot game camera");
		}
#endif
	}

}
