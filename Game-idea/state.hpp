#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <queue>

//struct containing the server connection info, the  
//server socket and the queue of packets received
struct Server {
	bool isConnected = false;
	bool versionCompatible = true;
	std::queue<sf::Packet> packets;
	sf::TcpSocket tcp;
};

class State {
public:
	virtual ~State() {};
	virtual int update(std::vector<sf::Event> events, float dTime) = 0;
	virtual void draw() = 0;

protected:
	State(sf::RenderWindow& inWindow) : window(inWindow) {}

	sf::RenderWindow& window;
};

class IngameState {
public:
	virtual ~IngameState() {};
	virtual int update(std::vector<sf::Event> events, float dTime) = 0;
	virtual void draw() = 0;

protected:
	IngameState(sf::RenderWindow& inWindow) : window(inWindow) {}

	sf::RenderWindow& window;
};
