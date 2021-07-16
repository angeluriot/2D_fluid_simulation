#include "Fluid.h"

Fluid::Fluid()
{
	size_x = 0;
	size_y = 0;
	viscosity = 0.;
	mouse_strength = 0.;
	time_step = 0.;
	viscous_precision = 0.;
	pressure_precision = 0.;
	pressure.clear();
	temp_pressure.clear();
	pressure_gradient.clear();
	speed.clear();
	temp_speed.clear();
	speed_divergence.clear();
	particles.clear();
}

Fluid::Fluid(const Fluid& fluid)
{
	*this = fluid;
}

Fluid::Fluid(const double& viscosity, const double& mouse_strength, const double& time_step, const double& viscous_precision, const double& pressure_precision, uint16_t size_x, uint16_t size_y)
{
	this->size_x = size_x;
	this->size_y = size_y;
	this->viscosity = viscosity;
	this->mouse_strength = mouse_strength;
	this->time_step = time_step;
	this->viscous_precision = viscous_precision;
	this->pressure_precision = pressure_precision;

	pressure.clear();
	temp_pressure.clear();
	pressure_gradient.clear();
	speed.clear();
	temp_speed.clear();
	speed_divergence.clear();
	particles.clear();

	std::vector<double> scalar_temp(size_y + 2, 0.);
	std::vector<Vector> vector_temp(size_y + 2, Vector(0., 0.));
	std::vector<double> scalar_temp_1(size_y + 2, 1.);

	pressure = Scalar_field(size_x + 2, scalar_temp);
	temp_pressure = Scalar_field(size_x + 2, scalar_temp);
	pressure_gradient = Vector_field(size_x + 2, vector_temp);
	speed = Vector_field(size_x + 2, vector_temp);
	temp_speed = Vector_field(size_x + 2, vector_temp);
	speed_divergence = Scalar_field(size_x + 2, scalar_temp);

	for (uint16_t x = 1; x < size_x; x++)
		for (uint16_t y = 1; y < size_y; y++)
		{
			particles.push_back(Particle(Vector(x, y), 0.));
			particles.push_back(Particle(Vector(x + 0.5, y + 0.5), 0.));
		}
}

void Fluid::operator=(const Fluid& fluid)
{
	size_x = fluid.size_x;
	size_y = fluid.size_y;
	viscosity = fluid.viscosity;
	mouse_strength = fluid.mouse_strength;
	time_step = fluid.time_step;
	viscous_precision = fluid.viscous_precision;
	pressure_precision = fluid.pressure_precision;
	pressure = fluid.pressure;
	pressure_gradient = fluid.pressure_gradient;
	temp_pressure = fluid.temp_pressure;
	speed = fluid.speed;
	temp_speed = fluid.temp_speed;
	speed_divergence = fluid.speed_divergence;
	particles = fluid.particles;
}

Vector Fluid::speed_value(const Vector& position)
{
	double x_comma = position.x - floor(position.x);
	double y_comma = position.y - floor(position.y);

	if (position.x >= 0 && position.x < size_x + 1 && position.y >= 0 && position.y < size_y + 1)
		return ((speed[position.x][position.y]			* (2. - x_comma - y_comma))	+	// 1 - la distance entre (int_x,		int_y)		fois sa valeur
				(speed[position.x + 1][position.y]		* (x_comma + 1. - y_comma))	+	// 1 - la distance entre (int_x + 1,	int_y)		fois sa valeur
				(speed[position.x][position.y + 1]		* (1. - x_comma + y_comma))	+	// 1 - la distance entre (int_x,		int_y + 1)	fois sa valeur
				(speed[position.x + 1][position.y + 1]	* (x_comma + y_comma)))			// 1 - la distance entre (int_x + 1,	int_y + 1)	fois sa valeur
				/ 4.;																	// La somme de chaque distances (les poids) vaut 4

	else
		return Vector(0., 0.);
}

Vector Fluid::screen_to_simulation(const Vector& position)
{
	return (((position * RESIZE) / static_cast<double>(WIDTH)) * (size_x - 2)) + Vector(1., 1.);
}

Vector Fluid::simulation_to_screen(const Vector& position)
{
	return (((position - Vector(1., 1.)) / static_cast<double>(size_x - 2)) * WIDTH) / RESIZE;
}

void Fluid::reset_particles()
{
	auto it = particles.begin();

	for (uint16_t x = 1; x < size_x; x++)
		for (uint16_t y = 1; y < size_y; y++)
		{
			it->position = Vector(x, y);
			it++;
			it->position = Vector(x + 0.5, y + 0.5);
			it++;
		}
}

