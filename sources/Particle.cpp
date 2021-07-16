#include "Particle.h"

Particle::Particle()
{
	position = Vector(0., 0.);
	speed = 0.;
}

Particle::Particle(const Particle& particle)
{
	*this = particle;
}

Particle::Particle(const Vector& position, const double& speed)
{
	this->position = position;
	this->speed = speed;
}

void Particle::operator=(const Particle& particle)
{
	position = particle.position;
	speed = particle.speed;
}

sf::Color Particle::get_color()
{
	return sf::Color();
}