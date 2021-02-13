#pragma once

#include "Core.h"

#include <string>
#include <chrono>
#include <ctime>
#include <iostream>


#define LOGGER_DEFAULT_NAME "LOGGER"


namespace Strave {

	namespace svlog {

		enum SvLogColor {

			UNKNOWN	= 0,
			RED	= 4,
			PURPLE	= 5,
			GRAY	= 8,
			BLUE	= 9,
			GREEN	= 10,
			PINK	= 13,
			YELLOW	= 14,
			WHITE	= 15,
			DEFAULT = 15

		};

		enum ArgDataType {

			STRING	= 0,
			INT,
			LONG_INT,
			LONG_LONG_INT,
			DOUBLE,
			FLOAT,
			EXCEPTION

		};

		typedef struct {
			unsigned int info, warn, error, trace;

		} SvLogColors_t;

		class Logger {

		public:
			Logger();
			Logger(std::string name, SvLogColors_t colors);
			~Logger();

			template<typename Arg_t> inline int infoc(Arg_t arg, const ArgDataType dt[], unsigned int argsLeft) {

				if (argsLeft == 1) {
					std::cout << arg << std::endl;

					return 1;

				} else {
					std::cout << arg << std::endl;

					return 0;
				}

			}
			template<typename... Arg_t> inline void infoc(Arg_t... args, const ArgDataType dt[]) {

				unsigned int t_NumOfArgs = (sizeof(dt) / sizeof(dt[0]));

				if (info(args..., &dt, t_NumOfArgs - 1) == 0) {
					info(args..., &dt, t_NumOfArgs - 1);

				}

			}
			void info(const char log[]);
			void error(const char log[]);
			void warn(const char log[]);
			void trace(const char log[]);
			void info(const std::exception& ex);
			void error(const std::exception& ex);
			void warn(const std::exception& ex);
			void trace(const std::exception& ex);

		private:
			std::string				m_Name;
			SvLogColors_t				m_Color;
			std::chrono::system_clock::time_point	m_Time;

		};

	}

}

