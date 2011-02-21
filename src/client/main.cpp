#include <SFML/Window.hpp>

int main(int argc, char** argv)
{
	sf::Window app(sf::VideoMode(800, 600, 32), "OpenMC");

	bool run = true;
	while (run) {
		sf::Event ev;
		while (app.GetEvent(ev)) {
			if (ev.Type == sf::Event::Closed) {
				run = false;
			}
		}

		app.Display();
	}

	return 0;
}
