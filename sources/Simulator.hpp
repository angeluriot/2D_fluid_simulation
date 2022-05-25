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

	static float				quality;
	static dim::Color			dye_color;
	static float				time_step;
	static float				dissipation;
	static float				viscosity;
	static float				vorticity;
	static int					precision;
	static float				mouse_strength;
	static dim::VertexBuffer	screen;
	static dim::FrameBuffer		velocity;
	static dim::FrameBuffer		density;
	static dim::FrameBuffer		divergence;
	static dim::FrameBuffer		pressure;
	static dim::FrameBuffer		gradient;
	static dim::Shader			copy_shader;
	static dim::Shader			density_sources;
	static dim::Shader			density_diffusion;
	static dim::Shader			density_advection;
	static dim::Shader			velocity_forces;
	static dim::Shader			velocity_vorticity;
	static dim::Shader			velocity_diffusion;
	static dim::Shader			velocity_advection;
	static dim::Shader			velocity_divergence;
	static dim::Shader			velocity_pressure;
	static dim::Shader			velocity_gradient;
	static dim::Shader			velocity_subtraction;
	static dim::Shader			velocity_boundaries;

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

	static void copy(dim::FrameBuffer& source, dim::FrameBuffer& target);
	static void compute_density_sources();
	static void compute_density_diffusion();
	static void compute_density_advection();
	static void compute_velocity_forces();
	static void compute_velocity_vorticity();
	static void compute_velocity_diffusion();
	static void compute_velocity_advection();
	static void compute_velocity_divergence();
	static void compute_velocity_pressure();
	static void compute_velocity_gradient();
	static void compute_velocity_subtraction();
	static void compute_velocity_boundaries();

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
