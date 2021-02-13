#include "Application.h"

#include "Thread.h"
#include "WindowsWindow.h"
#include "Renderer.h"
#include "RenderingConstraints.h"
#include "UDT.h"
#include "ObjectContainer.h"
#include "GameObject2D.h"
#include "AnimationContainer.h"
#include "UserInterface.h"

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
		delete ObjectContainer::s_Container;

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
