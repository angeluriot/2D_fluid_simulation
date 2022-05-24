#include "Simulator.hpp"

dim::VertexBuffer Renderer::screen;
dim::Shader Renderer::density;
dim::Shader Renderer::velocity;
dim::Shader Renderer::pressure;
dim::Shader Renderer::divergence;
Renderer::Layer Renderer::layer;

void Renderer::init()
{
	density.load("shaders/all.vert", "shaders/render/density.frag");
	velocity.load("shaders/all.vert", "shaders/render/velocity.frag");
	pressure.load("shaders/all.vert", "shaders/render/pressure.frag");
	divergence.load("shaders/all.vert", "shaders/render/divergence.frag");
}

void Renderer::draw()
{
	dim::Shader shader;
	dim::Texture texture;

	switch (layer)
	{
	case Layer::Density:
		shader = density;
		texture = Simulator::density.get_texture();
		break;
	case Layer::Velocity:
		shader = velocity;
		texture = Simulator::velocity.get_texture();
		break;
	case Layer::Pressure:
		shader = pressure;
		texture = Simulator::pressure.get_texture();
		break;
	case Layer::Divergence:
		shader = divergence;
		texture = Simulator::divergence.get_texture();
		break;
	default:
		break;
	}

	screen.send_data(shader, dim::Mesh::screen, dim::DataType::Positions | dim::DataType::TexCoords);

	shader.bind();
		screen.bind();
			texture.bind();
				shader.send_uniform("u_screen", dim::Window::get_size() * Simulator::quality);
				shader.send_uniform("u_texture", texture);
				screen.draw();
			texture.unbind();
		screen.unbind();
	shader.unbind();
}
