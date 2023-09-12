#version 430
precision highp float;

in vec2 v_texcoord;

out vec4 frag_color;

uniform vec2 u_screen;
uniform float u_time_step;
uniform float u_dissipation;
uniform sampler2D u_density;
uniform sampler2D u_velocity;

void main()
{
	vec2 velocity = texture(u_velocity, v_texcoord).xy;
	vec2 prev_pos = v_texcoord - ((velocity * u_time_step) / u_screen);
	vec3 dissipation = vec3(u_dissipation * 1.75, u_dissipation * 1., u_dissipation * 0.75);
	frag_color = vec4(texture(u_density, prev_pos).rgb * (1. - dissipation), 1.);
}
