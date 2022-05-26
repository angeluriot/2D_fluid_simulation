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

	static float				resolution;				// The resolution of the simulation.
	static dim::Color			dye_color;				// The color of the dye.
	static float				time_step;				// The time step of the simulation.
	static float				dissipation;			// The dissipation rate of the dye.
	static float				vorticity;				// The vorticity of the fluid.
	static int					iterations;				// The number of iterations of the computation.
	static float				mouse_strength;			// The strength of the mouse interactions.
	static dim::VertexBuffer	screen;					// The screen vertex buffer.
	static dim::FrameBuffer		velocity;				// The velocity of the fluid.
	static dim::FrameBuffer		density;				// The density of the dye.
	static dim::FrameBuffer		divergence;				// The divergence of the fluid.
	static dim::FrameBuffer		pressure;				// The pressure of the fluid.
	static dim::FrameBuffer		gradient;				// The gradient of the pressure.
	static dim::Shader			copy_shader;			// A shader to copy a texture.
	static dim::Shader			density_sources;		// A shader to compute the dye sources.
	static dim::Shader			density_advection;		// A shader to compute the dye advection.
	static dim::Shader			velocity_forces;		// A shader to compute the fluid forces.
	static dim::Shader			velocity_vorticity;		// A shader to compute the fluid vorticity.
	static dim::Shader			velocity_advection;		// A shader to compute the fluid advection.
	static dim::Shader			velocity_divergence;	// A shader to compute the fluid divergence.
	static dim::Shader			velocity_pressure;		// A shader to compute the fluid pressure.
	static dim::Shader			velocity_gradient;		// A shader to compute the pressure gradient.
	static dim::Shader			velocity_subtraction;	// A shader to subtract the gradient to the velocity.
	static dim::Shader			velocity_boundaries;	// A shader to apply the fluid boundaries.

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
	 * @brief Copy a texture with a shader.
	 *
	 * @param source the source texture
	 * @param target the target texture
	 */
	static void copy(dim::FrameBuffer& source, dim::FrameBuffer& target);

	/**
	 * @brief Compute the dye sources.
	 */
	static void compute_density_sources();

	/**
	 * @brief Compute the advection of the dye.
	 */
	static void compute_density_advection();

	/**
	 * @brief Compute the fluid forces.
	 */
	static void compute_velocity_forces();

	/**
	 * @brief Compute the fluid vorticity.
	 */
	static void compute_velocity_vorticity();

	/**
	 * @brief Compute the fluid advection.
	 */
	static void compute_velocity_advection();

	/**
	 * @brief Compute the fluid divergence.
	 */
	static void compute_velocity_divergence();

	/**
	 * @brief Compute the fluid pressure.
	 */
	static void compute_velocity_pressure();

	/**
	 * @brief Compute the pressure gradient.
	 */
	static void compute_velocity_gradient();

	/**
	 * @brief Subtract the gradient to the velocity.
	 */
	static void compute_velocity_subtraction();

	/**
	 * @brief Apply the fluid boundaries.
	 */
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
