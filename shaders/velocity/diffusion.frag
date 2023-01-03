#version 430
precision highp float;

in vec2 v_texcoord;

out vec4 frag_color;

uniform vec2 u_screen;
uniform float u_time_step;
uniform float u_viscosity;
uniform sampler2D u_velocity;
uniform sampler2D u_velocity_temp;

vec2 get_velocity(float x, float y)
{
	return texture(u_velocity_temp, v_texcoord + vec2(x, y) / u_screen).xy;
}

void main()
{
	float k = u_time_step * u_viscosity * u_screen.x * u_screen.y;
	vec2 v = texture(u_velocity, v_texcoord).xy;
	vec2 velocity = (v + k * (get_velocity(-1., 0.) + get_velocity(1., 0.) + get_velocity(0., -1.) + get_velocity(0., 1.))) / (1. + 4. * k);
	frag_color = vec4(velocity, 0., 1.);
}
