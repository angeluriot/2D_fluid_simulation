#include "Simulator.hpp"

float Simulator::quality = 0.2f;
dim::Color Simulator::dye_color = dim::Color(0.1f, 0.5f, 1.f);
float Simulator::time_step = 0.3f;
float Simulator::dissipation = 0.005f;
float Simulator::viscosity = 0.f;
float Simulator::vorticity = 0.2f;
int Simulator::precision = 20;
float Simulator::mouse_strength = 70.f;
dim::VertexBuffer Simulator::screen;
dim::FrameBuffer Simulator::velocity;
dim::FrameBuffer Simulator::density;
dim::FrameBuffer Simulator::divergence;
dim::FrameBuffer Simulator::pressure;
dim::FrameBuffer Simulator::gradient;
dim::Shader	Simulator::copy_shader;
dim::Shader	Simulator::density_sources;
dim::Shader	Simulator::density_diffusion;
dim::Shader	Simulator::density_advection;
dim::Shader	Simulator::velocity_forces;
dim::Shader	Simulator::velocity_vorticity;
dim::Shader	Simulator::velocity_diffusion;
dim::Shader	Simulator::velocity_advection;
dim::Shader	Simulator::velocity_divergence;
dim::Shader	Simulator::velocity_pressure;
dim::Shader	Simulator::velocity_gradient;
dim::Shader	Simulator::velocity_subtraction;
dim::Shader	Simulator::velocity_boundaries;

void Simulator::init()
{
	copy_shader.load("shaders/all.vert", "shaders/copy.frag");
	density_sources.load("shaders/all.vert", "shaders/density/sources.frag");
	density_diffusion.load("shaders/all.vert", "shaders/density/diffusion.frag");
	density_advection.load("shaders/all.vert", "shaders/density/advection.frag");
	velocity_forces.load("shaders/all.vert", "shaders/velocity/forces.frag");
	velocity_vorticity.load("shaders/all.vert", "shaders/velocity/vorticity.frag");
	velocity_diffusion.load("shaders/all.vert", "shaders/velocity/diffusion.frag");
	velocity_advection.load("shaders/all.vert", "shaders/velocity/advection.frag");
	velocity_divergence.load("shaders/all.vert", "shaders/velocity/divergence.frag");
	velocity_pressure.load("shaders/all.vert", "shaders/velocity/pressure.frag");
	velocity_gradient.load("shaders/all.vert", "shaders/velocity/gradient.frag");
	velocity_subtraction.load("shaders/all.vert", "shaders/velocity/subtraction.frag");
	velocity_boundaries.load("shaders/all.vert", "shaders/velocity/boundaries.frag");
	Renderer::init();
	reset();
}

void Simulator::reset()
{
	velocity.create(dim::Window::get_size() * quality, dim::Texture::Filtering::Linear, dim::Texture::Warpping::ClampToEdge, dim::Texture::Type::RGB_32f);
	density.create(dim::Window::get_size(), dim::Texture::Filtering::Linear, dim::Texture::Warpping::ClampToEdge, dim::Texture::Type::RGB_32f);
	divergence.create(dim::Window::get_size() * quality, dim::Texture::Filtering::Linear, dim::Texture::Warpping::ClampToEdge, dim::Texture::Type::RGB_32f);
	pressure.create(dim::Window::get_size() * quality, dim::Texture::Filtering::Linear, dim::Texture::Warpping::ClampToEdge, dim::Texture::Type::RGB_32f);
	gradient.create(dim::Window::get_size() * quality, dim::Texture::Filtering::Linear, dim::Texture::Warpping::ClampToEdge, dim::Texture::Type::RGB_32f);
}

dim::Vector2 Simulator::screen_to_world(dim::Vector2int pos)
{
	return dim::Vector2((float)pos.x / (float)dim::Window::get_size().x, 1.f - (float)pos.y / (float)dim::Window::get_size().y);
}

dim::Vector2int Simulator::world_to_screen(dim::Vector2 pos)
{
	return dim::Vector2int(pos.x * dim::Window::get_size().x, (1.f - pos.y) * dim::Window::get_size().y);
}

void Simulator::copy(dim::FrameBuffer& source, dim::FrameBuffer& target)
{
	copy_shader.bind();
		target.bind();
			screen.bind();
				source.get_texture().bind();
					copy_shader.send_uniform("u_texture", source.get_texture());
					screen.draw();
				source.get_texture().unbind();
			screen.unbind();
		target.unbind();
	copy_shader.unbind();
}

