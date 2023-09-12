#version 430
precision highp float;

in vec2 v_texcoord;

out vec4 frag_color;

uniform vec2 u_screen;
uniform float u_time_step;
uniform float u_vorticity;
uniform sampler2D u_velocity;

vec2 get_velocity(float x, float y)
{
	return texture(u_velocity, v_texcoord + vec2(x, y) / u_screen).xy;
}

float get_curl(float x, float y)
{
	return get_velocity(x, y + 1).x - get_velocity(x, y - 1).x + get_velocity(x - 1, y).y - get_velocity(x + 1, y).y;
}

void main()
{
	vec2 direction = vec2(0.);

	if (!(v_texcoord.x <= 1. / u_screen.x || v_texcoord.y <= 1. / u_screen.y ||
		1. - v_texcoord.x <= 1. / u_screen.x || 1. - v_texcoord.y <= 1. / u_screen.y))
	{
		direction.x = abs(get_curl(0, -1)) - abs(get_curl(0, 1));
		direction.y = abs(get_curl(1, 0)) - abs(get_curl(-1, 0));
		direction *= u_vorticity / (length(direction) + 0.00001);
	}

	vec2 velocity = get_velocity(0, 0) + u_time_step * get_curl(0, 0) * direction;
	frag_color = vec4(velocity, 0., 1.);
}
