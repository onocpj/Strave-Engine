#include "WindowsWindow.h"

#include "../entities/GameObject2D.h"
#include "../core/logger/Log.h"
#include "../core/thread/Thread.h"
#include "../core/Application.h"
#include "../renderer/Renderer.h"
#include "../core/datatypes/UDT.h"

#include <iostream>


namespace Strave {

	//========================================================================================
	//									class: EngineClocks
	//========================================================================================

	sf::Clock	EngineClocks::m_Clock;
	sf::Time	EngineClocks::m_Time;
	float		EngineClocks::deltaTime;

	//========================================================================================
	//									class: Resolution
	//========================================================================================

	Resolution::Resolution(unsigned int width, unsigned int height)
		: m_Width(width), m_Height(height)
	{}
	Resolution::~Resolution() {}

	//========================================================================================
	//									class: Framerate
	//========================================================================================

	Framerate::Framerate(unsigned int framerate)
		: m_FramerateCap(framerate)
	{}
	Framerate::~Framerate() {}

	//========================================================================================
	//									class: WindowWindow
	//========================================================================================

	extern Strave::Application* StraveApplication;

	WindowsWindow::WindowsWindow()
		: m_RenderWindow(UNDEF_SMARTPTR), m_Event(UNDEF_SMARTPTR), m_Title(UNDEF_STR), m_Resolution(sf::VideoMode(0, 0)), m_Framerate(Framerate(0)),
		m_WindowStart(UNDEF_FUNC), m_WindowUpdate(UNDEF_FUNC), m_WindowEnd(UNDEF_FUNC), m_ExitInvoker(UNDEF_BOOL)
	{}
	WindowsWindow::WindowsWindow(const VIDEO_MODE& resolution, const Framerate& framerate, std::string title, const std::function<void(void)>& fStart, const std::function<void(void)>& fUpdate, const std::function<void(void)>& fEnd)
		: m_RenderWindow(UNDEF_SMARTPTR), m_Event(UNDEF_SMARTPTR), m_Title(title), m_Resolution(resolution), m_Framerate(framerate),
		m_WindowStart(fStart), m_WindowUpdate(fUpdate), m_WindowEnd(fEnd), m_ExitInvoker(UNDEF_BOOL)
	{

		m_RenderWindow = std::make_shared<sf::RenderWindow>(resolution, title);
		m_Event = std::make_shared<sf::Event>();

		m_RenderWindow->setFramerateLimit(m_Framerate.GetFramerateCap());

	}
	WindowsWindow::~WindowsWindow() {}

	void WindowsWindow::Execute(void) {
#if defined(USE_SFML)
		// initialize application trigger states
		StraveApplication->SetOnStartState(true);
		StraveApplication->SetOnExitState(false);

		// if render window is defined
		if (m_RenderWindow != UNDEF_SMARTPTR) {
			SV_CORE_INFO("Window opened");

			m_WindowStart();								// call function that exucutes upon start
			StraveApplication->GetRenderer()->PostInit();	// post initialization after all object are loaded
			
			// while window is opened
			while (m_RenderWindow->isOpen()) {

				// Make sure thread for logic update is created only once.
				// So if application state is "on start" state
				if (StraveApplication->GetOnStartState() == true) {

					// Send new task to thread pool.
					// This task is main sandbox task that is called every frame
					thread::ThreadPool::s_Pool->Enqueue([this]() {

						//============================================ Logic thread =================================================
						// This thread will resolve all the problems and tasks called in update function							|
						// Also updates engine clocks																				|
						//																											|
						while (!m_ExitInvoker) {		//																			|
							EngineClocks::Update();		// update engine clocks														|
							m_WindowUpdate();			// call function that updates logic every frame								|
						}								//																			|
						//																											|
						//===========================================================================================================
					
					});

					// Make sure on start state is not longer active
					StraveApplication->SetOnStartState(false);
				}

				//======================================== Rendering thread =================================================
				// Main thread is primary used to render individual objects and user interface								|
				//																											|
				m_RenderWindow->clear(sf::Color(0, 255, 0, 0));		// clear window content									|
				StraveApplication->GetRenderer()->RenderScene();	// call function for rendering							|
				m_RenderWindow->display();							// display window content								|
				//																											|
				//===========================================================================================================

				// while event occured
				while (m_RenderWindow->pollEvent(*m_Event)) {

					// if window close event is invoked
					if (m_Event->type == sf::Event::Closed) {
						m_RenderWindow->close(); // close rendering window

						// Call function that invoke on aplication close state.
						// Send this function to mutex, so other threads will not throw any exception when 
						// application running state changes. Mainly used, because thread class is chcecking application running state, 
						// while application is running
						thread::ThreadPool::SendToMutex(std::bind(&WindowsWindow::InvokeCloseState, this));

						m_WindowEnd(); // call function that executes upon window close state is invoked

						SV_CORE_INFO("Window closed");
					}

				}

			}

		} else {
			SV_CORE_ERROR("Undefined render window");
		}
#endif
#if defined(USE_STRAVE)
		// initialize application trigger states
		StraveApplication->SetOnStartState(true);
		StraveApplication->SetOnExitState(false);

		// if render window is defined
		if (m_RenderWindow != UNDEF_SMARTPTR) {
			SV_CORE_INFO("Window opened");

			m_WindowStart();										// call function that exucutes upon start
			StraveApplication->GetRenderer()->PostInit();	// post initialization after all object are loaded

			// while window is opened
			while (m_RenderWindow->IsRunning()) {

				// Make sure thread for logic update is created only once.
				// So if application state is "on start" state
				if (StraveApplication->GetOnStartState() == true) {

					// Send new task to thread pool.
					// This task is main sandbox task that is called every frame
					thread::ThreadPool::s_Pool->Enqueue([this]() {

						//============================================ Logic thread =================================================
						// This thread will resolve all the problems and tasks called in update function							|
						// Also updates engine clocks																				|
						//																											|
						while (!m_ExitInvoker) {		//																			|
							EngineClocks::Update();		// update engine clocks														|
							m_WindowUpdate();			// call function that updates logic every frame								|
						}								//																			|
						//																											|
						//===========================================================================================================

						});

					// Make sure on start state is not longer active
					StraveApplication->SetOnStartState(false);

				}

				//======================================== Rendering thread =================================================
				// Main thread is primary used to render individual objects and user interface								|
				//																											|
				m_RenderWindow->Clear();							// clear window content									|
				StraveApplication->GetRenderer()->RenderScene();	// call function for rendering							|
				m_RenderWindow->Display();							// display window content								|
				//																											|
				//===========================================================================================================

			}

			if (!m_RenderWindow->IsRunning()) {
				thread::ThreadPool::SendToMutex(std::bind(&WindowsWindow::InvokeCloseState, this));
			}
			if (m_RenderWindow->ExpectionThrown()) {
				thread::ThreadPool::SendToMutex(std::bind(&WindowsWindow::InvokeCloseState, this));
				SV_CORE_ERROR("function WindowsWindow::Execute: Windows thrown exception and crashed");
			}

		}
		else {
			SV_CORE_ERROR("Undefined render window");
		}
#endif

	}
	void WindowsWindow::InvokeCloseState(void) {

		m_ExitInvoker = true;
		StraveApplication->m_ApplicationTiggerState.OnExit = true;

	}

}

