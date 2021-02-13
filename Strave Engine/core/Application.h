#pragma once


#define UNDEF_APP_TRIGGER	{ 0, 0 }


namespace Strave {

	namespace NRenderer { class Renderer; }

	class WindowsWindow;
	class Application {

	private:
		friend WindowsWindow;

		typedef struct {

			bool OnStart;
			bool OnExit;

		} AppStateTrigger_t;

	public:
		Application();
		virtual ~Application();

		virtual void Start(void) const	= 0;
		virtual void Update(void) const	= 0;
		virtual void End(void) const	= 0;

		inline const bool& GetOnStartState(void) { return m_ApplicationTiggerState.OnStart; }
		inline const bool& GetOnExitState(void) { return m_ApplicationTiggerState.OnExit; }

		inline Strave::WindowsWindow* GetWindow(void) const { return m_MainWindow; }
		inline Strave::NRenderer::Renderer* GetRenderer(void) const { return m_Renderer; }

		void Init(Strave::WindowsWindow* window);

	protected:
		inline void SetOnStartState(bool state) {

			m_ApplicationTiggerState.OnStart = state;

		}
		inline void SetOnExitState(bool state) {

			m_ApplicationTiggerState.OnExit = state;

		}

	private:
		AppStateTrigger_t		m_ApplicationTiggerState;
		Strave::WindowsWindow*		m_MainWindow;
		Strave::NRenderer::Renderer* 	m_Renderer;

	};

	Strave::Application* CreateApplication();

}

