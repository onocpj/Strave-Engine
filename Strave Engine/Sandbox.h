#pragma once

#include "Strave.h"

namespace NSandbox {

	class Sandbox : public Strave::Application {

	private:

	public:
		Sandbox() : Strave::Application() {}
		~Sandbox() {}

		virtual void Start(void) const override;
		virtual void Update(void) const override;
		virtual void End(void) const override;

	};

}

Strave::Application* Strave::CreateApplication() {
	return new NSandbox::Sandbox();
}

