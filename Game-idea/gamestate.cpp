#include "gamestate.hpp"
#include <iostream>
#include <chrono>

GameState::GameState(sf::RenderWindow& inWindow, GameInfo& inGameInfo, Settings inSettings, SocketsManager& inSocketsManager)
	:
	socketsManager(inSocketsManager),
	State(inWindow),
	gameInfo(inGameInfo),
	settings(inSettings)
{
	grassTexture.loadFromFile("textures/grass.png");
	grassSprite.setTexture(grassTexture);
	grassSprite.setScale(CON::PIXEL_SIZE, CON::PIXEL_SIZE);

	bodyTexture.loadFromFile("textures/player.png");
	bodySprite.setTexture(bodyTexture);
	bodySprite.setScale(CON::PIXEL_SIZE, CON::PIXEL_SIZE);
	bodySprite.setOrigin(bodySprite.getLocalBounds().width / 2, bodySprite.getLocalBounds().height / 2);
	
	font.loadFromFile("fonts/PublicPixel.ttf");
	lastServerUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

GameState::~GameState()
{
	if (ingameState != NULL)
		delete ingameState;

	ingameState = NULL;
}

int GameState::update(std::vector<sf::Event> events, float dTime)
{
	if (!socketsManager.isConnected())
		std::exit(100);

	size_t time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	if (time - lastServerUpdate > 70) {
		sf::Packet p;
		p << UDP::SEND::UPDATE_POS << sf::Int64(lobbyInfo.player.pos.x) << sf::Int64(lobbyInfo.player.pos.y);
		socketsManager.sendUdpPacket(p);	
		lastServerUpdate = time;
	}

	sf::Packet p;
	//manage tcp packets
	while (socketsManager.pollTcpPacket(p)) {
		sf::Uint8 code;
		p >> code;

		//a client has connected
		if (code == TCP::REC::CLIENT_CONNECTED) {
			Player other;
			sf::Uint32 otherId;
			p >> otherId >> other.pos.x >> other.pos.y;
			lobbyInfo.otherPlayers.insert(std::pair<sf::Uint32, Player>(otherId, other));
		}
		//a client has disconnected
		else if (code == TCP::REC::CLIENT_DISCONNECTED) {
			sf::Uint32 otherId;
			p >> otherId;
			if (lobbyInfo.otherPlayers.count(otherId))
				lobbyInfo.otherPlayers.erase(otherId);
		}
		//set initial players position
		else if (code == TCP::REC::SET_INITIAL_POS) {
			p >> lobbyInfo.player.pos.x >> lobbyInfo.player.pos.y;

			while (!p.endOfPacket()) {
				Player other;
				sf::Uint32 otherId;
				p >> otherId >> other.pos.x >> other.pos.y;
				lobbyInfo.otherPlayers.insert(std::pair<sf::Uint32, Player>(otherId, other));
			}
		}
	}
	//manage udp packets
	while (socketsManager.pollUdpPacket(p)) {
		sf::Uint8 code;
		p >> code;
		
		if (code == UDP::REC::UPDATE_POS) {
			sf::Uint32 otherId;
			sf::Vector2<sf::Int64> otherPos;
			p >> otherId >> otherPos.x >> otherPos.y;
		
			if (lobbyInfo.otherPlayers.count(otherId))
				lobbyInfo.otherPlayers[otherId].pos = otherPos;
		}
	}

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

	sf::Vector2<sf::Int64> movement;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		movement.y -= sf::Int64(GameInfo::speeds[gameInfo.speed] * dTime * 10);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		movement.y += sf::Int64(GameInfo::speeds[gameInfo.speed] * dTime * 10);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		movement.x -= sf::Int64(GameInfo::speeds[gameInfo.speed] * dTime * 10);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		movement.x += sf::Int64(GameInfo::speeds[gameInfo.speed] * dTime * 10);
	if (movement.x != 0 && movement.y != 0) {
		movement.x = sf::Int64(double(movement.x) / sqrt(2));
		movement.y = sf::Int64(double(movement.y) / sqrt(2));
	}
	lobbyInfo.player.pos += movement;

	sf::Vector2f center = sf::Vector2f(lobbyInfo.player.pos.x / 100.f, lobbyInfo.player.pos.y / 100.f);
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
	bodySprite.setPosition(lobbyInfo.player.pos.x / 100.f, lobbyInfo.player.pos.y / 100.f);
	window.draw(bodySprite);
	for (const auto& p : lobbyInfo.otherPlayers) {
		bodySprite.setPosition(p.second.pos.x / 100.f, p.second.pos.y / 100.f);
		window.draw(bodySprite);
	}

	if (ingameState != NULL)
		ingameState->draw();

	//do NOT call window.display()
}
