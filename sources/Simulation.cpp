#include "Simulation.h"

// Crée une simulation

Simulation::Simulation()
{
	window = NULL;
}

// Crée une simulation à partir des données du menu

Simulation::Simulation(const Menu& menu, sf::RenderWindow* window)
{
	restart(menu, window);
}

// Recrrée une simulation à partir des données du menu

void Simulation::restart(const Menu& menu, sf::RenderWindow* window)
{
	void_image.create(menu.variables[1].int_value, menu.variables[1].int_value * (9. / 16.) + 1, sf::Color(0, 0, 0));
	image = void_image;

	texture.create(WIDTH, HEIGHT);

	render_texture.create(WIDTH, HEIGHT);
	render_texture.clear(sf::Color::Black);
	render_texture.display();

	sprite = sf::Sprite(render_texture.getTexture());

	particle.setRadius(PARTICLE_SIZE / 2.);
	particle.setOrigin(PARTICLE_SIZE / 2., PARTICLE_SIZE / 2.);
	particle.setFillColor(sf::Color(0, 255, 255));

	this->window = window;

	fluid = Fluid(menu.variables[0].double_value, 1.5, TIME_STEP, 20, 20, menu.variables[1].int_value, menu.variables[1].int_value * (9. / 16.) + 1);

	draw_simulation();
}

// Affiche la simulation

void Simulation::draw_simulation()
{
	render_texture.clear(sf::Color(0, 0, 0));

	for (auto& p : fluid.particles)
	{
		particle.setPosition(fluid.simulation_to_screen(p.position).x, fluid.simulation_to_screen(p.position).y);
		particle.setFillColor(sf::Color((p.speed * 10. > 255. * 2. ? std::min(p.speed * 10. - 255. * 2., 255.) : 0.), (p.speed * 10. > 255. ? std::min(p.speed * 10. - 255., 255.) : 0.), std::min(p.speed * 10., 255.)));
		render_texture.draw(particle);
	}
}

// Met à jour la simulation

void Simulation::update(const Vector& mouse_position, const Vector& previous_mouse_position, bool left_clicked, bool right_clicked)
{
	static bool already_reset = false;

	fluid.update(fluid.screen_to_simulation(mouse_position), (mouse_position - previous_mouse_position) * RESIZE, left_clicked);

	if (right_clicked && !already_reset)
	{
		fluid.reset_particles();
		already_reset = true;
	}

	if (!right_clicked)
		already_reset = false;

	draw_simulation();
}

// Affiche la simulation

void Simulation::draw()
{
	render_texture.display();
	sprite = sf::Sprite(render_texture.getTexture());
	
	window->draw(sprite);
}