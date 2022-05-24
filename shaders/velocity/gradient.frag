#version 430
precision highp float;

in vec2 v_texcoord;

out vec4 frag_color;

uniform vec2 u_screen;
uniform sampler2D u_pressure;

float get_pressure(float x, float y)
{
	return texture2D(u_pressure, v_texcoord + vec2(x, y) / u_screen).x;
}

void main()
{
	float h = 1. / ((u_screen.x + u_screen.y) / 2.);
	vec2 gradient = vec2(0.5 * (get_pressure(1., 0.) - get_pressure(-1., 0.)) / h, 0.5 * (get_pressure(0., 1.) - get_pressure(0., -1.)) / h);
	frag_color = vec4(gradient, 0., 1.);
}
