#include "Logger.h"

#include <Windows.h>


namespace Strave {

	namespace svlog {

		//========================================================================================
		//									class: Logger
		//========================================================================================

		Logger::Logger()
			: m_Name(LOGGER_DEFAULT_NAME), m_Color({ SvLogColor::DEFAULT, SvLogColor::DEFAULT, SvLogColor::DEFAULT, SvLogColor::DEFAULT }), m_Time(std::chrono::system_clock::now())
		{}
		Logger::Logger(std::string name, SvLogColors_t colors) 
			: m_Name(name), m_Color(colors), m_Time(std::chrono::system_clock::now())
		{}
		Logger::~Logger() {}

		/*void Logger::info(std::string log) {

			m_Time = std::chrono::system_clock::now();
			std::time_t curr_time = std::chrono::system_clock::to_time_t(m_Time);

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), m_Color.info);

			std::cout << "[" << std::ctime(&curr_time) << "] " << m_Name << ": " << log << std::endl;

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SV_LOG_COLOR::DEFAULT);

		}*/
		void Logger::info(const char log[]) {

			m_Time = std::chrono::system_clock::now();
			std::time_t curr_time = std::chrono::system_clock::to_time_t(m_Time);

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), m_Color.info);

			std::cout << "[" << std::ctime(&curr_time) << "] " << m_Name << ": " << log << std::endl;

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SvLogColor::DEFAULT);

		}
		void Logger::error(const char log[]) {

			m_Time = std::chrono::system_clock::now();
			std::time_t curr_time = std::chrono::system_clock::to_time_t(m_Time);

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), m_Color.warn);

			std::cout << "[" << std::ctime(&curr_time) << "] " << m_Name << ": " << log << std::endl;

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SvLogColor::DEFAULT);

		}
		void Logger::warn(const char log[]) {

			m_Time = std::chrono::system_clock::now();
			std::time_t curr_time = std::chrono::system_clock::to_time_t(m_Time);

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), m_Color.error);

			std::cout << "[" << std::ctime(&curr_time) << "] " << m_Name << ": " << log << std::endl;

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SvLogColor::DEFAULT);
		}
		void Logger::trace(const char log[]) {

			m_Time = std::chrono::system_clock::now();
			std::time_t curr_time = std::chrono::system_clock::to_time_t(m_Time);

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), m_Color.trace);

			std::cout << "[" << std::ctime(&curr_time) << "] " << m_Name << ": " << log << std::endl;

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SvLogColor::DEFAULT);

		}
		void Logger::info(const std::exception& ex) {

			m_Time = std::chrono::system_clock::now();
			std::time_t curr_time = std::chrono::system_clock::to_time_t(m_Time);

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), m_Color.info);

			std::cout << "[" << std::ctime(&curr_time) << "] " << m_Name << ": " << ex.what() << std::endl;

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SvLogColor::DEFAULT);

		}
		void Logger::error(const std::exception& ex) {

			m_Time = std::chrono::system_clock::now();
			std::time_t curr_time = std::chrono::system_clock::to_time_t(m_Time);

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), m_Color.error);

			std::cout << "[" << std::ctime(&curr_time) << "] " << m_Name << ": " << ex.what() << std::endl;

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SvLogColor::DEFAULT);

		}
		void Logger::warn(const std::exception& ex) {

			m_Time = std::chrono::system_clock::now();
			std::time_t curr_time = std::chrono::system_clock::to_time_t(m_Time);

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), m_Color.warn);

			std::cout << "[" << std::ctime(&curr_time) << "] " << m_Name << ": " << ex.what() << std::endl;

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SvLogColor::DEFAULT);

		}
		void Logger::trace(const std::exception& ex) {

			m_Time = std::chrono::system_clock::now();
			std::time_t curr_time = std::chrono::system_clock::to_time_t(m_Time);

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), m_Color.trace);

			std::cout << "[" << std::ctime(&curr_time) << "] " << m_Name << ": " << ex.what() << std::endl;

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SvLogColor::DEFAULT);

		}

	}

}

