#version 430
precision highp float;

in vec2 v_texcoord;

out vec4 frag_color;

uniform vec2 u_screen;
uniform sampler2D u_velocity;

vec2 get_velocity(float x, float y)
{
	return texture2D(u_velocity, v_texcoord + vec2(x, y) / u_screen).xy;
}

void main()
{
	float h = 1. / ((u_screen.x + u_screen.y) / 2.);
	float divergence = - 0.5 * h * (get_velocity(1., 0.).x - get_velocity(-1., 0.).x + get_velocity(0., 1.).y - get_velocity(0., -1.).y);
	frag_color = vec4(divergence, 0., 0., 1.);
}
