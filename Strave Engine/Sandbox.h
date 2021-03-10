#pragma once

#include "Strave.h"

namespace nSandbox {

	class Sandbox : public Strave::Application 
	{
	public:
		Sandbox() : Strave::Application() {}
		~Sandbox() {}

		virtual void Start(void) const override;
		virtual void Update(void) const override;
		virtual void End(void) const override;
	};

}

Strave::Application* Strave::CreateApplication() {
	return new nSandbox::Sandbox();
}

