#include "server.hpp"
#include <iostream>
#include <stack>

Server::Server()
{
	listener.listen(5000);
	std::cout << "started listening TCP on port 5000\n";
	selector.add(listener);

	udp.bind(6000);
	std::cout << "binded UDP to port 6000\n";
	selector.add(udp);
}

int Server::run()
{
	while (true) {
		if (!selector.wait())
			continue;
		
		//accept a new client
		if (selector.isReady(listener)) {
			sf::TcpSocket* newTcp = new sf::TcpSocket();
			if (listener.accept(*newTcp) != sf::Socket::Done)
				delete newTcp;
			else {
				uninitialized.push_back(newTcp);
				selector.add(*newTcp);
			}
		}

		//initialize an accepted client
		std::stack<int> indexesToRemove;
		for (int i = 0; i < uninitialized.size(); i++) {
			if (!selector.isReady(*uninitialized[i]))
				continue;

			sf::Packet p;
			uninitialized[i]->receive(p);
			indexesToRemove.push(i);

			sf::Uint8 code;
			std::string version = "";
			sf::Uint16 inPort = 0;
			p >> code;
			//check if client is trying to "login"
			if (code == 1) {
				p >> version;
				p >> inPort;
			}

			//incompatible client version
			if (version != "dev" || inPort == 0) {
				p.clear();
				p << sf::Uint8(2);
				uninitialized[i]->send(p);
				selector.remove(*uninitialized[i]);
				delete uninitialized[i];
				continue;
			}

			//tell the client that it has been accepted
			p.clear();
			p << sf::Uint8(1);
			uninitialized[i]->send(p);

			Client newClient;
			newClient.port = inPort;
			newClient.tcp = uninitialized[i];
			clients.insert(std::pair<int, Client>(currentId++, newClient));
		}
		while (!indexesToRemove.empty()) {
			uninitialized.erase(uninitialized.begin() + indexesToRemove.top());
			indexesToRemove.pop();
		}

		if (selector.isReady(udp)) {
			sf::IpAddress ip;
			sf::Uint16 port;
			sf::Packet p;
			udp.receive(p, ip, port);

			//handle udp msg
		}

		std::stack<int> idsToRemove;
		for (auto& c : clients) {
			if (!selector.isReady(*c.second.tcp))
				continue;

			sf::Packet p;
			c.second.tcp->receive(p);
			sf::Uint8 code;
			p >> code;
			
			//client thinks it is not connected
			if (code == 1)
				idsToRemove.push(c.first);
			//join public lobby
			else if (code == 2) {
				
			}
		}
		while (!idsToRemove.empty()) {
			selector.remove(*clients[idsToRemove.top()].tcp);
			delete clients[idsToRemove.top()].tcp;
			clients.erase(idsToRemove.top());
			idsToRemove.pop();
		}
	}

	return 0;
}