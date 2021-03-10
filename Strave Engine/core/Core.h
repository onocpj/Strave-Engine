#pragma once

#if defined(_WIN32)
	#define SV_PLATFORM_WINDOWS
#else
	#error Strave support only Windows !

#endif //  _WIN32

// define platform running on
#if defined(SV_PLATFORM_WINDOWS)
	#define SV_SANDBOX_BUILD_AS_DLL

	#define _CRT_SECURE_NO_WARNINGS // Disable secure warnings
	#define USE_SFML	// Temporary (needs to be replaced with own input holding): Represents using SFML lib
	
	#define USE_STRAVE		
	#undef USE_STRAVE	// Undef till strave input is not complete

#endif // SV_PLATFORM_WINDOWS

#if defined SV_SANDBOX_BUILD_AS_DLL
	#define SV_SANDBOX_API __declspec(dllexport)
#else	
	#define SV_SANDBOX_API __declspec(dllimport)
#endif

#if defined(USE_SFML)
	#define USE_SFML_INPUT	
	#define USE_SFML_CAMERA
#endif
#if defined(USE_STRAVE)
	#define USE_STRAVE_INPUT
	#define USE_STRAVE_CAMERA
#endif

// Find out number of threads for specific system...
#if defined(SV_PLATFORM_WINDOWS)
	#include <thread>
	
	namespace core { 
		enum processor_count {
			one_processor		= 1,
			two_processors,
			four_processors		= 4,
			six_processors		= 6,
			eight_processors	= 8,
		}; 
	}

	const auto g_core__processor_count = std::thread::hardware_concurrency();

	#define PROCESSOR_COUNT				g_core__processor_count					// total processor count
	#define MINIMAL_PROCESSOR_COUNT		core::processor_count::two_processors	// minimal processor count for engine
	#define REQUIRED_PROCESSOR_COUNT	core::processor_count::two_processors	// required processor count
	#define MAIN_THREAD					core::processor_count::one_processor	// value represents one thread (main thread)

	#define REQUIRED_THREADS_AVAILABLE

#else
	#error Strave support only Windows !

#endif // SV_PLATFORM_WINDOWS

