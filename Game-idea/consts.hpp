#pragma	once
#include "SFML/System.hpp"

namespace CON {
	constexpr char GAME_NAME[] = "Game-idea";
	constexpr int PIXEL_SIZE = 5;

	constexpr int VIEW_WIDTH = 1920;
	constexpr int VIEW_HEIGHT = 1080;

	constexpr int TCP_SERVER_PORT = 5000;
	constexpr int UDP_SERVER_PORT = 6000;
	constexpr char SERVER_IP[] = "IP";
}

namespace UDP {
	namespace SEND {
		constexpr sf::Uint8 UPDATE_POS = sf::Uint8(1);
	}

	namespace REC {
		constexpr sf::Uint8 UPDATE_POS = sf::Uint8(1);
	}
}

namespace TCP {
	namespace SEND {
		constexpr sf::Uint8 CONNECT = sf::Uint8(1);
		constexpr sf::Uint8 JOIN_PUBLIC = sf::Uint8(2);
		constexpr sf::Uint8 EXITED_LOBBY = sf::Uint8(3);
	}

	namespace REC {
		constexpr sf::Uint8 CONNECTED = sf::Uint8(1);
		constexpr sf::Uint8 VERSION_INCOMPATIBLE = sf::Uint8(2);
		constexpr sf::Uint8 JOIN_PUBLIC = sf::Uint8(3);
		constexpr sf::Uint8 CLIENT_CONNECTED = sf::Uint8(4);
		constexpr sf::Uint8 CLIENT_DISCONNECTED = sf::Uint8(5);
		constexpr sf::Uint8 SET_INITIAL_POS = sf::Uint8(6);
	}
}