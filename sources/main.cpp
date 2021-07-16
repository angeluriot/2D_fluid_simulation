#include "Simulation.h"

// Initialisation de la fenêtre en fonction de l'écran

void init_window(sf::RenderWindow& window, std::string project_name)
{
	uint16_t width;
	uint16_t height;

	if (sf::VideoMode::getDesktopMode().width > (16. / 9.) * sf::VideoMode::getDesktopMode().height)
		height = (sf::VideoMode::getDesktopMode().height * 3) / 4, width = (height * 16) / 9;

	else if (sf::VideoMode::getDesktopMode().width < (16. / 9.) * sf::VideoMode::getDesktopMode().height)
		width = (sf::VideoMode::getDesktopMode().width * 3) / 4, height = (width * 9) / 16;

	else
		width = (sf::VideoMode::getDesktopMode().width * 3) / 4, height = (sf::VideoMode::getDesktopMode().height * 3) / 4;

	screen_width = width;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	window.create(sf::VideoMode(width, height), project_name, sf::Style::Close | sf::Style::Titlebar, settings);

	sf::Image icon;
	icon.loadFromFile("dependencies/resources/icon.png");

	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

// Initialisation du menu

void init_menu(Menu& menu, sf::RenderWindow& window)
{
	std::vector<Variable> variables;

	variables.push_back(Variable("Viscosite", Double, 0.001, 0.0000000001, 1000., 4));
	variables.push_back(Variable("Taille de la grille", Int, 128, 16, 1024));

	menu.create(variables, sf::Color(20, 20, 200));
	menu.init_names(50, 750, sf::Color::White, window, 350);
	menu.init_lines(700, 15, 1200, sf::Color(75, 75, 255));
	menu.init_circles(20, sf::Color::White);
	menu.init_values(40, 50, sf::Color::White);
	menu.init_boxes(45, 900, sf::Color::White);
	menu.init_start(150, window.getSize().x - 150, window.getSize().y - 150, sf::Color::White);
}

// Main

int main()
{
	sf::RenderWindow window;
	init_window(window, "< Nom du projet >");

	Menu menu;
	init_menu(menu, window);

	Restart restart_button = Restart(150, window.getSize().x - 150, window.getSize().y - 150, sf::Color::White);

	bool simulation_end = false, end = false;
	Cursor_type cursor_type = arrow;

	sf::Vector2i mouse_position, previous_mouse_position;
	sf::Mouse mouse;
	sf::Cursor cursor;
	sf::Event sf_event;
	sf::Clock clock;

	cursor.loadFromSystem(sf::Cursor::Arrow);
	window.setMouseCursor(cursor);

	// lancement

	Simulation simulation(menu, &window);

	while (!end)
	{
		// Lancement du menu

		while (!menu.end && !end)
		{
			event_check(sf_event, window, end);

			previous_mouse_position = mouse_position;
			mouse_position = mouse.getPosition(window);

			menu.update(previous_mouse_position, mouse_position, mouse.isButtonPressed(mouse.Left), cursor, cursor_type, window);
			menu.draw(window);
		}

		cursor.loadFromSystem(sf::Cursor::Wait);
		window.setMouseCursor(cursor);

		menu.end = false, menu.grabbed = false, menu.grab_forbiden = false;

		simulation.restart(menu, &window);

		if (restart_button.is_in(sf::Vector2f(mouse_position)))
			cursor.loadFromSystem(sf::Cursor::Hand);

		else
			cursor.loadFromSystem(sf::Cursor::Arrow);

		window.setMouseCursor(cursor);

		// Lancement de la simulation

		while (!simulation_end && !end)
		{
			event_check(sf_event, window, end);

			window.clear(sf::Color::Black);

			if (menu.variables.back().int_value != 0)
			{
				if (clock.getElapsedTime() >= sf::seconds(1. / 60.))
				{
					previous_mouse_position = mouse_position;
					mouse_position = mouse.getPosition(window);
					simulation.update(mouse_position, previous_mouse_position, mouse.isButtonPressed(mouse.Left), mouse.isButtonPressed(mouse.Right));
					clock.restart();
				}
			}

			simulation.draw();

			simulation_end = restart_button.update(mouse_position, mouse.isButtonPressed(mouse.Left), cursor, cursor_type, window);
			restart_button.draw(window);

			window.display();
		}

		simulation_end = false, restart_button.grabbed = false, restart_button.grab_forbiden = false;
	}

	return 0;
}