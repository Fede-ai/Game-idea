#include "homestate.hpp"
#include <iostream> //
#include <vector>
/* 
	Todo:
	- titolo
	- new game btn
	- load btn
	- join and create session btn
	- exit btn
	- settings btn (window.close)
*/
enum actions { none, exit_program }; 

class ClickableElement {
public:
	sf::Vector2f pos;
	sf::Vector2f size;
	int action;

	ClickableElement(sf::Vector2f pos, sf::Vector2f size, int action) {
		this->pos = pos;
		this->size = size;
		this->action = action;
	}
};

void DrawTitle(sf::RenderWindow&, std::vector<ClickableElement> &elements);
void DrawBtn(sf::RenderWindow&, sf::Vector2f, sf::Vector2f, sf::String, int, std::vector<ClickableElement> & elements);
static void handleClick(int);




std::vector<ClickableElement> elements;


HomeState::HomeState(sf::RenderWindow& inWindow)
	:
	window(inWindow)
{

}

int HomeState::update(std::vector<sf::Event> events)
{
	for (const auto& e : events) {

		if (e.type == sf::Event::MouseButtonPressed && e.key.code == sf::Mouse::Left)
		{
			sf::Vector2f mousePos = sf::Vector2f(e.mouseButton.x, e.mouseButton.y);

			for (const auto& item : elements) {
				std::cout << mousePos.x << std::endl;
				std::cout << item.pos.x << std::endl;
				std::cout << item.size.x << std::endl;
				std::cout << "---------------------" << std::endl;
				
				if ( (mousePos.x > item.pos.x && mousePos.x < item.size.x) && (mousePos.y > item.pos.y && mousePos.y < item.size.y) ) {
					handleClick(item.action);
				}
			}
		}

	}
	return 0;
}

// TODOOOO: fixare salvataggio cordinate

void HomeState::draw()
{
	std::vector<ClickableElement> drawnElements;
	window.clear(sf::Color::Black);

	DrawTitle(window, drawnElements);

	DrawBtn(
		window,
		sf::Vector2f(300, 100),
		sf::Vector2f(window.getView().getCenter().x, -300),
		"New Game",
		actions::none,
		drawnElements
	);

	DrawBtn(
		window,
		sf::Vector2f(300, 100),
		sf::Vector2f(window.getView().getCenter().x,-150),
		"Load Game",
		actions::none,
		drawnElements
	);

	DrawBtn(
		window, sf::Vector2f(300, 100),
		sf::Vector2f(window.getView().getCenter().x,250),
		"Quit",
		actions::exit_program,
		drawnElements
	);


	elements = drawnElements;

	window.display();
}

void DrawTitle(sf::RenderWindow& window, std::vector<ClickableElement>& elements) {

	sf::Font font;
	sf::Text title;

	if (!font.loadFromFile("./fonts/Roboto-Regular.ttf"))
		std::cout << "ERROR: Cannot find font";

	// Setup text style
	title.setFont(font);
	title.setCharacterSize(30);
	title.setFillColor(sf::Color::White);
	title.setStyle(sf::Text::Bold); // optional but cool

	title.setString("Game Idea - Developed by OLIX and Fede-ai (who wrote about 2 lines of code)");

	float textCenter = title.getLocalBounds().width / 2;
	float textHigh = 100; //px

	float x = window.getView().getCenter().x - textCenter;
	float y = -(sf::VideoMode::getDesktopMode().height / 2.) + textHigh;//window.getView().getViewport().height perchè mi da 1??

	title.setPosition(x, y);

	window.draw(title);


	elements.push_back(
		ClickableElement(
			sf::Vector2f(x, y),
			sf::Vector2f(title.getLocalBounds().width, title.getLocalBounds().height),
			actions::none
		)
	);
}


void DrawBtn(sf::RenderWindow& window, sf::Vector2f size, sf::Vector2f pos, sf::String text, int action, std::vector<ClickableElement>& elements) {

	sf::RectangleShape button(size);
	sf::Vector2f center(size.x / 2, size.y / 2);

	button.setOrigin(center.x, center.y);
	button.setPosition(pos.x, pos.y);

	//draw text

	sf::Font font;
	sf::Text title;

	if (!font.loadFromFile("./fonts/Roboto-Regular.ttf"))
		std::cout << "ERROR: Cannot find font";

	// Setup text style
	title.setFont(font);
	title.setCharacterSize(20);
	title.setFillColor(sf::Color::Black);
	title.setString(text);

	float textCenter = title.getLocalBounds().width / 2;
	float textHigh = title.getLocalBounds().height / 2;

	float x = window.getView().getCenter().x - textCenter;
	float y = pos.y-textHigh;//window.getView().getViewport().height perchè mi da 1??

	title.setPosition(x, y);


	window.draw(button);
	window.draw(title);

	elements.push_back(
		ClickableElement(pos,size,action)
	);
}

static void handleClick(int action) {

	std::cout << "CLICK";
	switch (action)
	{
	case actions::none:
		break;

	case actions::exit_program:
		//
		break;

	default:
		break;
	}
}