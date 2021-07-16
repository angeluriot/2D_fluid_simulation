#ifndef SIMULATION_H
#define SIMULATION_H
#include "Menu.h"
#include "Fluid.h"

// Classe définissant la simulation

class Simulation
{
public:

	sf::Image			void_image;		// Image vide
	sf::Image			image;			// Image à afficher
	sf::Texture			texture;		// Texture supportant l'image
	sf::CircleShape		particle;		//
	sf::RenderTexture	render_texture;	// Texture à afficher
	sf::Sprite			sprite;			// Le sprite servant à afficher la texture
	sf::RenderWindow*	window;			// Pointeur sur la fenêtre
	Fluid				fluid;			// Le fluide simulé

	Simulation();
	Simulation(const Menu& menu, sf::RenderWindow* window);

	void restart(const Menu& menu, sf::RenderWindow* window);
	void draw_simulation();
	void update(const Vector& mouse_position, const Vector& previous_mouse_position, bool left_clicked, bool right_clicked);
	void draw();
};

#endif