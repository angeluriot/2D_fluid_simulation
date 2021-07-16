#ifndef PARTICLE_H
#define PARTICLE_H
#include "utils.h"
#include "Vector.h"

class Particle
{
public:

	Vector position;
	double speed;

	Particle();
	Particle(const Particle& particle);
	Particle(const Vector& position, const double& speed);

	void operator=(const Particle& particle);

	sf::Color get_color();
};

#endif