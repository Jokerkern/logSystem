// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include "framework.h"
#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"  // support for loading levels from the environment variable
#include "spdlog/fmt/ostr.h" // support for user defined types
#include "spdlog/sinks/basic_file_sink.h"
#include "tcp_sink.h"
#include "websocket_sink.h"
#include "webSocketControl.h"
#include "logControl.h"

#include <memory>
#include <set>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <thread>
#include <time.h>
#include <Windows.h>

enum LOGLEVEL {
	debug,
	info,
	warn,
	error,
	critical
};

#endif //PCH_H
