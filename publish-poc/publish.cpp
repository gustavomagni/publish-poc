#include "publish.h"

#include <thread>         
#include <chrono>
#include <string>
#include <ctime> 

using namespace std::chrono;

int main(int argc, char** argv) {
	std::string url("amqp://localhost:61616");
	std::string address("topic.valorEconomico");
	std::string user("admin");
	std::string password("admin");
	int desired = 7395;
	int received = 0;

	//Criado deamon para envio constante de dados simulando um motor de envio
	//A cada iteracao realizamos um thread sleep de 1 segundo para envio de um novo lote de dados
	while (true) {
		publish pub(url, address, user, password, desired, received);

		proton::container cont{ pub };

		try {
			cont.run();
		}
		catch (const std::exception & e) {
			std::cerr << e.what() << "\n";
			return 1;
		}

		//std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return 0;
}
