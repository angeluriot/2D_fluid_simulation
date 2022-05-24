#version 430
precision highp float;

in vec2 v_texcoord;

out vec4 frag_color;

uniform vec2 u_screen;
uniform float u_time_step;
uniform float u_viscosity;
uniform sampler2D u_density;
uniform sampler2D u_density_temp;

vec3 get_density(float x, float y)
{
	return texture2D(u_density_temp, v_texcoord + vec2(x, y) / u_screen).rgb;
}

void main()
{
	float k = u_time_step * u_viscosity * u_screen.x * u_screen.y;
	vec3 d = texture2D(u_density, v_texcoord).rgb;
	vec3 density = (d + k * (get_density(-1., 0.) + get_density(1., 0.) + get_density(0., -1.) + get_density(0., 1.))) / (1. + 4. * k);
	frag_color = vec4(density, 1.);
}
