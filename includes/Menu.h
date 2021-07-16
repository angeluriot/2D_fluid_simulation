#ifndef MENU_H
#define MENU_H
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>

// Classe définissant une ligne

class Line
{
public:

	sf::RectangleShape rectangle;
	sf::CircleShape left;
	sf::CircleShape right;

	Line();
	Line(const Line& line);
	Line(const double& size, const double& width, const double& x_position, const double& y_position, const sf::Color& color);

	void operator=(const Line& line);

	void set_rotation(const double& angle);
	void draw(sf::RenderWindow& window);
};

// Clase définissant une case à cocher

class Box
{
public:

	Line left;
	Line right;
	Line top;
	Line bottom;
	Line check_left;
	Line check_right;
	sf::RectangleShape hitbox;

	Box();
	Box(const Box& box);
	Box(const double& size, const double& width, const double& x_position, const double& y_position, const sf::Color& color);

	void operator=(const Box& box);

	void draw(sf::RenderWindow& window, bool checked);
};

// Types de variables

enum Variable_type { Bool, Int, Double };

// Types de curseurs

enum Cursor_type { arrow, hand, resize };

// Clase définissant une variable

class Variable
{
public:

	Variable_type type;
	bool bool_value;
	int32_t int_value;
	int32_t int_min;
	int32_t int_max;
	double double_value;
	double double_min;
	double double_max;
	int32_t precision;
	bool percent;
	sf::Text name;
	sf::Text value;
	Box box;
	bool checked;
	Line line;
	sf::CircleShape circle;
	bool grabbed;
	bool grab_forbiden;

	Variable();
	Variable(const Variable& variable);
	Variable(const std::string& name, Variable_type type, const double& initial_value, double min = 0., double max = 1., uint8_t precision = 0, bool percent = false);

	void operator=(const Variable& variable);

	bool is_in(sf::Vector2f position);
	sf::Vector2f get_circle_position();
	double get_value_from_circle();
	void update(const sf::Vector2i& previous_mouse_position, const sf::Vector2i& mouse_position, bool left_click);
	void draw(sf::RenderWindow& window);
};

// Clase définissant le menu

class Menu
{
public:

	std::vector<Variable> variables;
	sf::Color background;
	sf::Font texts_font;
	sf::Texture start_texture;
	sf::Texture start_texture_grabbed;
	sf::RectangleShape start_button;
	bool grabbed;
	bool grab_forbiden;
	bool end;

	Menu();
	Menu(const Menu& menu);
	Menu(const std::vector<Variable>& variables, const sf::Color& background);

	void create(const std::vector<Variable>& variables, const sf::Color& background);
	void init_names(uint16_t name_size, const double& names_position, const sf::Color& name_color, const sf::RenderWindow& window, const double& y_edges);
	void init_lines(const double& lines_sizes, const double& lines_widths, const double& lines_positions, const sf::Color& line_color);
	void init_circles(const double& circles_radius, const sf::Color& circle_color);
	void init_values(uint16_t value_size, const double& values_position, const sf::Color& value_color);
	void init_boxes(const double& boxes_sizes, const double& boxes_position, const sf::Color& boxes_color);
	void init_start(const double& start_size, const double& start_x_position, const double& start_y_position, const sf::Color& start_color);

	void operator=(const Menu& menu);

	bool is_in(sf::Vector2f position);
	void update(const sf::Vector2i& previous_mouse_position, const sf::Vector2i& mouse_position, bool left_click, sf::Cursor& cursor, Cursor_type& cursor_type, sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);
};

// Classe définissant un bouton pour recommencer

class Restart
{
public :

	sf::RectangleShape button;
	sf::Texture texture;
	sf::Texture texture_grabbed;
	bool grabbed;
	bool grab_forbiden;

	Restart();
	Restart(const Restart& restart);
	Restart(const double& start_size, const double& start_x_position, const double& start_y_position, const sf::Color& start_color);

	void operator=(const Restart& restart);

	bool is_in(sf::Vector2f position);
	bool update(const sf::Vector2i& mouse_position, bool left_click, sf::Cursor& cursor, Cursor_type& cursor_type, sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);
};

std::string double_to_string(const double& value, uint8_t precision, bool percent);

#endif