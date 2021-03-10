#pragma once

#include "../logger/Log.h"
#include "../datatypes/UDT.h"
#include "../datatypes/DT.h"

#include <chrono>
#include <thread>
#include <iostream>

namespace Strave 
{
	class Timer
	{
	public:
		inline Timer(String title = UNDEF_STR)
			: m_Title(title), m_Duration(NULL)
		{
			m_Start = std::chrono::steady_clock::now();
		}
		inline ~Timer() 
		{
			float ms = UNDEF_FLOAT;

			m_End = std::chrono::steady_clock::now();
			m_Duration = m_End - m_Start;

			ms = m_Duration.count() * 1000.0f;

			SV_CORE_INFO("Timer duration time");
			std::cout << m_Title << ": " << ms << "ms" << std::endl;
		}

	private:
		String m_Title;
		std::chrono::time_point<std::chrono::steady_clock> m_Start;
		std::chrono::time_point<std::chrono::steady_clock> m_End;
		std::chrono::duration<float> m_Duration;
	};
}

