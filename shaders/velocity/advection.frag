#version 430
precision highp float;

in vec2 v_texcoord;

out vec4 frag_color;

uniform vec2 u_screen;
uniform float u_time_step;
uniform sampler2D u_density;
uniform sampler2D u_velocity;

void main()
{
	vec2 velocity = texture2D(u_velocity, v_texcoord).xy;
	vec2 prev_pos = v_texcoord - ((velocity * u_time_step) / u_screen);
	frag_color = vec4(texture2D(u_density, prev_pos).rgb, 1.);
}
