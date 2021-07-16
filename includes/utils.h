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

extern uint16_t				screen_width;					// Largeur de l'écran

#define		WIDTH			1920							// Largeur de l'écran
#define		HEIGHT			1080							// Hauteeur de l'écran
#define		RESIZE			((1. / 1920.) * screen_width)	// Coéfficent en fonction de la taille de l'écran
#define		TIME_STEP		0.01666666						// Temps entre deux updates
#define		PARTICLE_SIZE	7.								//

void		event_check(sf::Event& sf_event, sf::RenderWindow& window, bool& end);
uint32_t	random_int(int32_t min, int32_t max);
bool		rand_probability(const double& probability);

#endif