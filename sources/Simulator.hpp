#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "menu/Menu.hpp"
#include "renderer/Renderer.hpp"

/**
 * @brief A static class representing the simulation.
 */
class Simulator
{
public:

	/**
	 * @brief Initialize the simulation.
	 */
	static void init();

	/**
	 * @brief Reset the simulation
	 */
	static void reset();

	/**
	 * @brief Transform a screen position to a world position.
	 *
	 * @param position the screen position
	 * @return the world position
	 */
	static dim::Vector2 screen_to_world(dim::Vector2int position);

	/**
	 * @brief Transform a world position to a screen position.
	 *
	 * @param position the world position
	 * @return the screen position
	 */
	static dim::Vector2int world_to_screen(dim::Vector2 position);

	/**
	 * @brief Handle the events of the simulation.
	 */
	static void update();

	/**
	 * @brief Check the simulation events.
	 *
	 * @param sf_event the SFML event object
	 */
	static void check_events(const sf::Event& sf_event);

	/**
	 * @brief Draw the simulation.
	 */
	static void draw();
};

#endif
