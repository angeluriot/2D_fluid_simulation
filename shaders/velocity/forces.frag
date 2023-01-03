#version 430
precision highp float;

in vec2 v_texcoord;

out vec4 frag_color;

uniform vec2 u_screen;
uniform vec2 u_mouse;
uniform vec2 u_prev_mouse;
uniform float u_strength;
uniform sampler2D u_velocity;

void main()
{
	vec2 screen = 100. * u_screen / ((u_screen.x + u_screen.y) / 2.);
	vec2 move = (u_mouse - u_prev_mouse) * screen;
	vec2 coord = (u_mouse - v_texcoord) * screen;
	vec2 splat = move * exp(-dot(coord, coord) / u_strength);
	frag_color = vec4(texture(u_velocity, v_texcoord).xy + splat, 0., 1.);
}
