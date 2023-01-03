#version 430
precision highp float;

in vec2 v_texcoord;

out vec4 frag_color;

uniform vec2 u_screen;
uniform sampler2D u_divergence;
uniform sampler2D u_pressure;

float get_pressure(float x, float y)
{
	return texture(u_pressure, v_texcoord + vec2(x, y) / u_screen).x;
}

void main()
{
	float d = texture(u_divergence, v_texcoord).x;
	float pressure = (d + get_pressure(-1., 0.) + get_pressure(1., 0.) + get_pressure(0., -1.) + get_pressure(0., 1.)) / 4.;
	frag_color = vec4(pressure, 0., 0., 1.);
}
