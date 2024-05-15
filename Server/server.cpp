#include "server.hpp"
#include <iostream>
#include <stack>
#include <thread>

Server::Server()
{
	listener.listen(5000);
	std::cout << "started listening TCP on port 5000\n";
	selector.add(listener);

	udp.bind(6000);
	std::cout << "binded UDP to port 6000\n";
	std::thread udpThread(&Server::handleUdp, this);
	udpThread.detach();
}

int Server::run()
{
	int i = 0;
	while (true) {
		if (!selector.wait())
			continue;

		//accept a new client
		if (selector.isReady(listener)) {
			std::cout << "INCOMING\n";

			sf::TcpSocket* newTcp = new sf::TcpSocket();
			if (listener.accept(*newTcp) != sf::Socket::Done)
				delete newTcp;
			else {
				uninitialized.push_back(newTcp);
				selector.add(*newTcp);
			}
		}

		//handle tcp comms
		std::stack<int> idsToRemove;
		for (auto& c : clients) {
			if (!selector.isReady(*c.second.tcp))
				continue;

			sf::Packet p;
			if (c.second.tcp->receive(p) == sf::Socket::Disconnected) {
				idsToRemove.push(c.first);
				continue;
			}
			sf::Uint8 code;
			p >> code;
			
			//client thinks it is not connected
			if (code == sf::Uint8(1))
				idsToRemove.push(c.first);
			//join public lobby
			else if (code == sf::Uint8(2)) {
				p.clear();
				c.second.pos = sf::Vector2f(100, 100);
				p << sf::Uint8(3) << sf::Int16(c.second.pos.x) << sf::Int16(c.second.pos.y);

				//tell the new client about all other previous clients
				for (const auto& id : publicLobby)
					p << sf::Int32(id) << sf::Int16(clients[id].pos.x) << sf::Int16(clients[id].pos.y);
				c.second.tcp->send(p);

				//tell to all previous clients about the new client
				p.clear();
				p << sf::Uint8(4) << sf::Int32(c.first) << sf::Int16(c.second.pos.x) << sf::Int16(c.second.pos.y);
				for (const auto& id : publicLobby)
					clients[id].tcp->send(p);

				publicLobby.insert(c.first);
			}
		}
		while (!idsToRemove.empty()) {
			selector.remove(*clients[idsToRemove.top()].tcp);
			delete clients[idsToRemove.top()].tcp;
			clients.erase(idsToRemove.top());
			if (publicLobby.count(idsToRemove.top())) {
				publicLobby.erase(idsToRemove.top());

				sf::Packet p;
				p << sf::Uint8(5) << sf::Int32(idsToRemove.top());
				for (const auto& id : publicLobby)
					clients[id].tcp->send(p);
			}
			std::cout << "disconnected, id: " << idsToRemove.top() << "\n";
			idsToRemove.pop();
		}

		//initialize an accepted client
		std::stack<int> indexesToRemove;
		for (int i = 0; i < uninitialized.size(); i++) {
			if (!selector.isReady(*uninitialized[i]))
				continue;
			indexesToRemove.push(i);

			sf::Packet p;
			if (uninitialized[i]->receive(p) == sf::Socket::Disconnected) {
				selector.remove(*uninitialized[i]);
				delete uninitialized[i];
				continue;
			}

			sf::Uint8 code;
			std::string version = "";
			p >> code;
			//check if client is trying to "login"
			if (code == 1)
				p >> version;

			//incompatible client version
			if (version != "dev") {
				p.clear();
				p << sf::Uint8(2);
				uninitialized[i]->send(p);
				selector.remove(*uninitialized[i]);
				delete uninitialized[i];
				continue;
			}

			//tell the client that it has been accepted
			p.clear();
			p << sf::Uint8(1) << sf::Int32(currentId);
			uninitialized[i]->send(p);

			Client newClient;
			newClient.ip = uninitialized[i]->getRemoteAddress();
			newClient.tcp = uninitialized[i];
			clients.insert(std::pair<int, Client>(currentId++, newClient));
			std::cout << "new client, id: " << currentId - 1 << "\n";
		}
		while (!indexesToRemove.empty()) {
			uninitialized.erase(uninitialized.begin() + indexesToRemove.top());
			indexesToRemove.pop();
		}
	}

	return 0;
}

void Server::handleUdp()
{
	while (true) {
		sf::IpAddress ip;
		sf::Uint16 port;
		sf::Packet p;
		udp.receive(p, ip, port);

		sf::Int32 id;
		p >> id;

		if (!publicLobby.count(id))
			continue;

		clients[id].port = port;
		clients[id].ip = ip;

		sf::Uint8 code;
		p >> code;
		//update client's pos
		if (code == sf::Uint8(1)) {
			sf::Vector2<sf::Int16> newPos;
			p >> newPos.x >> newPos.y;
			clients[id].pos = sf::Vector2f(newPos);
			
			p.clear();
			p << sf::Uint8(1) << sf::Int32(id) << sf::Int16(newPos.x) << sf::Int16(newPos.y);
			for (const auto& otherId : publicLobby) {
				if (id == otherId)
					continue;
				udp.send(p, clients[otherId].ip, clients[otherId].port);
			}
		}
	}
}
