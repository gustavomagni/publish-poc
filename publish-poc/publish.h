#include "options.h"

#include <proton/connection.hpp>
#include <proton/connection_options.hpp>
#include <proton/container.hpp>
#include <proton/message.hpp>
#include <proton/message_id.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/tracker.hpp>
#include <proton/types.hpp>
#include <proton/sender_options.hpp>
#include <proton/target_options.hpp>
#include <proton/target.hpp>

#include <iostream>
#include <map>
#include <thread>
#include <chrono>

#include "fake_cpp11.h"

using proton::sender_options;
using proton::delivery_mode;
using proton::target_options;

#pragma once
class publish : public proton::messaging_handler {

private:
	std::string conn_url_{};
	std::string address_{};
	std::string message_body_{};

public:
	publish(const std::string& url, const std::string& address, const std::string& messageBody) : conn_url_(url), address_(address), message_body_(messageBody) {}


	void on_container_start(proton::container& cont) override {
		proton::connection_options co;

		cont.connect(conn_url_);
	}

	void on_connection_open(proton::connection& conn) override {
		target_options target_options;
		target_options.dynamic(false);

		sender_options sender_options;
		sender_options.target(target_options);

		conn.open_sender(address_);
	}

	void on_sender_open(proton::sender& snd) override {
		std::cout << "SEND: Opened sender for target address '" << snd.target().address() << "'\n";
	}

	void on_sendable(proton::sender& snd) override {
		proton::message msg{message_body_};
		snd.send(msg);

		std::cout << "SEND: Sent message '" << msg.body() << "'\n";

		snd.close();
		snd.connection().close();
	}
};

