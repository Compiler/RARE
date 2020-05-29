#include "Logger.h"

#include "spdlog/sinks/basic_file_sink.h" 
namespace Rare {


	std::shared_ptr<spdlog::logger> Logger::_coreLogger(spdlog::stdout_color_mt("Rare"));
	//std::shared_ptr<spdlog::logger> Logger::_clientLogger;

	void Logger::init() {
		//RARE_ASSERT(_imguiConsole != nullptr);

		_coreLogger->set_pattern("%^[%T]%@ %n: %v%$");
		spdlog::set_pattern("%^[%T]%@ %n: %v%$");

		auto colorSink = std::make_shared < spdlog::sinks::stdout_color_sink_mt>();
		//auto mySink = std::make_shared<Rare::my_sink_mt>();

		_coreLogger->set_level(spdlog::level::trace);

		//_clientLogger = spdlog::stdout_color_mt("RareApp");
		_coreLogger->set_level(spdlog::level::trace);


	}




}