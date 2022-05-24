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

	enum class Layer
	{
		Density = 0,
		Velocity = 1,
		Pressure = 2,
		Divergence = 3
	};

	static dim::VertexBuffer	screen;		// The screen buffer.
	static dim::Shader			density;
	static dim::Shader			velocity;
	static dim::Shader			pressure;
	static dim::Shader			divergence;
	static Layer				layer;

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