void Simulator::compute_density_sources()
{
	dim::Vector2int mouse_pos = sf::Mouse::getPosition(dim::Window::get_window());
	bool clicking = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	if (clicking && !Menu::active && mouse_pos.x >= 0 && mouse_pos.x <= dim::Window::get_size().x && mouse_pos.y >= 0 && mouse_pos.y <= dim::Window::get_size().y)
	{
		screen.send_data(density_sources, dim::Mesh::screen, dim::DataType::Positions | dim::DataType::TexCoords);
		dim::FrameBuffer density_temp(dim::Window::get_size(), dim::Texture::Filtering::Linear, dim::Texture::Warpping::ClampToEdge, dim::Texture::Type::RGB_32f);

		density_sources.bind();
			density_temp.bind();
				screen.bind();
					density.get_texture().bind();
						density_sources.send_uniform("u_screen", dim::Window::get_size() * quality);
						density_sources.send_uniform("u_mouse", screen_to_world(mouse_pos));
						density_sources.send_uniform("u_strength", mouse_strength * quality);
						density_sources.send_uniform("u_color", dye_color);
						density_sources.send_uniform("u_density", density.get_texture());
						screen.draw();
					density.get_texture().unbind();
				screen.unbind();
			density_temp.unbind();
		density_sources.unbind();

		std::swap(density, density_temp);
	}
}

void Simulator::compute_density_diffusion()
{
	screen.send_data(density_diffusion, dim::Mesh::screen, dim::DataType::Positions | dim::DataType::TexCoords);
	dim::FrameBuffer density_temp_1(dim::Window::get_size(), dim::Texture::Filtering::Linear, dim::Texture::Warpping::ClampToEdge, dim::Texture::Type::RGB_32f);
	dim::FrameBuffer density_temp_2(dim::Window::get_size(), dim::Texture::Filtering::Linear, dim::Texture::Warpping::ClampToEdge, dim::Texture::Type::RGB_32f);
	copy(density, density_temp_1);

	density_diffusion.bind();
		screen.bind();
			density.get_texture().bind();

				density_diffusion.send_uniform("u_screen", dim::Window::get_size() * quality);
				density_diffusion.send_uniform("u_time_step", time_step);
				density_diffusion.send_uniform("u_viscosity", viscosity);
				density_diffusion.send_uniform("u_density", density.get_texture());

				for (int i = 0; i < precision; i++)
				{
					density_temp_2.bind();
						density_temp_1.get_texture().bind();
							density_diffusion.send_uniform("u_density_temp", density_temp_1.get_texture());
							screen.draw();
						density_temp_1.get_texture().unbind();
					density_temp_2.unbind();

					std::swap(density_temp_1, density_temp_2);
					density_temp_2.bind();
						density_temp_2.clear();
					density_temp_2.unbind();
				}

			density.get_texture().unbind();
		screen.unbind();
	density_diffusion.unbind();

	std::swap(density, density_temp_1);
}

void Simulator::compute_density_advection()
{
	screen.send_data(density_advection, dim::Mesh::screen, dim::DataType::Positions | dim::DataType::TexCoords);
	dim::FrameBuffer density_temp(dim::Window::get_size(), dim::Texture::Filtering::Linear, dim::Texture::Warpping::ClampToEdge, dim::Texture::Type::RGB_32f);

	density_advection.bind();
		density_temp.bind();
			screen.bind();
				density.get_texture().bind();
				velocity.get_texture().bind();
					density_advection.send_uniform("u_screen", dim::Window::get_size() * quality);
					density_advection.send_uniform("u_time_step", time_step);
					density_advection.send_uniform("u_dissipation", dissipation);
					density_diffusion.send_uniform("u_density", density.get_texture());
					density_advection.send_uniform("u_velocity", velocity.get_texture());
					screen.draw();
				velocity.get_texture().unbind();
				density.get_texture().unbind();
			screen.unbind();
		density_temp.unbind();
	density_advection.unbind();

	std::swap(density, density_temp);
}

void Simulator::compute_velocity_forces()
{
	static dim::Vector2int prev_mouse_pos = sf::Mouse::getPosition(dim::Window::get_window());
	dim::Vector2int mouse_pos = sf::Mouse::getPosition(dim::Window::get_window());
	bool clicking = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	if (clicking && !Menu::active && mouse_pos.x >= 0 && mouse_pos.x <= dim::Window::get_size().x && mouse_pos.y >= 0 && mouse_pos.y <= dim::Window::get_size().y)
	{
		screen.send_data(velocity_forces, dim::Mesh::screen, dim::DataType::Positions | dim::DataType::TexCoords);
		dim::FrameBuffer velocity_temp(dim::Window::get_size() * quality, dim::Texture::Filtering::Linear, dim::Texture::Warpping::ClampToEdge, dim::Texture::Type::RGB_32f);

		velocity_forces.bind();
			velocity_temp.bind();
				screen.bind();
					velocity.get_texture().bind();
						velocity_forces.send_uniform("u_screen", dim::Window::get_size() * quality);
						velocity_forces.send_uniform("u_mouse", screen_to_world(mouse_pos));
						velocity_forces.send_uniform("u_prev_mouse", screen_to_world(prev_mouse_pos));
						velocity_forces.send_uniform("u_strength", mouse_strength * quality);
						velocity_forces.send_uniform("u_velocity", velocity.get_texture());
						screen.draw();
					velocity.get_texture().unbind();
				screen.unbind();
			velocity_temp.unbind();
		velocity_forces.unbind();

		std::swap(velocity, velocity_temp);
	}

	prev_mouse_pos = mouse_pos;
}

