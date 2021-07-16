#ifndef FLUID_H
#define FLUID_H
#include "Particle.h"

typedef std::vector<std::vector<double>> Scalar_field;
typedef std::vector<std::vector<Vector>> Vector_field;

// Classe définissant un fluide

class Fluid
{
public:

	uint16_t size_x;
	uint16_t size_y;
	double viscosity;
	double mouse_strength;
	double time_step;
	double viscous_precision;
	double pressure_precision;
	Scalar_field pressure;
	Scalar_field temp_pressure;
	Vector_field pressure_gradient;
	Vector_field speed;
	Vector_field temp_speed;
	Scalar_field speed_divergence;
	std::list<Particle> particles;

	Fluid();
	Fluid(const Fluid& fluid);
	Fluid(const double& viscosity, const double& mouse_strength, const double& time_step, const double& viscous_precision, const double& pressure_precision, uint16_t size_x, uint16_t size_y);

	void operator=(const Fluid& fluid);

	Vector speed_value(const Vector& position);
	Vector screen_to_simulation(const Vector& position);
	Vector simulation_to_screen(const Vector& position);
	void reset_particles();

	void advection_update();
	void viscous_diffusion_update();
	void external_forces_update(const Vector& mouse_position, const Vector& mouse_move);
	void divergence_update();
	void gradient_update();
	void pressure_update();
	void border_update();
	void particles_update();
	void update(const Vector& mouse_position, const Vector& mouse_move, bool clicked);
};

#endif