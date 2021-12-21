#include "pch.h"

logControl::logControl()
{
}


logControl::~logControl()
{
}

void logControl::open(const char* name, const char* path, std::shared_ptr<logsystem_server> g_webSocketControl) {
	spdlog::sinks::tcp_sink_config cfg("127.0.0.1", 9999);
   ////auto my_logger = spdlog::tcp_logger_mt("tcplog", cfg);
   ////my_logger->set_level(spdlog::level::debug);
   ////my_logger->info("hello world");
   auto udp_sink = std::make_shared<spdlog::sinks::tcp_sink_mt>(cfg);
   //console_sink->set_level(spdlog::level::trace);
   //console_sink->set_pattern("[multi_sink_example] [%^%l%$] %v");

	auto websocket_sink = std::make_shared<spdlog::sinks::websocket_sink_mt>(g_webSocketControl);
	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path);
	spdlog::logger logger(name, { websocket_sink, file_sink, udp_sink });
	this->my_logger = std::make_shared<spdlog::logger>(logger);
	//this->my_logger = spdlog::basic_logger_mt(name, path);
}

void logControl::write(LOGLEVEL type, const char* log) {
	switch (type)
	{
	case debug:
		this->my_logger->debug(log);
		break;

	case info:
		this->my_logger->info(log);
		break;
	case warn:
		this->my_logger->warn(log);
		break;
	case error:
		this->my_logger->error(log);
		break;
	case critical:
		this->my_logger->critical(log);
		break;
	default:
		break;
	}
	this->my_logger->flush();
}
void logControl::close() {
	spdlog::drop_all();
	spdlog::shutdown();
}