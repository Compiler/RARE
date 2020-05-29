#pragma once
#define SPDLOG_TRACE_ON
#include <spdlog/spdlog.h>
#include <spdlog/sinks/sink.h>
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>



namespace Rave {
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
	class Logger {

		private:
		static std::shared_ptr<spdlog::logger> _coreLogger;
		static std::shared_ptr<spdlog::logger> _clientLogger;

		public:
		static void init();


		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return _coreLogger; };
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return _clientLogger; };

	};



}


#ifdef RAVE_DEBUG_BUILD

#define RAVE_CORE_TRACE(...) ::Rave::Logger::getCoreLogger()->trace(__VA_ARGS__)
#define RAVE_CORE_DEBUG(...) ::Rave::Logger::getCoreLogger()->debug(__VA_ARGS__)
#define RAVE_CORE_LOG(...) ::Rave::Logger::getCoreLogger()->info(__VA_ARGS__)
#define RAVE_CORE_WARN(...) ::Rave::Logger::getCoreLogger()->warn(__VA_ARGS__)
#define RAVE_CORE_ERROR(...) ::Rave::Logger::getCoreLogger()->error(__VA_ARGS__)
#define RAVE_CORE_FATAL(...) ::Rave::Logger::getCoreLogger()->critical(__VA_ARGS__); DebugBreak()

#else 
#define RAVE_CORE_TRACE(...)
#define RAVE_CORE_DEBUG(...)
#define RAVE_CORE_LOG(...) 
#define RAVE_CORE_WARN(...)
#define RAVE_CORE_ERROR(...) 
#define RAVE_CORE_FATAL(...) 

#endif

#define RAVE_TRACE(...) ::Rave::Logger::getClientLogger()->trace(__VA_ARGS__)
#define RAVE_DEBUG(...) ::Rave::Logger::getClientLogger()->debug(__VA_ARGS__)
#define RAVE_LOG(...) ::Rave::Logger::getClientLogger()->info(__VA_ARGS__)
#define RAVE_WARN(...) ::Rave::Logger::getClientLogger()->warn(__VA_ARGS__)
#define RAVE_ERROR(...) ::Rave::Logger::getClientLogger()->error(__VA_ARGS__)
#define RAVE_FATAL(...) ::Rave::Logger::getClientLogger()->critical(__VA_ARGS__)