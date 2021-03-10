#include "Application.h"

#include "../core/thread/Thread.h"
#include "../window/WindowsWindow.h"
#include "../renderer/Renderer.h"
#include "../renderer/RenderingConstraints.h"
#include "../core/datatypes/UDT.h"
#include "../entities/container/ObjectContainer.h"
#include "../entities/GameObject2D.h"
#include "../animation/AnimationContainer.h"
#include "../interface/UserInterface.h"

#include <iostream>


namespace Strave {

	//========================================================================================
	//									class: Application
	//========================================================================================

	Application::Application() 
		: m_ApplicationTiggerState(UNDEF_APP_TRIGGER),
		m_MainWindow(UNDEF_PTR), 
		m_Renderer(UNDEF_PTR)
	{}
	Application::~Application() {
	
		AnimationContainer::Delete();
		m_Renderer->Delete();
		delete ObjectContainer::Get();

	}

	void Application::Init(WindowsWindow* window) {

		RenderingConstraints* myConstraints = new RenderingConstraints();
		myConstraints->SetInitPoint(Vector2f(750.0f, 550.0f));
		myConstraints->SetPreloadRadius(1000.0f);
		myConstraints->SetRenderingRadius(800.0f);

		m_MainWindow = window;
		m_Renderer = new Renderer(*window, *myConstraints);

	}

}
