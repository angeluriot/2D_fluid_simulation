#include "Simulator.hpp"

dim::VertexBuffer Renderer::screen;

void Renderer::init()
{
	dim::Shader::add("compute", "shaders/compute.vert", "shaders/compute.frag");
	screen.send_data("compute", dim::Mesh::screen, dim::DataType::Positions | dim::DataType::TexCoords);
}

void Renderer::draw()
{
	dim::Shader::get("compute").bind();
		screen.bind();
			
				// TODO
				screen.draw();
			
		screen.unbind();
	dim::Shader::get("compute").unbind();
}
