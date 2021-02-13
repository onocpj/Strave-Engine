#pragma once

#include <iostream>

#if defined(SV_PLATFORM_WINDOWS)
	#if defined(REQUIRED_THREADS_AVAILABLE)

	namespace Strave { Strave::Application* StraveApplication; }
	extern Strave::Application* Strave::CreateApplication();

	int main(int argc, char* argv[]) {

		// ...my code...

		Strave::WindowsWindow*	window;

		Strave::StraveApplication = Strave::CreateApplication(); // create application instance

		// Initializing logger
		Strave::svlog::Log::Init("CORE", "CLIENT",
			{ Strave::svlog::SvLogColor::BLUE, Strave::svlog::SvLogColor::RED, Strave::svlog::SvLogColor::YELLOW, Strave::svlog::SvLogColor::GRAY },
			{ Strave::svlog::SvLogColor::BLUE, Strave::svlog::SvLogColor::RED, Strave::svlog::SvLogColor::YELLOW, Strave::svlog::SvLogColor::GRAY }
		);

		// Initializing thread pool
		Strave::thread::ThreadPool::s_Pool = Strave::thread::ThreadPool::Create(Strave::thread::ProcessorInit_t() = { PROCESSOR_COUNT, MINIMAL_PROCESSOR_COUNT, REQUIRED_PROCESSOR_COUNT });

		// Initializing main window
		window = new Strave::WindowsWindow(
			sf::VideoMode(1280, 720),						// window resolution
			Strave::Framerate(0),							// window framerate cap
			"Strave Engine - Test",							// window title
			std::bind(&Strave::Application::Start, Strave::StraveApplication),	// window function upon start
			std::bind(&Strave::Application::Update, Strave::StraveApplication),	// window function on update
			std::bind(&Strave::Application::End, Strave::StraveApplication)		// window function upon end
		);

		Strave::StraveApplication->Init(window);	// initialize application
		window->Execute();				// execute window

		// free up memory
		delete Strave::StraveApplication;
		delete window;
		Strave::thread::ThreadPool::s_Pool->Delete();

		SV_CORE_INFO("Engine closed");

		return 0;
	}

	#else
		#error System have not enough threads !
	#endif // REQUIRED_THREADS_AVAILABLE
#endif // SV_PLATFORM_WINDOWS
