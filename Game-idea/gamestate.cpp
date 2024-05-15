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
	grassSprite.setScale(Consts::PIXEL_SIZE, Consts::PIXEL_SIZE);

	bodyTexture.loadFromFile("textures/player.png");
	bodySprite.setTexture(bodyTexture);
	bodySprite.setScale(Consts::PIXEL_SIZE, Consts::PIXEL_SIZE);
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
		p << sf::Uint8(1) << sf::Int16(gameInfo.player.pos.x) << sf::Int16(gameInfo.player.pos.y);
		socketsManager.sendUdpPacket(p);	
		lastServerUpdate = time;
	}

	sf::Packet p;
	while (socketsManager.pollTcpPacket(p)) {
		sf::Uint8 code;
		p >> code;
		//a client has connected
		if (code == sf::Uint8(4)) {
			Player other;
			sf::Int32 otherId;
			sf::Vector2<sf::Int16> otherPos;
			p >> otherId >> otherPos.x >> otherPos.y;
			other.pos = sf::Vector2f(otherPos);
			gameInfo.otherPlayers.insert(std::pair<int, Player>(otherId, other));
		}
		//a client has disconnected
		else if (code == sf::Uint8(5)) {
			sf::Int32 otherId;
			p >> otherId;
			if (gameInfo.otherPlayers.count(otherId))
				gameInfo.otherPlayers.erase(otherId);
		}
	}

	while (socketsManager.pollUdpPacket(p)) {
		sf::Uint8 code;
		p >> code;

		if (code == sf::Uint8(1)) {
			sf::Int32 otherId;
			sf::Vector2<sf::Int16> otherPos;
			p >> otherId >> otherPos.x >> otherPos.y;
		
			if (gameInfo.otherPlayers.count(otherId)) {
				gameInfo.otherPlayers[otherId].pos = sf::Vector2f(otherPos);
			}
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

	sf::Vector2f movement;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		movement.y -= GameInfo::speeds[gameInfo.speed] * dTime / 10.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		movement.y += GameInfo::speeds[gameInfo.speed] * dTime / 10.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		movement.x -= GameInfo::speeds[gameInfo.speed] * dTime / 10.f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		movement.x += GameInfo::speeds[gameInfo.speed] * dTime / 10.f;
	if (movement.x != 0 && movement.y != 0) {
		movement.x /= sqrt(2);
		movement.y /= sqrt(2);
	}
	gameInfo.player.pos += movement;

	window.setView(sf::View(sf::Vector2f(gameInfo.player.pos), sf::Vector2f(Consts::VIEW_SIZE_X, Consts::VIEW_SIZE_Y)));

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
	bodySprite.setPosition(sf::Vector2f(gameInfo.player.pos));
	window.draw(bodySprite);
	for (const auto& p : gameInfo.otherPlayers) {
		bodySprite.setPosition(sf::Vector2f(p.second.pos));
		window.draw(bodySprite);
	}

	if (ingameState != NULL)
		ingameState->draw();

	//do NOT call window.display()
}