void Simulator::compute_velocity_vorticity()
{
	screen.send_data(velocity_vorticity, dim::Mesh::screen, dim::DataType::Positions | dim::DataType::TexCoords);
	dim::FrameBuffer velocity_temp(dim::Window::get_size() * quality, dim::Texture::Filtering::Linear, dim::Texture::Warpping::ClampToEdge, dim::Texture::Type::RGB_32f);

	velocity_vorticity.bind();
		velocity_temp.bind();
			screen.bind();
				velocity.get_texture().bind();
					velocity_vorticity.send_uniform("u_screen", dim::Window::get_size() * quality);
					velocity_vorticity.send_uniform("u_time_step", time_step);
					velocity_vorticity.send_uniform("u_vorticity", vorticity);
					velocity_vorticity.send_uniform("u_velocity", velocity.get_texture());
					screen.draw();
				velocity.get_texture().unbind();
			screen.unbind();
		velocity_temp.unbind();
	velocity_vorticity.unbind();

	std::swap(velocity, velocity_temp);
}

void Simulator::compute_velocity_diffusion()
{
	screen.send_data(velocity_diffusion, dim::Mesh::screen, dim::DataType::Positions | dim::DataType::TexCoords);
	dim::FrameBuffer velocity_temp_1(dim::Window::get_size() * quality, dim::Texture::Filtering::Linear, dim::Texture::Warpping::ClampToEdge, dim::Texture::Type::RGB_32f);
	dim::FrameBuffer velocity_temp_2(dim::Window::get_size() * quality, dim::Texture::Filtering::Linear, dim::Texture::Warpping::ClampToEdge, dim::Texture::Type::RGB_32f);
	copy(velocity, velocity_temp_1);

	velocity_diffusion.bind();
		screen.bind();
			velocity.get_texture().bind();

				velocity_diffusion.send_uniform("u_screen", dim::Window::get_size() * quality);
				velocity_diffusion.send_uniform("u_time_step", time_step);
				velocity_diffusion.send_uniform("u_viscosity", viscosity);
				velocity_diffusion.send_uniform("u_velocity", velocity.get_texture());

				for (int i = 0; i < precision; i++)
				{
					velocity_temp_2.bind();
						velocity_temp_1.get_texture().bind();
							velocity_diffusion.send_uniform("u_velocity_temp", velocity_temp_1.get_texture());
							screen.draw();
						velocity_temp_1.get_texture().unbind();
					velocity_temp_2.unbind();

					std::swap(velocity_temp_1, velocity_temp_2);
					velocity_temp_2.bind();
						velocity_temp_2.clear();
					velocity_temp_2.unbind();
				}

			velocity.get_texture().unbind();
		screen.unbind();
	velocity_diffusion.unbind();

	std::swap(velocity, velocity_temp_1);
}

void Simulator::compute_velocity_advection()
{
	screen.send_data(velocity_advection, dim::Mesh::screen, dim::DataType::Positions | dim::DataType::TexCoords);
	dim::FrameBuffer velocity_temp(dim::Window::get_size() * quality, dim::Texture::Filtering::Linear, dim::Texture::Warpping::ClampToEdge, dim::Texture::Type::RGB_32f);

	velocity_advection.bind();
		velocity_temp.bind();
			screen.bind();
				velocity.get_texture().bind();
					velocity_advection.send_uniform("u_screen", dim::Window::get_size() * quality);
					velocity_advection.send_uniform("u_time_step", time_step);
					velocity_advection.send_uniform("u_velocity", velocity.get_texture());
					screen.draw();
				velocity.get_texture().unbind();
			screen.unbind();
		velocity_temp.unbind();
	velocity_advection.unbind();

	std::swap(velocity, velocity_temp);
}

void Simulator::compute_velocity_divergence()
{
	screen.send_data(velocity_divergence, dim::Mesh::screen, dim::DataType::Positions | dim::DataType::TexCoords);
	divergence.bind();
		divergence.clear();
	divergence.unbind();

	velocity_divergence.bind();
		divergence.bind();
			screen.bind();
				velocity.get_texture().bind();
					velocity_divergence.send_uniform("u_screen", dim::Window::get_size() * quality);
					velocity_divergence.send_uniform("u_velocity", velocity.get_texture());
					screen.draw();
				velocity.get_texture().unbind();
			screen.unbind();
		divergence.unbind();
	velocity_divergence.unbind();
}

