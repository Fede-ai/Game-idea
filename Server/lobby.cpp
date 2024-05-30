#include "lobby.hpp"
#include <iostream>
#include <queue>

Lobby::Lobby(std::function<void(sf::Uint32, Client)> inLeaveLobby)
	:
	leaveLobby(inLeaveLobby)
{
	udp.bind(sf::Socket::AnyPort);
	selector.add(udp);
}

void Lobby::runLobby()
{
	std::cout << "running lobby \"" << name << "\" on udp port " << udp.getLocalPort() << "\n";
	while (true) {
		if (!selector.wait(sf::seconds(1)))
			continue;

		//handle tcp comms
		std::queue<sf::Uint32> idsToRemove;
		for (auto& [id, player] : players) {
			if (!selector.isReady(*player.tcp))
				continue;

			sf::Packet p;
			if (player.tcp->receive(p) == sf::Socket::Disconnected) {
				idsToRemove.push(id);
				continue;
			}
			sf::Uint8 code;
			p >> code;

			//leave lobby
			if (code == sf::Uint8(3)) {
				selector.remove(*player.tcp);
				leaveLobby(id, player);
				players.erase(id);

				//tell the other players that this player left
				p.clear();
				p << sf::Uint8(5) << sf::Uint32(id);
				for (const auto& [id, player] : players)
					player.tcp->send(p);

				std::cout << "left public lobby, id: " << id << "\n";
			}
		}
		while (!idsToRemove.empty()) {
			selector.remove(*players[idsToRemove.front()].tcp);
			delete players[idsToRemove.front()].tcp;
			players.erase(idsToRemove.front());

			//tell the other players that this player left
			sf::Packet p;
			p << sf::Uint8(5) << sf::Uint32(idsToRemove.front());
			for (const auto& [id, player] : players)
				player.tcp->send(p);

			std::cout << "disconnected, id: " << idsToRemove.front() << "\n";
			idsToRemove.pop();
		}

		if (selector.isReady(udp)) {
			sf::IpAddress ip;
			unsigned short port;
			sf::Packet p;
			udp.receive(p, ip, port);

			//find out who sent the msg and what he wants to do
			sf::Uint8 code;
			sf::Uint32 id = 0;
			p >> code;
			p >> id;

			//find out if player exists
			if (!players.count(id)) {
				std::cout << name << ": udp msg from un unrecognized player\n";
				continue;
			}

			players[id].port = port;

			//update client's pos
			if (code == sf::Uint8(1)) {
				auto& pos = players[id].pos;
				sf::Vector2<sf::Int16> newPos;
				p >> newPos.x >> newPos.y;
				//code needed to be able to send 4 bytes and not 16
				if (abs(pos.x % 32700 - newPos.x) > abs(pos.x % 32700 - newPos.x - 32700))
					pos.x = sf::Int64(32700 * std::floor(pos.x / 32700) + newPos.x + 32700);
				else if (abs(pos.x % 32700 - newPos.x) > abs(pos.x % 32700 - newPos.x + 32700))
					pos.x = sf::Int64(32700 * std::floor(pos.x / 32700) + newPos.x - 32700);
				else
					pos.x = sf::Int64(32700 * std::floor(pos.x / 32700) + newPos.x);
				if (abs(pos.y % 32700 - newPos.y) > abs(pos.y % 32700 - newPos.y - 32700))
					pos.y = sf::Int64(32700 * std::floor(pos.y / 32700) + newPos.y + 32700);
				else if (abs(pos.y % 32700 - newPos.y) > abs(pos.y % 32700 - newPos.y + 32700))
					pos.y = sf::Int64(32700 * std::floor(pos.y / 32700) + newPos.y - 32700);
				else
					pos.y = sf::Int64(32700 * std::floor(pos.y / 32700) + newPos.y);

				//send this packet to all other clients
				p.clear();
				p << sf::Uint8(1) << sf::Uint32(id) << sf::Int16(pos.x % 32700) << sf::Int16(pos.y % 32700);
				for (const auto& [otherId, otherPlayer] : players) {
					if (id == otherId)
						continue;
					udp.send(p, otherPlayer.tcp->getRemoteAddress(), otherPlayer.port);
				}
			}
		}
	}
}

void Lobby::joinLobby(sf::Uint32 id, Client newClient)
{
	sf::Packet p;
	p << sf::Uint8(3);
	newClient.tcp->send(p);

	//decide the spawn position
	newClient.pos = sf::Vector2<sf::Int64>(100, 100);

	//tell the new client about all other previous clients
	p.clear();
	p << sf::Uint8(6) << sf::Uint16(udp.getLocalPort()) << sf::Uint32(id) << sf::Int64(newClient.pos.x) << sf::Int64(newClient.pos.y);
	for (const auto& [otherId, player] : players)
		p << sf::Uint32(otherId) << sf::Int64(player.pos.x) << sf::Int64(player.pos.y);
	newClient.tcp->send(p);

	//tell all previous clients about the new client
	p.clear();
	p << sf::Uint8(4) << sf::Uint32(id) << sf::Int64(newClient.pos.x) << sf::Int64(newClient.pos.y);
	for (const auto& [otherId, player] : players)
		player.tcp->send(p);

	//add the new client to the list of clients 
	selector.add(*newClient.tcp);
	players[id] = newClient;
	std::cout << "joined public lobby, id: " << id << "\n";
}
