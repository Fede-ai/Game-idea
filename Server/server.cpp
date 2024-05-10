#include "server.hpp"
#include <iostream>

Server::Server()
{
	listener.listen(50000);
	std::cout << "started listening on port 50000\n";

	selector.add(listener);
}

int Server::run()
{
	while (true) {
		if (!selector.wait())
			continue;

		if (selector.isReady(listener)) {
			sf::TcpSocket* newClient = new sf::TcpSocket();
			if (listener.accept(*newClient) != sf::Socket::Done) {
				delete newClient;
			}
			else {
				sf::Packet p;
				newClient->receive(p);
				sf::Uint8 code;
				p >> code;
				std::string version = "";
				if (code == 1)
					p >> version;

				//compatible client
				if (version == "dev") {
					p.clear();
					p << sf::Uint8(1);
					newClient->send(p);
					clients.push_back(newClient);
				}
				//incompatible client
				else {
					p.clear();
					p << sf::Uint8(2);
					newClient->send(p);
					delete newClient;
				}
			}
		}
	}

	return 0;
}
