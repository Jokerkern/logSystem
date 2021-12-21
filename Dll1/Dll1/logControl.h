#pragma once
#include "pch.h"
class logControl
{
private:
	std::shared_ptr<spdlog::logger> my_logger;
public:
	logControl();
	~logControl();
	void open(const char*, const char*, std::shared_ptr<logsystem_server>);
	void write(enum LOGLEVEL, const char*);
	void close();
};
 