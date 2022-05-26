#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "libraries.hpp"
#include "menu/Menu.hpp"

/**
 * @brief Takes care of the simulation display.
 */
class Renderer
{
public:

	/**
	 * @brief An enum that represents the different shown layers.
	 */
	enum class Layer
	{
		Density = 0,	// The density of the dye
		Velocity = 1,	// The velocity of the fluid
		Pressure = 2,	// The pressure of the fluid
		Divergence = 3	// The divergence of the fluid
	};

	static dim::VertexBuffer	screen;		// The screen buffer.
	static dim::Shader			density;	// The density shader.
	static dim::Shader			velocity;	// The velocity shader.
	static dim::Shader			pressure;	// The pressure shader.
	static dim::Shader			divergence;	// The divergence shader.
	static Layer				layer;		// The current shown layer.

	/**
	 * @brief Initialize the renderer.
	 */
	static void init();

	/**
	 * @brief Draw the simulation.
	 */
	static void draw();
};

#endif
