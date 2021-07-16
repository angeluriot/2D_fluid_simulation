#ifndef UTILS_H
#define UTILS_H
#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>
#include <vector>
#include <array>
#include <ctype.h>
#include <iostream>
#include <list>

extern uint16_t				screen_width;					// Largeur de l'�cran

#define		WIDTH			1920							// Largeur de l'�cran
#define		HEIGHT			1080							// Hauteeur de l'�cran
#define		RESIZE			((1. / 1920.) * screen_width)	// Co�fficent en fonction de la taille de l'�cran
#define		TIME_STEP		0.01666666						// Temps entre deux updates
#define		PARTICLE_SIZE	7.								//

void		event_check(sf::Event& sf_event, sf::RenderWindow& window, bool& end);
uint32_t	random_int(int32_t min, int32_t max);
bool		rand_probability(const double& probability);

#endif