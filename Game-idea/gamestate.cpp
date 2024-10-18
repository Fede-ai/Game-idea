#include "gamestate.hpp"
#include <iostream>
#include <chrono>

GameState::GameState(sf::RenderWindow& inWindow, GameInfo& inGameInfo, Settings inSettings, Server& inServer, WeaponsManager& inWeaponManager)
	:
	State(inWindow),
	server(inServer),
	gameInfo(inGameInfo),
	settings(inSettings),
	weaponManager(inWeaponManager)
{
	udp.bind(sf::Socket::AnyPort);
	udp.setBlocking(false);

	grassTexture.loadFromFile("textures/grass.png");
	grassSprite.setTexture(grassTexture);
	grassSprite.setScale(CON::PIXEL_SIZE, CON::PIXEL_SIZE);

	bodyTexture.loadFromFile("textures/player.png");
	bodySprite.setTexture(bodyTexture);
	bodySprite.setScale(CON::PIXEL_SIZE, CON::PIXEL_SIZE);
	bodySprite.setOrigin(bodySprite.getLocalBounds().width / 2, bodySprite.getLocalBounds().height / 2);
	
	font.loadFromFile("fonts/PublicPixel.ttf");
	lastUpdateSent = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

GameState::~GameState()
{
	if (ingameState != NULL)
		delete ingameState;

	ingameState = NULL;
}

int GameState::update(std::vector<sf::Event> events, float dTime)
{
	if (!server.isConnected)
		std::exit(100);

	//send udp packet to server
	size_t time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
	if (time - lastUpdateSent > 50) {
		sf::Packet p;
		p << sf::Uint32(myId) << sf::Uint16(time % 65500);
		p << sf::Int16(lobbyInfo.player.pos.x % 32700) << sf::Int16(lobbyInfo.player.pos.y % 32700);
		udp.send(p, CON::SERVER_IP, lobbyInfo.serverUdpPort);
		lastUpdateSent = time;
	}

	//manage tcp packets
	while (!server.packets.empty()) {
		sf::Packet p = server.packets.front();
		sf::Uint8 code;
		p >> code;

		//a client has connected
		if (code == REC::CLIENT_CONNECTED) {
			Player other;
			sf::Uint32 otherId;
			p >> otherId >> other.pos.x >> other.pos.y;
			lobbyInfo.otherPlayers.insert(std::pair<sf::Uint32, Player>(otherId, other));
		}
		//a client has disconnected
		else if (code == REC::CLIENT_DISCONNECTED) {
			sf::Uint32 otherId;
			p >> otherId;
			if (lobbyInfo.otherPlayers.count(otherId))
				lobbyInfo.otherPlayers.erase(otherId);
		}
		//set initial players position
		else if (code == REC::JOIN_PUBLIC) {
			p >> lobbyInfo.serverUdpPort >> myId >> lobbyInfo.player.pos.x >> lobbyInfo.player.pos.y;
			
			while (!p.endOfPacket()) {
				Player other;
				sf::Uint32 otherId;
				p >> otherId >> other.pos.x >> other.pos.y;
				lobbyInfo.otherPlayers.insert(std::pair<sf::Uint32, Player>(otherId, other));
			}
		}

		server.packets.pop();
	}

	//manage udp packets
	sf::Packet p;
	sf::IpAddress ip;
	unsigned short port;
	while (udp.receive(p, ip, port) == sf::Socket::Done) {
		if (ip != CON::SERVER_IP)
			continue;
		
		sf::Uint16 timeServerMsg, numPlayers;
		p >> timeServerMsg >> numPlayers;

		//update lastUpdateReceived
		size_t receivedTime = 0;
		if (abs(long(lastUpdateReceived % 65500 - timeServerMsg)) > abs(long(lastUpdateReceived % 65500 - timeServerMsg - 65500)))
			receivedTime = sf::Int64(65500 * std::floor(lastUpdateReceived / 65500) + timeServerMsg + 65500);
		else
			receivedTime = sf::Int64(65500 * std::floor(lastUpdateReceived / 65500) + timeServerMsg);

		//ignore the late packet
		if (receivedTime <= lastUpdateReceived)
			continue;
		else
			lastUpdateReceived = receivedTime;
		
		for (unsigned int i = 0; i < numPlayers; i++) {
			sf::Uint32 otherId;
			sf::Vector2<sf::Int16> newPos;
			p >> otherId >> newPos.x >> newPos.y;

			if (!lobbyInfo.otherPlayers.count(otherId))
				continue;

			auto& pos = lobbyInfo.otherPlayers[otherId].goingTo;
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
		}
	}

	for (auto& [id, player] : lobbyInfo.otherPlayers) {
		if (player.pos == player.goingTo)
			continue;

		player.pos.x += sf::Int64((player.goingTo.x - player.pos.x) * dTime / 30.f);
		player.pos.y += sf::Int64((player.goingTo.y - player.pos.y) * dTime / 30.f);
	}

	//manage ingame states events
	if (ingameState != NULL) {
		int whatHappened = ingameState->update(events, dTime);

		if (whatHappened == 1) {
			delete ingameState;
			ingameState = NULL;
		}
		else if (whatHappened == 2) {
			delete ingameState;
			ingameState = NULL;
			return 2;
		}
		else if (whatHappened == 3) {
			delete ingameState;
			ingameState = NULL;
			// handle saving
			window.close();
			return 3;
		}
		return 0;
	}

	//handle events
	for (const auto& e : events) {
		if (e.type == sf::Event::Closed)
			window.close();
		else if (e.type == sf::Event::KeyReleased && e.key.code == sf::Keyboard::Escape) {
			if (ingameState != NULL)
				delete ingameState;
			ingameState = new PauseState(window);
		}
	}

	//manage movement
	sf::Vector2<sf::Int64> movement;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		movement.y -= sf::Int64(GameInfo::speeds[gameInfo.speed] * dTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		movement.y += sf::Int64(GameInfo::speeds[gameInfo.speed] * dTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		movement.x -= sf::Int64(GameInfo::speeds[gameInfo.speed] * dTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		movement.x += sf::Int64(GameInfo::speeds[gameInfo.speed] * dTime);
	if (movement.x != 0 && movement.y != 0) {
		movement.x = sf::Int64(double(movement.x) / sqrt(2));
		movement.y = sf::Int64(double(movement.y) / sqrt(2));
	}
	lobbyInfo.player.pos += movement;

	//move view so that the player is centered
	sf::Vector2f center = sf::Vector2f(lobbyInfo.player.pos.x / 10.f, lobbyInfo.player.pos.y / 10.f);
	window.setView(sf::View(center, sf::Vector2f(CON::VIEW_WIDTH, CON::VIEW_HEIGHT)));

	return 0;
}

void GameState::draw()
{
	window.clear(sf::Color::Black);

	const sf::Vector2f center = window.getView().getCenter();
	const sf::FloatRect chunk = grassSprite.getGlobalBounds();
	//draw chunks
	for (int x = 0; x < 2; x++) {	
		float xChunk = std::floor(center.x / chunk.width) * chunk.width;
		if (int(center.x) % int(chunk.width) > chunk.width / 2 * center.x / abs(center.x))
			xChunk += chunk.width * x;
		else 
			xChunk -= chunk.width * x;
		
		for (int y = 0; y < 2; y++) {
			float yChunk = std::floor(center.y / chunk.height) * chunk.height;
			if (int(center.y) % int(chunk.height) > chunk.height / 2 * center.y / abs(center.y))
				yChunk += chunk.height * y;
			else
				yChunk -= chunk.height * y;

			grassSprite.setPosition(xChunk, yChunk);
			window.draw(grassSprite);
		}
	}

	//draw players
	bodySprite.setPosition(lobbyInfo.player.pos.x / 10.f, lobbyInfo.player.pos.y / 10.f);
	window.draw(bodySprite);
	for (const auto& [id, c] : lobbyInfo.otherPlayers) {
		bodySprite.setPosition(c.pos.x / 10.f, c.pos.y / 10.f);
		window.draw(bodySprite);
	}

	if (ingameState != NULL)
		ingameState->draw();

	//do NOT call window.display()
}
