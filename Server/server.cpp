#include "server.hpp"
#include <iostream>
#include <queue>
#include <thread>

Server::Server()
	:
	publicLobby([this] (sf::Uint32 id, Client c) {
		clients[id] = c;
		selector.add(*c.tcp);
	})
{
	listener.listen(5000);
	std::cout << "started listening TCP on port 5000\n";
	selector.add(listener);

	std::thread publicThread(&Lobby::runLobby, &publicLobby);
	publicThread.detach();
}

int Server::run()
{
	int i = 0;
	while (true) {
		if (!selector.wait(sf::seconds(1)))
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
				selector.remove(*client.tcp);
				publicLobby.joinLobby(id, client);
				clients.erase(id);
			}
		}
		while (!idsToRemove.empty()) {
			selector.remove(*clients[idsToRemove.front()].tcp);
			delete clients[idsToRemove.front()].tcp;
			clients.erase(idsToRemove.front());
			
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
			if (version != "dev5") {
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
			newClient.tcp = uninitialized[i];
			clients.insert(std::pair<sf::Uint32, Client>(++currentId, newClient));
			std::cout << "new client, id: " << currentId << ", ";
			std::cout << uninitialized[i]->getRemoteAddress() << ":" << uninitialized[i]->getRemotePort() << "\n";
		}
		while (!indexesToRemove.empty()) {
			uninitialized.erase(uninitialized.begin() + indexesToRemove.front());
			indexesToRemove.pop();
		}
	}

	return 0;
}
