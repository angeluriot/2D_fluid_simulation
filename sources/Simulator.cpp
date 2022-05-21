#include "Simulator.hpp"

void Simulator::init()
{
	Renderer::init();
	reset();
}

void Simulator::reset()
{
	// TODO
}

dim::Vector2 Simulator::screen_to_world(dim::Vector2int pos)
{
	return dim::Vector2(); // TODO
}

dim::Vector2int Simulator::world_to_screen(dim::Vector2 pos)
{
	return dim::Vector2int(); // TODO
}

void Simulator::update()
{
	static dim::Vector2int prev_mouse_pos = sf::Mouse::getPosition(dim::Window::get_window());
	dim::Vector2int mouse_pos = sf::Mouse::getPosition(dim::Window::get_window());

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !Menu::active && mouse_pos.x >= 0 && mouse_pos.x <= dim::Window::get_size().x &&
		mouse_pos.y >= 0 && mouse_pos.y <= dim::Window::get_size().y)
	{
		// TODO
	}

	prev_mouse_pos = sf::Mouse::getPosition(dim::Window::get_window());
}

void Simulator::check_events(const sf::Event& sf_event)
{
	if (sf_event.type == sf::Event::Resized)
	{
		// TODO
	}
}

void Simulator::draw()
{
	Renderer::draw();
}
