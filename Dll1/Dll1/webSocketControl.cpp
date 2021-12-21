#include "webSocketControl.h"
#include <fstream>
#include <unordered_set>

	logsystem_server::logsystem_server() {
		// Initialize Asio Transport
		m_server.init_asio();

		// Register handler callbacks
		m_server.set_open_handler(bind(&logsystem_server::on_open, this, ::_1));
		m_server.set_close_handler(bind(&logsystem_server::on_close, this, ::_1));
		m_server.set_message_handler(bind(&logsystem_server::on_message, this, ::_1, ::_2));
		m_server.clear_access_channels(websocketpp::log::alevel::all);
	}

	void logsystem_server::run(uint16_t port) {
		// listen on specified port
		m_server.listen(port);

		// Start the server accept loop
		m_server.start_accept();

		// Start the ASIO io_service run loop
		try {
			m_server.run();
		}
		catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}

	void logsystem_server::on_open(connection_hdl hdl) {
		
			std::ifstream din("dir.txt");
			std::string addr;
			std::string line;
			std::unordered_set<std::string> addrs;
			while (getline(din, addr)) {
				addrs.insert(addr);
			}
			din.close();
			std::ofstream dout("dir.txt", std::ios::trunc | std::ios::out);
			for (auto& i : addrs) {
				dout << i << std::endl;
			}

			for (auto& a : addrs) {
				std::ifstream fin(a);
				while (getline(fin, line)) {
					m_server.send(hdl, line, websocketpp::frame::opcode::text);
				}
				fin.close();
			}
			m_connections.insert(hdl);
	}

	void logsystem_server::on_close(connection_hdl hdl) {
		m_connections.erase(hdl);
	}

	void logsystem_server::on_message(connection_hdl hdl, server::message_ptr msg) {
		//msg->get_payload();
	}
	void logsystem_server::sendData(const char* msg) {
		con_list::iterator it;
		for (it = m_connections.begin(); it != m_connections.end(); ++it) {
			m_server.send(*it, msg, websocketpp::frame::opcode::text);
		}
	}