void Simulator::compute_velocity_pressure()
{
	screen.send_data(velocity_pressure, dim::Mesh::screen, dim::DataType::Positions | dim::DataType::TexCoords);
	pressure.bind();
		pressure.clear();
	pressure.unbind();
	dim::FrameBuffer pressure_temp(dim::Window::get_size() * quality, dim::Texture::Filtering::Linear, dim::Texture::Warpping::ClampToEdge, dim::Texture::Type::RGB_32f);

	velocity_pressure.bind();
		screen.bind();
			divergence.get_texture().bind();

				velocity_pressure.send_uniform("u_screen", dim::Window::get_size() * quality);
				velocity_pressure.send_uniform("u_divergence", divergence.get_texture());

				for (int i = 0; i < precision; i++)
				{
					pressure_temp.bind();
						pressure.get_texture().bind();
							velocity_pressure.send_uniform("u_pressure", pressure.get_texture());
							screen.draw();
						pressure.get_texture().unbind();
					pressure_temp.unbind();

					std::swap(pressure, pressure_temp);

					pressure_temp.bind();
						pressure_temp.clear();
					pressure_temp.unbind();
				}

			divergence.get_texture().unbind();
		screen.unbind();
	velocity_pressure.unbind();
}

void Simulator::compute_velocity_gradient()
{
	screen.send_data(velocity_gradient, dim::Mesh::screen, dim::DataType::Positions | dim::DataType::TexCoords);
	gradient.bind();
		gradient.clear();
	gradient.unbind();

	velocity_gradient.bind();
		gradient.bind();
			screen.bind();
				pressure.get_texture().bind();
					velocity_gradient.send_uniform("u_screen", dim::Window::get_size() * quality);
					velocity_gradient.send_uniform("u_pressure", pressure.get_texture());
					screen.draw();
				pressure.get_texture().unbind();
			screen.unbind();
		gradient.unbind();
	velocity_gradient.unbind();
}

void Simulator::compute_velocity_subtraction()
{
	screen.send_data(velocity_subtraction, dim::Mesh::screen, dim::DataType::Positions | dim::DataType::TexCoords);
	dim::FrameBuffer velocity_temp(dim::Window::get_size() * quality, dim::Texture::Filtering::Linear, dim::Texture::Warpping::ClampToEdge, dim::Texture::Type::RGB_32f);

	velocity_subtraction.bind();
		velocity_temp.bind();
			screen.bind();
				velocity.get_texture().bind();
				gradient.get_texture().bind();
					velocity_subtraction.send_uniform("u_velocity", velocity.get_texture());
					velocity_subtraction.send_uniform("u_gradient", gradient.get_texture());
					screen.draw();
				gradient.get_texture().unbind();
				velocity.get_texture().unbind();
			screen.unbind();
		velocity_temp.unbind();
	velocity_subtraction.unbind();

	std::swap(velocity, velocity_temp);
}

void Simulator::compute_velocity_boundaries()
{
	screen.send_data(velocity_boundaries, dim::Mesh::screen, dim::DataType::Positions | dim::DataType::TexCoords);
	dim::FrameBuffer velocity_temp(dim::Window::get_size() * quality, dim::Texture::Filtering::Linear, dim::Texture::Warpping::ClampToEdge, dim::Texture::Type::RGB_32f);

	velocity_boundaries.bind();
		velocity_temp.bind();
			screen.bind();
				velocity.get_texture().bind();
					velocity_boundaries.send_uniform("u_screen", dim::Window::get_size() * quality);
					velocity_boundaries.send_uniform("u_velocity", velocity.get_texture());
					screen.draw();
				velocity.get_texture().unbind();
			screen.unbind();
		velocity_temp.unbind();
	velocity_boundaries.unbind();

	std::swap(velocity, velocity_temp);
}

void Simulator::update()
{
	compute_density_sources();
	//compute_density_diffusion();
	compute_density_advection();
	compute_velocity_forces();
	compute_velocity_vorticity();
	//compute_velocity_diffusion();
	compute_velocity_advection();
	compute_velocity_divergence();
	compute_velocity_pressure();
	compute_velocity_gradient();
	compute_velocity_subtraction();
	compute_velocity_boundaries();
}

void Simulator::check_events(const sf::Event& sf_event)
{
	if (sf_event.type == sf::Event::Resized)
		reset();
}

void Simulator::draw()
{
	Renderer::draw();
}
