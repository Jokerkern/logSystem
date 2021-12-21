// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include <spdlog/common.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/null_mutex.h>

#include <mutex>
#include <string>
#include <chrono>
#include <functional>
#include "webSocketControl.h"


#pragma once

// Simple tcp client sink
// Connects to remote address and send the formatted log.
// Will attempt to reconnect if connection drops.
// If more complicated behaviour is needed (i.e get responses), you can inherit it and override the sink_it_ method.

namespace spdlog {
namespace sinks {

template<typename Mutex>
class websocket_sink : public spdlog::sinks::base_sink<Mutex>
{
public:
    // connect to tcp host/port or throw if failed
    // host can be hostname or ip address

    explicit websocket_sink(std::shared_ptr<logsystem_server> g_webSocketControl)
    {
        this->g_webSocketControl = g_webSocketControl;
    }

    ~websocket_sink() override = default;

protected:
    void sink_it_(const spdlog::details::log_msg &msg) override
    {
        spdlog::memory_buf_t formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
        char* data = formatted.data();
        data[formatted.size()-1] = '\0';
        g_webSocketControl->sendData(data);
    }

    void flush_() override {}
    std::shared_ptr<logsystem_server> g_webSocketControl;
};

using websocket_sink_mt = websocket_sink<std::mutex>;
using websocket_sink_st = websocket_sink<spdlog::details::null_mutex>;

} // namespace sinks
//
// factory functions
//
template<typename Factory = spdlog::synchronous_factory>
inline std::shared_ptr<logger> websocket_logger_mt(const std::string &logger_name, std::shared_ptr<logsystem_server> g_webSocketControl)
{
    return Factory::template create<sinks::websocket_sink_mt>(logger_name, std::shared_ptr<logsystem_server>);
}

} // namespace spdlog