void Fluid::advection_update()
{
	for (uint16_t x = 1; x < size_x + 1; x++)
		for (uint16_t y = 1; y < size_y + 1; y++)
			temp_speed[x][y] = speed_value(Vector(x, y) - (speed[x][y] * time_step));

	std::swap(temp_speed, speed);
}

void Fluid::viscous_diffusion_update()
{
	double alpha = 1. / (viscosity * time_step);
	double beta = 4. + alpha;

	for (uint8_t k = 0; k < viscous_precision; k++)
	{
		for (uint16_t x = 1; x < size_x + 1; x++)
			for (uint16_t y = 1; y < size_y + 1; y++)
				temp_speed[x][y] = (speed[x - 1][y] + speed[x + 1][y] + speed[x][y - 1] + speed[x][y + 1] + (alpha * speed[x][y])) / beta;

		std::swap(temp_speed, speed);
	}
}

void Fluid::external_forces_update(const Vector& mouse_position, const Vector& mouse_move)
{
	for (uint16_t x = 1; x < size_x + 1; x++)
		for (uint16_t y = 1; y < size_y + 1; y++)
			speed[x][y] += mouse_move * mouse_strength * (1. / (get_distance2(mouse_position, Vector(x, y)) + 0.1));
}

void Fluid::divergence_update()
{
	for (uint16_t x = 1; x < size_x + 1; x++)
		for (uint16_t y = 1; y < size_y + 1; y++)
			speed_divergence[x][y] = (speed[x + 1][y].x - speed[x - 1][y].x + speed[x][y + 1].y - speed[x][y - 1].y) / 2.;
}

void Fluid::gradient_update()
{
	for (uint16_t x = 1; x < size_x + 1; x++)
		for (uint16_t y = 1; y < size_y + 1; y++)
			pressure_gradient[x][y] = Vector((pressure[x + 1][y] - pressure[x - 1][y]) / 2., (pressure[x][y + 1] - pressure[x][y - 1]) / 2.);
}

void Fluid::pressure_update()
{
	divergence_update();

	std::vector<double> scalar_temp(size_y + 2, 0.);
	std::fill(pressure.begin(), pressure.end(), scalar_temp);

	for (uint8_t k = 0; k < pressure_precision; k++)
	{
		for (uint16_t x = 1; x < size_x + 1; x++)
			for (uint16_t y = 1; y < size_y + 1; y++)
				temp_pressure[x][y] = (pressure[x - 1][y] + pressure[x + 1][y] + pressure[x][y - 1] + pressure[x][y + 1] - speed_divergence[x][y]) / 4.;

		std::swap(temp_pressure, pressure);
	}

	gradient_update();

	for (uint16_t x = 1; x < size_x + 1; x++)
		for (uint16_t y = 1; y < size_y + 1; y++)
			speed[x][y] -= pressure_gradient[x][y];
}

void Fluid::border_update()
{
	for (uint16_t x = 1; x < size_x + 1; x++)
	{
		speed[x].front() = speed[x][size_y];
		speed[x].back() = speed[x][1];
	}

	for (uint16_t y = 1; y < size_y + 1; y++)
	{
		speed.front()[y] = speed[size_x][y];
		speed.back()[y] = speed[1][y];
	}

	speed.front().front() = speed[size_x][size_y];
	speed.back().front() = speed[1][size_y];
	speed.front().back() = speed[size_x][1];
	speed.back().back() = speed[1][1];
}

void Fluid::particles_update()
{
	Vector particle_speed;

	for (auto& particle : particles)
	{
		particle_speed = speed_value(particle.position);
		particle.speed = particle_speed.get_norm();
		particle.position += particle_speed * time_step;

		if (particle.position.x < 0.5)
		{
			particle.position.x = size_x;
			particle.position += particle_speed * time_step;
		}

		else if (particle.position.x > size_x + 0.5)
		{
			particle.position.x = 1.;
			particle.position += particle_speed * time_step;
		}

		else if (particle.position.y < 0.5)
		{
			particle.position.y = size_y;
			particle.position += particle_speed * time_step;
		}

		else if (particle.position.y > size_y + 0.5)
		{
			particle.position.y = 1.;
			particle.position += particle_speed * time_step;
		}
	}
}

void Fluid::update(const Vector& mouse_position, const Vector& mouse_move, bool clicked)
{
	advection_update();
	viscous_diffusion_update();

	if (clicked && mouse_position.x > 0 && mouse_position.y > 0 && mouse_position.x < size_x + 1 && mouse_position.y < size_y + 1)
		external_forces_update(mouse_position, mouse_move);

	pressure_update();
	border_update();
	particles_update();
}