#include "publish.h"

#include <thread>         
#include <chrono>
#include <string>
#include <ctime> 

using namespace std::chrono;

int main(int argc, char** argv) {
	std::string url("amqp://localhost:61616");
	std::string address("topic.valorEconomico");

	while (true) {
	   
		auto start = std::chrono::system_clock::now();
		auto end = std::chrono::system_clock::now();

		std::chrono::duration<double> elapsed_seconds = end - start;
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);

		std::string messageBody(std::to_string(end_time));

		publish pub(url, address, messageBody);

		proton::container cont{ pub };

		try {
			cont.run();
		}
		catch (const std::exception & e) {
			std::cerr << e.what() << "\n";
			return 1;
		}

		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return 0;
}
