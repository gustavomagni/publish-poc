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
#include <string>
#include <ctime> 

#include "fake_cpp11.h"

using proton::sender_options;
using proton::delivery_mode;
using proton::target_options;

#pragma once
class publish : public proton::messaging_handler {

private:
	std::string conn_url_{};
	std::string address_{};
	std::string user_{};
	std::string password_{};
	int desired_ = 7395;
	int received_ = 0;

public:
	publish(const std::string& url, const std::string& address, const std::string& user, const std::string& password, const int& desired, const int& received) :
		conn_url_(url),
		address_(address),
		user_(user),
		password_(password),
		desired_(desired),
		received_(received)
	{}


	void on_container_start(proton::container& cont) override {
		proton::connection_options co;
		co.user(user_);
		co.password(password_);
		co.sasl_enabled(true);
		co.sasl_allow_insecure_mechs(true);

		cont.connect(conn_url_, co);
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
		proton::message msg{ getMessage() };
		snd.send(msg);

		std::cout << "SEND: Sent message '" << msg.body() << "'\n";

		received_++;

		if (received_ == desired_) {
			snd.close();
			snd.connection().close();

			std::cout << "SEND CLOSED...";
		}
	}

	std::string getMessage() {
		auto start = std::chrono::system_clock::now();
		auto end = std::chrono::system_clock::now();

		std::chrono::duration<double> elapsed_seconds = end - start;
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);

		return std::to_string(end_time);
	}
};

