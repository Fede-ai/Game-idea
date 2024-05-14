#include "server.hpp"
#include <iostream>
#include <thread>

Server::Server()
{
	listener.listen(5000);
	std::cout << "started listening on port 5000\n";
	udp.bind(6000);

	std::thread acceptClientsThread(&Server::acceptClients, this);
	acceptClientsThread.detach();
}

int Server::run()
{
	while (true) {
		if (!selector.wait())
			continue;

	}

	return 0;
}

void Server::acceptClients()
{
	while (true) {
		sf::TcpSocket* newClient = new sf::TcpSocket();
		if (listener.accept(*newClient) != sf::Socket::Done) {
			delete newClient;
			continue;
		}

		sf::Packet p;
		newClient->receive(p);
		sf::Uint8 code;
		p >> code;
		std::string version = "";
		if (code == 1)
			p >> version;

		//incompatible client
		if (version != "dev") {
			p.clear();
			p << sf::Uint8(2);
			newClient->send(p);
			delete newClient;
			continue;
		}

		p.clear();
		p << sf::Uint8(1);
		newClient->send(p);

		sf::IpAddress ip;
		unsigned short port;
		udp.receive(p, ip, port);
		sf::Uint16 remPort;
		p >> remPort;

		clients.push_back(newClient);
	}
}
