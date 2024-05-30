#include "statesmanager.hpp"
#include <chrono>
#include <thread>

StatesManager::StatesManager()
{
	sf::Image cursorImage, scaledCursor;
	cursorImage.loadFromFile("textures/cursor.png");
	scaledCursor.create(cursorImage.getSize().x * CON::PIXEL_SIZE, cursorImage.getSize().y * CON::PIXEL_SIZE);
	for (unsigned int x = 0; x < cursorImage.getSize().x; x++) {
		for (unsigned int y = 0; y < cursorImage.getSize().y; y++) {
			for (int a = 0; a < CON::PIXEL_SIZE; a++) {
				for (int b = 0; b < CON::PIXEL_SIZE; b++) {
					scaledCursor.setPixel(x * CON::PIXEL_SIZE + a, y * CON::PIXEL_SIZE + b, cursorImage.getPixel(x, y));
				}
			}
		}
	}
	cursor.loadFromPixels(scaledCursor.getPixelsPtr(), scaledCursor.getSize(), sf::Vector2u(0, 0));

	std::thread tcpThread(&StatesManager::handleTcp, this);
	tcpThread.detach();
}

StatesManager::~StatesManager()
{
	if (state != NULL)
		delete state;

	state = NULL;
}

int StatesManager::run()
{
	sf::ContextSettings openGLSettings;
	openGLSettings.antialiasingLevel = 8;
	auto width = sf::VideoMode::getDesktopMode().width;
	sf::RenderWindow window(sf::VideoMode(int(width * 2 / 3.f), int(width * 3 / 8.f)), CON::GAME_NAME, sf::Style::Default, openGLSettings);
	window.setView(sf::View(sf::Vector2f(0, 0), sf::Vector2f(1920, 1080)));
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);
	window.setMouseCursor(cursor);

	state = new HomeState(window, gameInfo, server);
	auto lastTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	while (window.isOpen()) {
		sf::Event e;
		std::vector<sf::Event> events;
		while (window.pollEvent(e)) {
			//toggle fullscreen
			if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter && e.key.alt) {
				sf::View view = window.getView();
				if (window.getSize().x == sf::VideoMode::getFullscreenModes()[0].width)
					window.create(sf::VideoMode(int(width * 2 / 3.f), int(width * 3 / 8.f)), CON::GAME_NAME, sf::Style::Default, openGLSettings);
				else
					window.create(sf::VideoMode::getDesktopMode(), CON::GAME_NAME, sf::Style::Fullscreen, openGLSettings);
				window.setView(view);
				window.setVerticalSyncEnabled(true);
				window.setKeyRepeatEnabled(false);
				window.setMouseCursor(cursor);
			}

			events.push_back(e);
		}

		auto time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		int whatHappened = 0;
		if (!isTransitioning)
			whatHappened = state->update(events, int(std::max(time - lastTime, long long(1))) / 1000.f);
		lastTime = time;

		//draw if nothing happened
		if (whatHappened == 0) {
			state->draw();
			window.display();
		}
		//from home-state go game-state
		else if (whatHappened == 1) {
			delete state;
			state = new GameState(window, gameInfo, settings, server);
		}
		//from game-state (pause-state) to home-state
		else if (whatHappened == 2) {
			sf::Packet p;
			p << TCP::SEND::EXITED_LOBBY;
			server.tcp.send(p);

			delete state;
			state = new HomeState(window, gameInfo, server);
		}
		//from home-state to shop
		else if (whatHappened == 3) {
			delete state;
			state = new ShopState(window);
		}
	}

	return 0;
}

void StatesManager::handleTcp()
{
	sf::Packet p;
	sf::Uint8 res = 0;
	while (res != TCP::REC::CONNECTED && res != TCP::REC::VERSION_INCOMPATIBLE) {
		while (server.tcp.connect(CON::SERVER_IP, CON::TCP_SERVER_PORT, sf::seconds(10)) != sf::Socket::Done)
			sf::sleep(sf::seconds(1));

		p.clear();
		p << TCP::SEND::CONNECT << std::string("dev5");
		server.tcp.send(p);

		p.clear();
		if (server.tcp.receive(p) == sf::Socket::Done)
			p >> res;
	}

	if (res == TCP::REC::CONNECTED)
		server.isConnected = true;
	else if (res == TCP::REC::VERSION_INCOMPATIBLE) {
		server.versionCompatible = false;
		return;
	}

	while (true) {
		sf::Packet p;
		sf::Socket::Status status = server.tcp.receive(p);
		if (status == sf::Socket::Done)
			server.packets.push(p);
		else if (status == sf::Socket::Disconnected)
			server.isConnected = false;
	}
}
