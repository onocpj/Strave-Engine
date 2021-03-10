#include "Log.h"

namespace Strave {

	namespace svlog {

		std::shared_ptr<Strave::svlog::Logger> Log::m_CoreLogger;
		std::shared_ptr<Strave::svlog::Logger> Log::m_ClientLogger;

	}

}
