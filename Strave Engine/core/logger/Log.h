#pragma once

#include "Logger.h"

// Log additional libraries
#include <memory>


namespace Strave {

	namespace svlog {

		class Log abstract {
		
		public:
			inline static void Init(std::string coreLoggerName, std::string clientLoggerName, SvLogColors_t coreLoggerColors, SvLogColors_t clientLoggerColors) {

				m_CoreLogger = std::make_shared<Strave::svlog::Logger>(coreLoggerName, coreLoggerColors);
				m_ClientLogger = std::make_shared<Strave::svlog::Logger>(clientLoggerName, clientLoggerColors);

			}

			inline static std::shared_ptr<Strave::svlog::Logger>& GetCoreLogger() { return m_CoreLogger; }
			inline static std::shared_ptr<Strave::svlog::Logger>& GetClientLogger() { return m_ClientLogger; }

		private:
			static std::shared_ptr<Strave::svlog::Logger> m_CoreLogger;
			static std::shared_ptr<Strave::svlog::Logger> m_ClientLogger;

		};

	}

}

// Core Logger
#define SV_CORE_INFO(...)	::Strave::svlog::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SV_CORE_WARN(...)	::Strave::svlog::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SV_CORE_ERROR(...)	::Strave::svlog::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SV_CORE_TRACE(...)	::Strave::svlog::Log::GetCoreLogger()->trace(__VA_ARGS__)

// Client Logger
#define SV_CLIENT_INFO(...)	::Strave::svlog::Log::GetClientLogger()->info(__VA_ARGS__)
#define SV_CLIENT_WARN(...)	::Strave::svlog::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SV_CLIENT_ERROR(...)	::Strave::svlog::Log::GetClientLogger()->error(__VA_ARGS__)
#define SV_CLIENT_TRACE(...)	::Strave::svlog::Log::GetClientLogger()->trace(__VA_ARGS__)

