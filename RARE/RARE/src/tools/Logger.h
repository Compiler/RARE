#pragma once
#define SPDLOG_TRACE_ON
#include <spdlog/spdlog.h>
#include <spdlog/sinks/sink.h>
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>



namespace Rare {
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
	class Logger {

	private:
		static std::shared_ptr<spdlog::logger> _coreLogger;
	public:
		Logger(const Logger&) = delete;
		static void init();
		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return _coreLogger; };
	};



}


#ifdef RARE_DEBUG_BUILD

#define RARE_TRACE(...) ::Rare::Logger::getCoreLogger()->trace(__VA_ARGS__)
#define RARE_DEBUG(...) ::Rare::Logger::getCoreLogger()->debug(__VA_ARGS__)
#define RARE_LOG(...) ::Rare::Logger::getCoreLogger()->info(__VA_ARGS__)
#define RARE_WARN(...) ::Rare::Logger::getCoreLogger()->warn(__VA_ARGS__)
#define RARE_ERROR(...) ::Rare::Logger::getCoreLogger()->error(__VA_ARGS__)
//#define RARE_FATAL(...) ::Rare::Logger::getCoreLogger()->critical(__VA_ARGS__)


#define RARE_FATAL(...) { \
	::Rare::Logger::getCoreLogger()->critical(__VA_ARGS__); \
	DebugBreak();\
}\

//#define RARE_FATAL(...); { \
//	::Rare::Logger::getCoreLogger()->critical(__VA_ARGS__); \
//	DebugBreak();\
//}\



#else 
#define RARE_TRACE(...)
#define RARE_DEBUG(...)
#define RARE_LOG(...) 
#define RARE_WARN(...)
#define RARE_ERROR(...) 
#define RARE_FATAL(...) 

#endif

//#define RARE_TRACE(...) ::Rare::Logger::getClientLogger()->trace(__VA_ARGS__)
//#define RARE_DEBUG(...) ::Rare::Logger::getClientLogger()->debug(__VA_ARGS__)
//#define RARE_LOG(...) ::Rare::Logger::getClientLogger()->info(__VA_ARGS__)
//#define RARE_WARN(...) ::Rare::Logger::getClientLogger()->warn(__VA_ARGS__)
//#define RARE_ERROR(...) ::Rare::Logger::getClientLogger()->error(__VA_ARGS__)
//#define RARE_FATAL(...) ::Rare::Logger::getClientLogger()->critical(__VA_ARGS__)