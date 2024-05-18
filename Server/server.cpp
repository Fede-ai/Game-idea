#include "server.hpp"
#include <iostream>
#include <queue>
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
			sf::TcpSocket* newTcp = new sf::TcpSocket();
			if (listener.accept(*newTcp) != sf::Socket::Done)
				delete newTcp;
			else {
				uninitialized.push_back(newTcp);
				selector.add(*newTcp);
			}
		}

		//handle tcp comms
		std::queue<sf::Uint32> idsToRemove;
		for (auto& [id, client] : clients) {
			if (!selector.isReady(*client.tcp))
				continue;

			sf::Packet p;
			if (client.tcp->receive(p) == sf::Socket::Disconnected) {
				idsToRemove.push(id);
				continue;
			}
			sf::Uint8 code;
			p >> code;
			
			//client thinks it is not connected
			if (code == sf::Uint8(1))
				idsToRemove.push(id);
			//join public lobby
			else if (code == sf::Uint8(2)) {
				p.clear();
				client.pos = sf::Vector2<sf::Int64>(100, 100);
				p << sf::Uint8(3);
				client.tcp->send(p);

				p.clear();
				p << sf::Uint8(6) << sf::Int64(client.pos.x) << sf::Int64(client.pos.y);
				//tell the new client about all other previous clients
				for (const auto& otherId : publicLobby)
					p << sf::Uint32(otherId) << sf::Int64(clients[otherId].pos.x) << sf::Int64(clients[otherId].pos.y);
				client.tcp->send(p);

				//tell to all previous clients about the new client
				p.clear();
				p << sf::Uint8(4) << sf::Uint32(id) << sf::Int64(client.pos.x) << sf::Int64(client.pos.y);
				for (const auto& otherId : publicLobby)
					clients[otherId].tcp->send(p);

				publicLobby.insert(id);
				std::cout << "joined public lobby, id: " << id << "\n";
			}
			//leave lobby
			else if (code == sf::Uint8(3)) {
				if (!publicLobby.count(id))
					continue;

				publicLobby.erase(id);

				sf::Packet p;
				p << sf::Uint8(5) << sf::Uint32(id);
				for (const auto& otherId : publicLobby)
					clients[otherId].tcp->send(p);

				std::cout << "left public lobby, id: " << id << "\n";
			}
		}
		while (!idsToRemove.empty()) {
			selector.remove(*clients[idsToRemove.front()].tcp);
			delete clients[idsToRemove.front()].tcp;
			clients.erase(idsToRemove.front());
			if (publicLobby.count(idsToRemove.front())) {
				publicLobby.erase(idsToRemove.front());

				sf::Packet p;
				p << sf::Uint8(5) << sf::Uint32(idsToRemove.front());
				for (const auto& id : publicLobby)
					clients[id].tcp->send(p);
			}
			std::cout << "disconnected, id: " << idsToRemove.front() << "\n";
			idsToRemove.pop();
		}

		//initialize an accepted client
		std::queue<int> indexesToRemove;
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

			sf::Uint8 code = 0;
			std::string version = "";
			p >> code;
			//check if client is trying to "login"
			if (code == 1)
				p >> version;

			//incompatible client version
			if (version != "dev0") {
				p.clear();
				p << sf::Uint8(2);
				uninitialized[i]->send(p);
				selector.remove(*uninitialized[i]);
				delete uninitialized[i];
				continue;
			}

			//tell the client that it has been accepted
			p.clear();
			p << sf::Uint8(1) << sf::Uint32(++currentId);
			uninitialized[i]->send(p);

			Client newClient;
			newClient.tcp = uninitialized[i];
			clients.insert(std::pair<sf::Uint32, Client>(currentId, newClient));
			std::cout << "new client, id: " << currentId << "\n";
		}
		while (!indexesToRemove.empty()) {
			uninitialized.erase(uninitialized.begin() + indexesToRemove.front());
			indexesToRemove.pop();
		}
	}

	return 0;
}

void Server::handleUdp()
{
	while (true) {
		sf::IpAddress ip;
		unsigned short port;
		sf::Packet p;
		udp.receive(p, ip, port);

		sf::Uint8 code;
		p >> code;

		//update client's pos
		if (code == sf::Uint8(1)) {
			sf::Uint32 id = 0;
			for (const auto& c : publicLobby) {
				if (clients[c].port == port && clients[c].tcp->getRemoteAddress() == ip) {
					id = c;
					break;
				}
			}
			if (id == 0)
				continue;

			p >> clients[id].pos.x >> clients[id].pos.y;
			p.clear();
			p << sf::Uint8(1) << sf::Uint32(id) << clients[id].pos.x << clients[id].pos.y;
			for (const auto& otherId : publicLobby) {
				if (id == otherId)
					continue;
				udp.send(p, clients[otherId].tcp->getRemoteAddress(), clients[otherId].port);
			}
		}
		//init client port
		else if (code == sf::Uint8(2)) {
			sf::Uint32 id;
			p >> id;

			if (!clients.count(id))
				continue;

			clients[id].port = port;
			p.clear();
			p << sf::Uint8(7);
			clients[id].tcp->send(p);
		}
	}
}
