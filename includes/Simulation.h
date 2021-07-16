#ifndef SIMULATION_H
#define SIMULATION_H
#include "Menu.h"
#include "Fluid.h"

// Classe d�finissant la simulation

class Simulation
{
public:

	sf::Image			void_image;		// Image vide
	sf::Image			image;			// Image � afficher
	sf::Texture			texture;		// Texture supportant l'image
	sf::CircleShape		particle;		//
	sf::RenderTexture	render_texture;	// Texture � afficher
	sf::Sprite			sprite;			// Le sprite servant � afficher la texture
	sf::RenderWindow*	window;			// Pointeur sur la fen�tre
	Fluid				fluid;			// Le fluide simul�

	Simulation();
	Simulation(const Menu& menu, sf::RenderWindow* window);

	void restart(const Menu& menu, sf::RenderWindow* window);
	void draw_simulation();
	void update(const Vector& mouse_position, const Vector& previous_mouse_position, bool left_clicked, bool right_clicked);
	void draw();
};

#endif