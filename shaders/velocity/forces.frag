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
	vec2 move = (u_mouse - u_prev_mouse) * vec2(pow(u_screen.x, 1.5), pow(u_screen.y, 1.5)) * 0.02;
	float strength = clamp(3. - pow(distance(v_texcoord * u_screen, u_mouse * u_screen), 2) / (u_strength * 1.2), 0., 3.);
	frag_color = vec4(texture2D(u_velocity, v_texcoord).xy + move * strength, 0., 1.);
}
